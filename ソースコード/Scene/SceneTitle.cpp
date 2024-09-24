#include "SceneTitle.h"
#include <DxLib.h>
#include <cmath>
#include <iomanip>
#include <sstream>
#include "SceneManager.h"
#include "SceneMain.h"
#include "Common/Input.h"
#include "Common/Application.h"
#include "Common/Game.h"
#include "Common/CommandList.h"
#include "Common/SoundSystem.h"
#include "Geometry/Quaternion.h"
#include "Geometry/Easing.h"
#include "File/FileID.h"
#include "File/FileManager.h"
#include "File/FileBase.h"
#include "File/FontSystem.h"
#include "File/RankingSystem.h"

namespace
{
	struct ObjectData
	{
		Vec3 pos;
		Quaternion rot;
		Vec3 size;
	};

	const enum FileIndex : int
	{
		LOGO,
		FRAME,
		FRAME_LEFT,
		FRAME_RIGHT,
		FRAME_NUM,
		FRAME_NUM_SELECT,
		OPTIN_FRAME,
		TEX_CAR,
		BGM_TITLE = TEX_CAR + 4,
		SE_SELECT,
		SE_CANCEL,
		SE_CURSOR_MOVE,
		SE_RANKING_PUSH,
		SKY_DOME,
		ARCH,
		CAR,
		ROAD,
		GROUND,
		STAND,
	};

	const enum TitleSelectIndex : int
	{
		PLAY_SINGLE,
		PLAY_MULTI,
		OPTION,
		END,
		MAX
	};

	const std::wstring STRS[TitleSelectIndex::MAX] =
	{
		L"ひとりで",
		L"みんなで",
		L"ランキング",
		L"ゲームをおわる"
	};

	const Vec3 CAMERA_TARGET = Vec3(0, 15, 0);

	// フォントサイズ
	constexpr int BASE_FONT_SIZE = 48;
	constexpr int SMALL_FONT_SIZE = 16;
	// ベースとなる描画関係
	constexpr int BASE_DRAW_X = 640;
	constexpr int BASE_DRAW_Y = 360;
	constexpr int INTERVAL_DRAW_Y = 90;
	// 選択フレームの描画関係
	constexpr int SELECT_FRAME_DRAW_RIGHT = 940;
	constexpr int SELECT_FRAME_DRAW_LEFT  = 340;
	constexpr int SELECT_FRAME_SWING_WIDTH = 20;
	constexpr float SELECT_FRAME_CYCLE_SPEED = 0.03f;
	constexpr float SELECT_FRAME_SLIDE_SPEED = 0.2f;
	// プレイ人数の描画関係
	constexpr int PLAY_NUM_FRAME_DRAW_X = BASE_DRAW_X + 100;
	constexpr int PLAY_NUM_FRAME_INTERVAL_DRAW_X = 60;
	constexpr float PLAY_NUM_FRAME_SLIDE_SPEED = 0.1f;
	// 文字の描画関係
	constexpr int STR_DRAW_Y = BASE_DRAW_Y - 16;
	constexpr int STR_DRAW_SLIDE_DIFF_X = 140;
	constexpr float STR_SLIDE_SPEED = 0.08f;
}

SceneTitle::SceneTitle() :
	SceneBase(Kind::TITLE),
	m_updateFunc(&SceneTitle::UpdateNormal),
	m_index(0),
	m_numIndex(0),
	m_selectFrameInfo{ SELECT_FRAME_DRAW_RIGHT, SELECT_FRAME_DRAW_LEFT, BASE_DRAW_Y },
	m_selectNumFrameInfo{ 0, PLAY_NUM_FRAME_DRAW_X },
	m_createFrame(0),
	m_createCount(0),
	m_isCreate(false),
	m_isRankingMode(false),
	m_isRankingAlpha(false),
	m_lightH(-1),
	m_rankingAlpha(0),
	m_isRankingEnd(false)
{
}

