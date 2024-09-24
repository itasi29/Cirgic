#include "SceneOption.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "Common/Input.h"
#include "Common/Application.h"
#include "Common/CommandList.h"
#include "Common/SoundSystem.h"
#include "Geometry/Easing.h"
#include "File/FileManager.h"
#include "File/FileBase.h"
#include "File/FontSystem.h"
#include "File/FileID.h"

namespace
{
	enum FileIndex : int
	{
		FRAME,
		SELECT_FRAME_L,
		SELECT_FRAME_R,
		OPTION_FRAME,
		SE_SELECT,
		SE_CANCEL,
		SE_CURSOR_MOVE,
		SE_NOT_CURSOR_MOVE,
	};
	enum OptionSelectIndex : int
	{
		BGM,
		SE,
		LIGHT,
		MAX
	};

	// フレーム描画関係
	constexpr int FRAME_DRAW_X = 128;
	constexpr int FRAME_DRAW_Y = 128;
	constexpr int INTERVAL_DRAW_Y = 128;
	// 選択フレームの描画関係
	constexpr int SELECT_FRAME_DRAW_RIGHT = 308;
	constexpr int SELECT_FRAME_DRAW_LEFT = 100;
	constexpr int SELECT_FRAME_SWING_WIDTH = 10;
	constexpr float SELECT_FRAME_CYCLE_SPEED = 0.03f;
	constexpr float SELECT_FRAME_SLIDE_SPEED = 0.2f;

	constexpr int FRAME_RETURN_DRAW_X = 420;
	constexpr int FRAME_RETURN_DRAW_Y = FRAME_DRAW_Y + INTERVAL_DRAW_Y * 2;
	constexpr int SELECT_RETURN_FRAME_DRAW_RIGHT = 600;
	constexpr int SELECT_RETURN_FRAME_DRAW_LEFT = 392;
	constexpr int FRAME_RETURN_INTERVAL = 300;
	// 音レベルの描画関係
	constexpr int LEVEL_DRAW_X_1 = 500;
	constexpr int LEVEL_DRAW_X_2 = 564;
	constexpr int LEVEL_DRAW_Y_1 = 128;
	constexpr int LEVEL_DRAW_Y_2 = 192;
	constexpr int LEVEL_ADD_DRAW_X = 128;
	// 文字
	constexpr int STR_DRAW_X = 160;
	constexpr int STR_DRAW_Y = 144;
	constexpr int STR_RETURN_DRAW_X = 480;
	constexpr int STR_RETURN_DRAW_Y = 262;

	constexpr int CHANGE_VOL = 51;
}

SceneOption::SceneOption() :
	SceneBase(Kind::OPTION),
	m_updateInputFunc(&SceneOption::UpdateInputNormal),
	m_isNowOpen(true),
	m_isReturnTitle(false),
	m_isOpenReturnCheck(false),
	m_indexNormal(0),
	m_indexReturn(0)
{
	auto& info = m_selectFrameInfo;
	info.rightX = SELECT_FRAME_DRAW_RIGHT;
	info.leftX = SELECT_FRAME_DRAW_LEFT;
	info.nextRightX = info.rightX;
	info.nextLeftX = info.leftX;
	info.y = FRAME_DRAW_Y + 35;
}

void SceneOption::Init()
{
	// 画像読み込み
	auto& fileMgr = FileManager::GetInstance();
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME, true));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_SELECT_LEFT, true));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_SELECT_RIGHT, true));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_OPTION, true));
	m_files.push_back(fileMgr.Load(FileID::Snd::SE::SELECT, true));
	m_files.push_back(fileMgr.Load(FileID::Snd::SE::CANCEL, true));
	m_files.push_back(fileMgr.Load(FileID::Snd::SE::CURSOR_MOVE, true));
	m_files.push_back(fileMgr.Load(FileID::Snd::SE::NOT_CURSOR_MOVE, true));

	auto& sndSys = SoundSystem::GetInstance();
	sndSys.StopNowPlaySe();
}

void SceneOption::Update(bool isFade)
{
	if (m_isNowOpen)
	{
		m_isNowOpen = false;
		auto& sndSys = SoundSystem::GetInstance();
		sndSys.PlaySe(m_files[FileIndex::SE_SELECT]->GetHandle(), true);
	}
	UpdateInput();
	UpdateFrame();
}