void SceneTitle::Init()
{
	// 画像読み込み
	auto& fileMgr = FileManager::GetInstance();
	m_files.push_back(fileMgr.Load(FileID::Img::LOGO));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_SELECT_LEFT));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_SELECT_RIGHT));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_NUM));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_SELECT_NUM));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_OPTION));
	m_files.push_back(fileMgr.Load(FileID::Img::TEX_CAR_RED));
	m_files.push_back(fileMgr.Load(FileID::Img::TEX_CAR_BULE));
	m_files.push_back(fileMgr.Load(FileID::Img::TEX_CAR_YELLOW));
	m_files.push_back(fileMgr.Load(FileID::Img::TEX_CAR_PURPLE));
	m_files.push_back(fileMgr.Load(FileID::Snd::BGM::TITLE));
	m_files.push_back(fileMgr.Load(FileID::Snd::SE::SELECT));
	m_files.push_back(fileMgr.Load(FileID::Snd::SE::CANCEL));
	m_files.push_back(fileMgr.Load(FileID::Snd::SE::CURSOR_MOVE));
	m_files.push_back(fileMgr.Load(FileID::Snd::SE::RANKING_PUSH));
	m_files.push_back(fileMgr.Load(FileID::Mdl::SKY_DOME));
	m_files.push_back(fileMgr.Load(FileID::Mdl::ARCH_GOAL));
	m_files.push_back(fileMgr.Load(FileID::Mdl::CAR));
	m_files.push_back(fileMgr.Load(FileID::Mdl::LOAD_STRAIGHT));
	m_files.push_back(fileMgr.Load(FileID::Mdl::GROUND));
	m_files.push_back(fileMgr.Load(FileID::Mdl::STAND));

	m_scnMgr.ChangeBgmH(m_files[FileIndex::BGM_TITLE]->GetHandle());

	// 文字列初期化
	for (int i = 0; i < STRS->size(); ++i)
	{
		auto& str = STRS[i];
		auto len = str.length();
		int x = BASE_DRAW_X - static_cast<int>(len * 0.4f * BASE_FONT_SIZE);
		int y = STR_DRAW_Y + INTERVAL_DRAW_Y * i;
		m_strDrawPos.push_back({str.c_str(), x, x, y, 0.0f});
	}

	m_cameraPos = Vec3(300, 100, 300);
	m_lightH = CreateDirLightHandle((CAMERA_TARGET - m_cameraPos).GetNormalized().VGet());
	SetCameraNearFar(Game::CAMERA_NEAR, Game::CAMERA_FAR);
	SetupCamera_Perspective(60.0f * Game::DEG_2_RAD);
	SetCameraPositionAndTarget_UpVecY(m_cameraPos.VGet(), CAMERA_TARGET.VGet());

	InitModelsPos();
	MV1SetTextureGraphHandle(m_files[FileIndex::SKY_DOME]->GetHandle(), 0, m_scnMgr.GetSkyDomeTexIndex(), false);
}

void SceneTitle::Update(bool isFade)
{
	(this->*m_updateFunc)();
	UpdateStr();
	UpdateSelectFrame();

	m_cameraPos = AngleAxis(0.75f, Vec3::Up()) * m_cameraPos;
	SetLightDirectionHandle(m_lightH, (CAMERA_TARGET - m_cameraPos).GetNormalized().VGet());
	SetCameraPositionAndTarget_UpVecY(m_cameraPos.VGet(), CAMERA_TARGET.VGet());
	for (auto& info : m_modelCarList)
	{
		info.pos += Vec3(0, 0, 1) * info.speed;
		if (info.pos.z > 2000.0f)
		{
			info.pos.z = -2000.0f;
			auto& app = Application::GetInstance();
			info.speed = app.GetRand(6.0f, 12.0f);
		}
		Matrix4x4 mat = info.baseMat * Pos(info.pos);
		MV1SetMatrix(info.handle, mat.GetMATRIX());
	}
}