void SceneOption::Draw() const
{
	DrawRotaGraph(640, 360, 1.0, 0.0, m_files[FileIndex::OPTION_FRAME]->GetHandle(), true);
	// フレーム
	DrawRotaGraphFast3F(FRAME_DRAW_X, FRAME_DRAW_Y + INTERVAL_DRAW_Y * 0, 0, 0, 0.125f, 0.5f, 0.0f, m_files[FileIndex::FRAME]->GetHandle(), true);
	DrawRotaGraphFast3F(FRAME_DRAW_X, FRAME_DRAW_Y + INTERVAL_DRAW_Y * 1, 0, 0, 0.125f, 0.5f, 0.0f, m_files[FileIndex::FRAME]->GetHandle(), true);
	DrawRotaGraphFast3F(FRAME_DRAW_X, FRAME_DRAW_Y + INTERVAL_DRAW_Y * 2, 0, 0, 0.125f, 0.5f, 0.0f, m_files[FileIndex::FRAME]->GetHandle(), true);
	// 文字
	auto& font = FontSystem::GetInstance();
	int fontH = font.GetHandle(32);
	DrawStringToHandle(STR_DRAW_X +  4, STR_DRAW_Y + INTERVAL_DRAW_Y * 0, L"BGM", 0x0, fontH);
	DrawStringToHandle(STR_DRAW_X + 16, STR_DRAW_Y + INTERVAL_DRAW_Y * 1, L"SE", 0x0, fontH);
	DrawStringToHandle(STR_DRAW_X     , STR_DRAW_Y + INTERVAL_DRAW_Y * 2, L"明るさ", 0x0, fontH);
	if (m_isReturnTitle)
	{
		DrawRotaGraphFast3F(FRAME_DRAW_X, FRAME_DRAW_Y + INTERVAL_DRAW_Y * 3, 0, 0, 0.125f, 0.5f, 0.0f, m_files[FileIndex::FRAME]->GetHandle(), true);
		DrawStringToHandle(STR_DRAW_X - 32, STR_DRAW_Y + INTERVAL_DRAW_Y * 3, L"タイトルへ", 0x0, fontH);
	}

	// レベル
	auto& sound = SoundSystem::GetInstance();
	DrawLevel(sound.GetBgmVol(), 0);
	DrawLevel(sound.GetSeVol(), 1);
	auto& app = Application::GetInstance();
	DrawLevel(app.GetScreenLight(), 2);

	// タイトルに戻るチェック
	if (m_isOpenReturnCheck)
	{
		DrawRotaGraph(640, 360, 0.5, 0.0, m_files[FileIndex::OPTION_FRAME]->GetHandle(), true);

		DrawRotaGraphFast3F(FRAME_RETURN_DRAW_X, FRAME_RETURN_DRAW_Y, 0, 0, 0.125f, 0.5f, 0.0f, m_files[FileIndex::FRAME]->GetHandle(), true);
		DrawRotaGraphFast3F(FRAME_RETURN_DRAW_X + FRAME_RETURN_INTERVAL, FRAME_RETURN_DRAW_Y, 0, 0, 0.125f, 0.5f, 0.0f, m_files[FileIndex::FRAME]->GetHandle(), true);

		DrawStringToHandle(STR_RETURN_DRAW_X, STR_RETURN_DRAW_Y, L"タイトルに戻りますか？", 0xffffff, fontH);
		DrawStringToHandle(FRAME_RETURN_DRAW_X                         + 40, FRAME_RETURN_DRAW_Y + 16, L"YES", 0x0, fontH);
		DrawStringToHandle(FRAME_RETURN_DRAW_X + FRAME_RETURN_INTERVAL + 46 , FRAME_RETURN_DRAW_Y + 16, L"NO", 0x0, fontH);
	}

	// 選択フレーム
	int xL = m_selectFrameInfo.leftX;
	int xR = m_selectFrameInfo.rightX;
	int y = m_selectFrameInfo.y;
	DrawRotaGraph(xL, y, 0.5, 0.0, m_files[FileIndex::SELECT_FRAME_L]->GetHandle(), true);
	DrawRotaGraph(xR, y, 0.5, 0.0, m_files[FileIndex::SELECT_FRAME_R]->GetHandle(), true);
}

void SceneOption::End()
{
	auto& sndSys = SoundSystem::GetInstance();
	sndSys.RestartStopedSe();
}

void SceneOption::UpdateInput()
{
	(this->*m_updateInputFunc)();
}

void SceneOption::UpdateFrame()
{
	auto& info = m_selectFrameInfo;

	info.cycleCount += SELECT_FRAME_CYCLE_SPEED;
	float sin = std::abs(std::sin(info.cycleCount));
	int width = static_cast<int>(SELECT_FRAME_SWING_WIDTH * 0.5f);

	int rs = info.nextRightX + width;
	int re = info.nextRightX - width;
	info.rightX = EaseIn(rs, re, sin);
	int ls = info.nextLeftX - width;
	int le = info.nextLeftX + width;
	info.leftX = EaseIn(ls, le, sin);

	if (info.isMove)
	{
		info.rate += 0.2f;
		if (info.rate > 1.0f)
		{
			info.rate = 1.0f;
			info.isMove = false;
		}
		info.leftX = EaseInOut(info.preLeftX, info.nextLeftX, info.rate);
		info.rightX = EaseInOut(info.preRightX, info.nextRightX, info.rate);
		info.y = EaseInOut(info.preY, info.nextY, info.rate);
	}
}

void SceneOption::UpdateInputNormal()
{
	auto& input = Input::GetInstance();
	auto& sndSys = SoundSystem::GetInstance();
	// 戻る
	if (input.IsTriggerd(0, Command::BTN_BACK))
	{
		sndSys.PlaySe(m_files[FileIndex::SE_CANCEL]->GetHandle(), true);
		m_scnMgr.OptionClose();
		return;
	}
	// 決定
	if (m_isReturnTitle && input.IsTriggerd(0, Command::BTN_OK) && m_indexNormal == OptionSelectIndex::MAX)
	{
		sndSys.PlaySe(m_files[FileIndex::SE_SELECT]->GetHandle(), true);
		m_updateInputFunc = &SceneOption::UpdateInputReturn;
		m_isOpenReturnCheck = true;
		m_indexReturn = 0;
		StartSelectFrameMove(SELECT_RETURN_FRAME_DRAW_LEFT, SELECT_RETURN_FRAME_DRAW_RIGHT, FRAME_RETURN_DRAW_Y + 35);
		return;
	}
	int max = OptionSelectIndex::MAX;
	if (m_isReturnTitle)
	{
		max += 1;
	}
	// 上
	if (input.IsTriggerd(0, Command::BTN_UP))
	{
		sndSys.PlaySe(m_files[FileIndex::SE_CURSOR_MOVE]->GetHandle(), true);
		m_indexNormal = (m_indexNormal - 1 + max) % max;
		auto& info = m_selectFrameInfo;
		StartSelectFrameMove(info.leftX, info.rightX, FRAME_DRAW_Y + INTERVAL_DRAW_Y * m_indexNormal + 35);
	}
	// 下
	if (input.IsTriggerd(0, Command::BTN_DOWN))
	{
		sndSys.PlaySe(m_files[FileIndex::SE_CURSOR_MOVE]->GetHandle(), true);
		m_indexNormal = (m_indexNormal + 1) % max;
		auto& info = m_selectFrameInfo;
		StartSelectFrameMove(info.leftX, info.rightX, FRAME_DRAW_Y + INTERVAL_DRAW_Y * m_indexNormal + 35);
	}
	// 右
	if (input.IsTriggerd(0, Command::BTN_RIGHT))
	{
		auto& app = Application::GetInstance();
		bool isChange = false;
		if (m_indexNormal == OptionSelectIndex::BGM)
		{
			isChange = sndSys.ChangeBgmVol(CHANGE_VOL);
		}
		else if (m_indexNormal == OptionSelectIndex::SE)
		{
			isChange = sndSys.ChangeSeVol(CHANGE_VOL);
		}
		else if (m_indexNormal == OptionSelectIndex::LIGHT)
		{
			isChange = app.AddScreenLight(CHANGE_VOL);
		}
		else if (m_isReturnTitle)
		{
			return;
		}

		if (isChange)
		{
			sndSys.PlaySe(m_files[FileIndex::SE_CURSOR_MOVE]->GetHandle(), true);
		}
		else
		{
			sndSys.PlaySe(m_files[FileIndex::SE_NOT_CURSOR_MOVE]->GetHandle(), true);
		}
	}
	// 左
	if (input.IsTriggerd(0, Command::BTN_LEFT))
	{
		auto& app = Application::GetInstance();
		bool isChange = false;
		if (m_indexNormal == OptionSelectIndex::BGM)
		{
			isChange = sndSys.ChangeBgmVol(-CHANGE_VOL);
		}
		else if (m_indexNormal == OptionSelectIndex::SE)
		{
			isChange = sndSys.ChangeSeVol(-CHANGE_VOL);
		}
		else if (m_indexNormal == OptionSelectIndex::LIGHT)
		{
			isChange = app.SubScreenLight(CHANGE_VOL);
		}
		else if (m_isReturnTitle)
		{
			return;
		}

		if (isChange)
		{
			sndSys.PlaySe(m_files[FileIndex::SE_CURSOR_MOVE]->GetHandle(), true);
		}
		else
		{
			sndSys.PlaySe(m_files[FileIndex::SE_NOT_CURSOR_MOVE]->GetHandle(), true);
		}
	}
}