void SceneTitle::Draw() const
{
#ifdef _DEBUG
	DrawString(8, 8, L"SceneTitle", 0xffffff);
#endif

	DrawModel();
	SetUseZBuffer3D(false);

	// タイトルロゴ
	DrawRotaGraph(BASE_DRAW_X + 32, 160, 1.0, 0.0, m_files[FileIndex::LOGO]->GetHandle(), true);
	// フレーム
	DrawRotaGraph(BASE_DRAW_X, BASE_DRAW_Y + INTERVAL_DRAW_Y * 0, 0.5, 0.0, m_files[FileIndex::FRAME]->GetHandle(), true);
	DrawRotaGraph(BASE_DRAW_X, BASE_DRAW_Y + INTERVAL_DRAW_Y * 1, 0.5, 0.0, m_files[FileIndex::FRAME]->GetHandle(), true);
	DrawRotaGraph(BASE_DRAW_X, BASE_DRAW_Y + INTERVAL_DRAW_Y * 2, 0.5, 0.0, m_files[FileIndex::FRAME]->GetHandle(), true);
	DrawRotaGraph(BASE_DRAW_X, BASE_DRAW_Y + INTERVAL_DRAW_Y * 3, 0.5, 0.0, m_files[FileIndex::FRAME]->GetHandle(), true);

	// 選択フレーム
	int xL = m_selectFrameInfo.leftX;
	int xR = m_selectFrameInfo.rightX;
	int y  = m_selectFrameInfo.y;
	DrawRotaGraph(xL, y, 0.5, 0.0, m_files[FileIndex::FRAME_LEFT]->GetHandle(), true);
	DrawRotaGraph(xR, y, 0.5, 0.0, m_files[FileIndex::FRAME_RIGHT]->GetHandle(), true);

	// プレイのみのフレーム
	if (m_index < TitleSelectIndex::OPTION)
	{
		int y = BASE_DRAW_Y + INTERVAL_DRAW_Y * m_index;

		auto& info = m_strDrawPos[m_index];
		int alpha = EaseIn(0, 255, info.rate);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

		if (m_index == TitleSelectIndex::PLAY_SINGLE)
		{
			DrawPlayNumFrame(PLAY_NUM_FRAME_DRAW_X, y, 1);
		}
		else
		{
			DrawPlayNumFrame(PLAY_NUM_FRAME_DRAW_X, y, 2);
			DrawPlayNumFrame(PLAY_NUM_FRAME_DRAW_X + PLAY_NUM_FRAME_INTERVAL_DRAW_X, y, 3);
			DrawPlayNumFrame(PLAY_NUM_FRAME_DRAW_X + PLAY_NUM_FRAME_INTERVAL_DRAW_X * 2, y, 4);
		}
		DrawRotaGraph(m_selectNumFrameInfo.x, y, 0.5, 0.0, m_files[FileIndex::FRAME_NUM_SELECT]->GetHandle(), true);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	// 文字
	auto& fontSys = FontSystem::GetInstance();
	auto fontHandle = fontSys.GetHandle(BASE_FONT_SIZE);
	for (auto& info : m_strDrawPos)
	{
		DrawStringToHandle(info.x, info.y, info.str, 0x000000, fontHandle);
	}

	DrawRanking();

#ifdef _DEBUG
	// 中心線
	DrawLine(640, 0, 640, 720, 0xff0000);
#endif

	SetUseZBuffer3D(true);
}

void SceneTitle::End()
{
	DeleteLightHandle(m_lightH);
	for (auto& h : m_entityModelHnaldeList)
	{
		MV1DeleteModel(h);
	}
}

void SceneTitle::InitModelsPos()
{
	LoadAndCreateObjects(L"Data/Stage/Other/Road.loc", FileIndex::ROAD);
	LoadAndCreateObjects(L"Data/Stage/Other/Ground.loc", FileIndex::GROUND);
	LoadAndCreateObjects(L"Data/Stage/Other/Stand.loc", FileIndex::STAND);
	LoadAndCreateObject(FileIndex::ARCH, Vec3(), AngleAxis(180, Vec3::Up()), Vec3(0.01f, 0.01f, 0.01f));
	LoadAndCreateObject(FileIndex::SKY_DOME, Vec3(), Quaternion(0, 0, 0, 1), Vec3(10, 10, 10));
	LoadAndCreateCar(0);
	LoadAndCreateCar(1);
	LoadAndCreateCar(2);
	LoadAndCreateCar(3);
}

void SceneTitle::UpdateNormal()
{
	UpdateInput();
	UpdateSelectNumFrame();
}

void SceneTitle::UpdateRanking()
{
	auto& input = Input::GetInstance();
	if (input.IsTriggerd(0, Command::BTN_BACK) || input.IsTriggerd(0, Command::BTN_OK))
	{
		auto& sndSys = SoundSystem::GetInstance();
		sndSys.PlaySe(m_files[FileIndex::SE_CANCEL]->GetHandle());
		m_isRankingAlpha = true;
		m_isRankingEnd = true;
	}

	if (m_isRankingEnd)
	{
		m_rankingAlpha -= 12;
		if (m_rankingAlpha < 0)
		{
			m_rankingAlpha = 0;
			m_updateFunc = &SceneTitle::UpdateNormal;
			m_isRankingMode = false;
		}
		return;
	}
	if (m_isRankingAlpha)
	{
		m_rankingAlpha += 12;
		if (m_rankingAlpha > 255)
		{
			m_isRankingAlpha = false;
			m_rankingAlpha = 255;
		}
		return;
	}

	for (auto& item : m_rankingInfo)
	{
		if (!item.isMove) continue;

		item.rate += 0.0625f;
		if (item.rate > 1.0f)
		{
			item.rate = 1.0f;
		}
	}
	if (m_isCreate) return;

	--m_createFrame;
	if (m_createFrame < 0)
	{
		auto& sndSys = SoundSystem::GetInstance();
		sndSys.PlaySe(m_files[FileIndex::SE_RANKING_PUSH]->GetHandle());
		RankingInfo info;
		auto& rankSys = RankingSystem::GetInstance();
		info.time = rankSys.GetTime(9 - m_createCount);
		info.y = 612 - m_createCount * 48;
		info.rate = 0.0f;
		info.isMove = true;
		m_rankingInfo.push_back(info);

		m_createFrame = 20;
		++m_createCount;
		if (m_createCount > 9)
		{
			m_isCreate = true;
		}
	}
}

void SceneTitle::UpdateInput()
{
	auto& input = Input::GetInstance();
	auto& sndSys = SoundSystem::GetInstance();
	// 上
	if (input.IsTriggerd(0, Command::BTN_UP))
	{
		sndSys.PlaySe(m_files[FileIndex::SE_CURSOR_MOVE]->GetHandle());
		m_index = (m_index - 1 + TitleSelectIndex::MAX) % TitleSelectIndex::MAX;
		InitSelectNumFrame();
		StartSelectFrameMove();
	}
	// 下
	if (input.IsTriggerd(0, Command::BTN_DOWN))
	{
		sndSys.PlaySe(m_files[FileIndex::SE_CURSOR_MOVE]->GetHandle());
		m_index = (m_index + 1) % TitleSelectIndex::MAX;
		InitSelectNumFrame();
		StartSelectFrameMove();
	}
	// マルチプレイの時
	if (m_index == TitleSelectIndex::PLAY_MULTI)
	{
		auto& info = m_selectNumFrameInfo;
		int size = Game::MAX_PLAYER_NUM - 1;
		if (input.IsTriggerd(0, Command::BTN_LEFT))
		{
			sndSys.PlaySe(m_files[FileIndex::SE_CURSOR_MOVE]->GetHandle());
			info.index = (info.index - 1 + size) % size;
			StartSelectNumFrameMove();
		}
		if (input.IsTriggerd(0, Command::BTN_RIGHT))
		{
			sndSys.PlaySe(m_files[FileIndex::SE_CURSOR_MOVE]->GetHandle());
			info.index = (info.index + 1) % size;
			StartSelectNumFrameMove();
		}
	}
	// 決定
	if (input.IsTriggerd(0, Command::BTN_OK))
	{
		sndSys.PlaySe(m_files[FileIndex::SE_SELECT]->GetHandle());

		if (m_index == TitleSelectIndex::PLAY_SINGLE)
		{
			m_nextSceneKind = Kind::MAIN;
			auto next = std::dynamic_pointer_cast<SceneMain>(Change());
			next->InitPlayerNum(1);
			m_scnMgr.Change(next);
		}
		else if (m_index == TitleSelectIndex::PLAY_MULTI)
		{
			m_nextSceneKind = Kind::MAIN;
			auto next = std::dynamic_pointer_cast<SceneMain>(Change());
			next->InitPlayerNum(m_selectNumFrameInfo.index + 2);
			m_scnMgr.Change(next);
		}
		else if (m_index == TitleSelectIndex::OPTION)
		{
			m_updateFunc = &SceneTitle::UpdateRanking;
			m_rankingInfo.clear();
			m_isRankingMode = true;
			m_isRankingAlpha = true;
			m_rankingAlpha = 0;
			m_createCount = 0;
			m_createFrame = 0;
			m_isCreate = false;
			m_isRankingEnd = false;
		}
		else if (m_index == TitleSelectIndex::END)
		{
			auto& app = Application::GetInstance();
			app.End();
		}
	}
}

void SceneTitle::UpdateStr()
{
	for (int i = 0; i < TitleSelectIndex::OPTION; ++i)
	{
		auto& info = m_strDrawPos[i];
		
		if (m_index == i)
		{
			info.rate = std::min<float>(info.rate + STR_SLIDE_SPEED, 1.0f);
		}
		else if (info.rate > 0.0f)
		{
			info.rate = std::max<float>(info.rate - STR_SLIDE_SPEED, 0.0f);
		}
		info.x = EaseIn(info.initX, info.initX - STR_DRAW_SLIDE_DIFF_X, info.rate);
	}
}

void SceneTitle::UpdateSelectFrame()
{
	auto& info = m_selectFrameInfo;
	
	info.cycleCount += SELECT_FRAME_CYCLE_SPEED;
	float sin = std::abs(std::sin(info.cycleCount));
	int width = static_cast<int>(SELECT_FRAME_SWING_WIDTH * 0.5f);

	int rs = SELECT_FRAME_DRAW_RIGHT + width;
	int re = SELECT_FRAME_DRAW_RIGHT - width;
	info.rightX = EaseIn(rs, re, sin);
	int ls = SELECT_FRAME_DRAW_LEFT - width;
	int le = SELECT_FRAME_DRAW_LEFT + width;
	info.leftX  = EaseIn(ls, le, sin);

	if (info.isMove)
	{
		info.rate += SELECT_FRAME_SLIDE_SPEED;
		if (info.rate > 1.0f)
		{
			info.rate = 1.0f;
			info.isMove = false;
		}
		info.y = EaseInOut(info.preY, info.nextY, info.rate);
	}
}

void SceneTitle::UpdateSelectNumFrame()
{
	auto& info = m_selectNumFrameInfo;
	if (!info.isMove) return;

	info.rate += PLAY_NUM_FRAME_SLIDE_SPEED;
	if (info.rate > 1.0f)
	{
		info.rate = 1.0f;
		info.isMove = false;
	}
	info.x = EaseInOut(info.preX, info.nextX, info.rate);
}

void SceneTitle::InitSelectNumFrame()
{
	m_selectNumFrameInfo.x = PLAY_NUM_FRAME_DRAW_X;
	m_selectNumFrameInfo.index = 0;
}

void SceneTitle::StartSelectFrameMove()
{
	auto& info = m_selectFrameInfo;

	info.preY = info.y;
	info.nextY = BASE_DRAW_Y + INTERVAL_DRAW_Y * m_index;
	info.rate = 0.0f;
	info.isMove = true;
}

void SceneTitle::StartSelectNumFrameMove()
{
	auto& info = m_selectNumFrameInfo;

	info.preX = info.x;
	info.nextX = PLAY_NUM_FRAME_DRAW_X + PLAY_NUM_FRAME_INTERVAL_DRAW_X * info.index;
	info.rate = 0.0f;
	info.isMove = true;
}

void SceneTitle::DrawPlayNumFrame(int x, int y, int num) const
{
	auto& fontSys = FontSystem::GetInstance();
	auto baseHandle = fontSys.GetHandle(BASE_FONT_SIZE);
	auto smallHandle = fontSys.GetHandle(SMALL_FONT_SIZE);

	DrawRotaGraph(x, y, 0.5, 0.0, m_files[FileIndex::FRAME_NUM]->GetHandle(), true);
	DrawFormatStringToHandle(x - 26, y - 26, 0x000000, baseHandle, L"%d", num);
	DrawStringToHandle(x + 8, y + 4, L"人", 0x000000, smallHandle);
}

void SceneTitle::DrawRanking() const
{
	if (!m_isRankingMode) return;

	auto& fontSys = FontSystem::GetInstance();
	if (m_isRankingAlpha)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_rankingAlpha);
	}
	DrawRotaGraph(640, 360, 1.0, 0.0, m_files[FileIndex::OPTIN_FRAME]->GetHandle(), true);
	DrawStringToHandle(400, 100, L"タイムランキング", 0xffffff, fontSys.GetHandle(64));