void SceneOption::UpdateInputReturn()
{
	auto& input = Input::GetInstance();
	auto& sndSys = SoundSystem::GetInstance();
	bool isReturnNormal = false;
	if (input.IsTriggerd(0, Command::BTN_OK))
	{
		if (m_indexReturn == 0)
		{
			sndSys.PlaySe(m_files[FileIndex::SE_SELECT]->GetHandle(), true);
			m_nextSceneKind = Kind::TITLE;
			auto next = std::dynamic_pointer_cast<SceneTitle>(Change());
			m_scnMgr.Change(next);
			m_scnMgr.OptionClose();
			return;
		}
		else
		{
			isReturnNormal = true;
		}
	}
	if (input.IsTriggerd(0, Command::BTN_CANCEL))
	{
		isReturnNormal = true;
	}
	if (isReturnNormal)
	{
		sndSys.PlaySe(m_files[FileIndex::SE_CANCEL]->GetHandle(), true);
		m_updateInputFunc = &SceneOption::UpdateInputNormal;
		m_isOpenReturnCheck = false;
		auto& info = m_selectFrameInfo;
		StartSelectFrameMove(SELECT_FRAME_DRAW_LEFT, SELECT_FRAME_DRAW_RIGHT, FRAME_DRAW_Y + INTERVAL_DRAW_Y * m_indexNormal + 35);
		return;
	}
	bool isMove = false;
	if (input.IsTriggerd(0, Command::BTN_RIGHT))
	{
		m_indexReturn = (m_indexReturn + 1) % 2;
		isMove = true;
	}
	if (input.IsTriggerd(0, Command::BTN_LEFT))
	{
		m_indexReturn = (m_indexReturn - 1 + 2) % 2;
		isMove = true;
	}
	if (isMove)
	{
		sndSys.PlaySe(m_files[FileIndex::SE_CURSOR_MOVE]->GetHandle(), true);
		int lX = SELECT_RETURN_FRAME_DRAW_LEFT + FRAME_RETURN_INTERVAL * m_indexReturn;
		int rX = SELECT_RETURN_FRAME_DRAW_RIGHT + FRAME_RETURN_INTERVAL * m_indexReturn;
		StartSelectFrameMove(lX, rX, FRAME_RETURN_DRAW_Y + 35);
	}
}

void SceneOption::DrawLevel(int level, int index) const
{
	int lv = static_cast<int>(level / (255 * 0.2f));
	int x1 = LEVEL_DRAW_X_1;
	int x2 = LEVEL_DRAW_X_2;
	int y1 = LEVEL_DRAW_Y_1 + INTERVAL_DRAW_Y * index;
	int y2 = LEVEL_DRAW_Y_2 + INTERVAL_DRAW_Y * index;

	for (int i = 0; i < lv; ++i)
	{
		DrawBox(x1, y1, x2, y2, 0x00ff00, true);
		x1 += LEVEL_ADD_DRAW_X;
		x2 += LEVEL_ADD_DRAW_X;
	}
	for (int i = lv; i < 5; ++i)
	{
		DrawBox(x1, y1, x2, y2, 0x080808, true);
		x1 += LEVEL_ADD_DRAW_X;
		x2 += LEVEL_ADD_DRAW_X;
	}
	x1 = LEVEL_DRAW_X_1;
	x2 = LEVEL_DRAW_X_2;
	for (int i = 0; i < 5; ++i)
	{
		DrawBoxAA(static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2), static_cast<float>(y2), 0xffffff, false, 2.0f);
		x1 += LEVEL_ADD_DRAW_X;
		x2 += LEVEL_ADD_DRAW_X;
	}
}

void SceneOption::StartSelectFrameMove(int lX, int rX, int y)
{
	auto& info = m_selectFrameInfo;

	info.preY = info.y;
	info.nextY = y;
	info.preRightX = info.rightX;
	info.nextRightX = rX;
	info.preLeftX = info.leftX;
	info.nextLeftX = lX;
	info.rate = 0.0f;
	info.isMove = true;
}