//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int i = 10;
	int alpha = 255;
	if (m_isRankingAlpha)
	{
		alpha = m_rankingAlpha;
	}
	for (auto& item : m_rankingInfo)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(alpha * item.rate));
		int time = item.time;
		int min = time / 3600;
		time %= 3600;
		int sec = time / 60;
		time %= 60;
		int conma = static_cast<int>(time / 60.0f * 100);
		std::wostringstream ss;
		ss << std::setw(2) << i << L"位　タイム: ";
		ss << std::setfill(L'0') << std::setw(2) << min << L":";
		ss << std::setfill(L'0') << std::setw(2) << sec << L".";
		ss << std::setfill(L'0') << std::setw(2) << conma;
//		DrawFormatStringToHandle(400, item.y, 0xffffff, fontSys.GetHandle(32), L"%2d位　タイム: %2d:%2d.%2d", i, min, sec, conma);
		DrawStringToHandle(400, item.y, ss.str().c_str(), 0xffffff, fontSys.GetHandle(32));
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		--i;
	}
}

void SceneTitle::DrawModel() const
{
	for (auto& h : m_entityModelHnaldeList)
	{
		MV1DrawModel(h);
	}
}

void SceneTitle::LoadAndCreateObjects(const wchar_t* path, int fileIndex)
{
	SetUseASyncLoadFlag(false);

	int handle = FileRead_open(path);

	if (handle != 0)
	{
		int size;
		FileRead_read(&size, sizeof(int), handle);

		for (int i = 0; i < size; ++i)
		{
			Vec3 pos;
			Quaternion rot;
			Vec3 size;

			FileRead_read(&pos, sizeof(pos), handle);
			FileRead_read(&rot, sizeof(rot), handle);
			FileRead_read(&size, sizeof(size), handle);

			int h = MV1DuplicateModel(m_files[fileIndex]->GetHandle());
			Matrix4x4 mat = Scale(size) * rot.GetMat() * Pos(pos);
			MV1SetMatrix(h, mat.GetMATRIX());

			m_entityModelHnaldeList.emplace_back(h);
		}

		FileRead_close(handle);
	}
	else
	{
		assert(false && "道データのファイルパスが違います");
	}

	SetUseASyncLoadFlag(true);
}

void SceneTitle::LoadAndCreateObject(int fileIndex, const Vec3& pos, const Quaternion& rot, const Vec3& scale)
{
	int h = MV1DuplicateModel(m_files[fileIndex]->GetHandle());
	Matrix4x4 mat = Scale(scale) * rot.GetMat() * Pos(pos);
	MV1SetMatrix(h, mat.GetMATRIX());

	m_entityModelHnaldeList.emplace_back(h);
}

void SceneTitle::LoadAndCreateCar(int no)
{
	CarData data;
	data.handle = MV1DuplicateModel(m_files[FileIndex::CAR]->GetHandle());

	int tex = m_files[FileIndex::TEX_CAR + no]->GetHandle();
	int texIdx = MV1GetMaterialDifMapTexture(data.handle, 0);
	MV1SetTextureGraphHandle(data.handle, texIdx, tex, false);

	auto& app = Application::GetInstance();
	data.pos = Vec3(-112.5f + no * 75, 0, app.GetRand(-1200.0f, -300.0f));
	data.baseMat = Scale(Vec3(0.01f, 0.01f, 0.01f)) * AngleAxis(180, Vec3::Up()).GetMat();

	Matrix4x4 mat = data.baseMat * Pos(data.pos);
	MV1SetMatrix(data.handle, mat.GetMATRIX());

	data.speed = app.GetRand(6.0f, 12.0f);

	m_modelCarList.emplace_back(data);
	m_entityModelHnaldeList.emplace_back(data.handle);
}
