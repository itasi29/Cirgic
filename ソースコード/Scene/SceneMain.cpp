#include "SceneMain.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <iomanip>
#include <sstream>
#include "SceneResult.h"
#include "SceneManager.h"
#include "Common/Game.h"
#include "Common/Camera.h"
#include "Common/SoundSystem.h"
#include "File/FileManager.h"
#include "File/FileBase.h"
#include "File/FontSystem.h"
#include "File/FileID.h"
#include "Race/RaceManager.h"
#include "Object/ObjectManager.h"
#include "Object/Car/CarManager.h"
#include "Object/Item/ItemKinokoTriple.h"
#include "Geometry/Easing.h"

namespace
{
	const enum FileIndex : int
	{
		COUNT_DOWN_3,
		COUNT_DOWN_2,
		COUNT_DOWN_1,
		COUNT_DOWN_GO,
		COUNT_DOWN_FINISH,
		COUNT_DOWN_CURTAIN,
		COIN,
		RAP,
		BACK_FRAME,
		STANDING_1,
		STANDING_2,
		STANDING_3,
		STANDING_4,
		FRAME_ITEM,
		ITEM_KINOKO,
		ITEM_KINOKO_TRIPLE,
		ITEM_CARAPACE_GREEN = ITEM_KINOKO_TRIPLE + 3,
		ITEM_CARAPACE_RED,
		ITEM_BANANA,
		MINI_MAP,
		MAP_PLAYER,
		SKY_DOME = MAP_PLAYER + 4,
		BGM_MAIN,
		SE_COUNT_DOWN,
		SE_GOAL,
	};

	constexpr int COUNT_DOWN_FRAME = 60;
	constexpr int ITEM_BLINK_TIME = 60;
	constexpr int BACK_SCREEN_CHANGE_FRAME = 60 * 25;
	constexpr int FNISH_COUNT = 180;
	constexpr int FONT_SIZE = 32;
	constexpr int DRAW_TIME_X = 1080;
	constexpr int DRAW_TIME_Y = 32;
	constexpr int DRAW_TIME_IMG_ADD_X_START = -20;
	constexpr int DRAW_TIME_IMG_ADD_Y_START = -16;
	constexpr int DRAW_TIME_IMG_ADD_X_END = 192;
	constexpr int DRAW_TIME_IMG_Y_END = 80;
	constexpr int DRAW_RAP_UPDATE_FLUSH_FRAME = 80;
	constexpr int DRAW_RAP_UPDATE_FLUSH_INTERVAL = 20;
	// ミニマップの描画情報
	constexpr int DRAW_MINMAP_LEFT_X = 1140;
	constexpr int DRAW_MINMAP_LEFT_Y = 580;
	constexpr int DRAW_MINMAP_CENTER_X = 640;
	constexpr int DRAW_MINMAP_CENTER_Y = 360;
	// 1人プレイ時の描画情報
	constexpr int DRAW_PALYER_1_FRAME_X_START = 16;
	constexpr int DRAW_PALYER_1_FRAME_Y_START = 600;
	constexpr int DRAW_PALYER_1_FRAME_X_END = 196;
	constexpr int DRAW_PALYER_1_FRAME_Y_END = 700;
	constexpr int DRAW_PLAYER_1_IMG_X = 64;
	constexpr int DRAW_PLAYER_1_IMG_Y = 650;
	constexpr double DRAW_PALYER_1_IMG_SIZE = 0.25;
	constexpr int DRAW_PLAYER_1_STR_X = 128;
	constexpr int DRAW_PLAYER_1_STR_Y = 630;
	constexpr int DRAW_PLAYER_1_INTERVAL = 192;
	constexpr int DRAW_PLAYER_1_CAR_UI_X = 96;
	constexpr int DRAW_PLAYER_1_CAR_UI_Y = 96;
	constexpr int DRAW_RAP_UPDATE_X = 1080;
	constexpr int DRAW_RAP_UPDATE_Y = 96;
	constexpr int DRAW_RAP_UPDATE_INTERVAL = 64;
	// 2人プレイ時の描画情報
	constexpr int DRAW_PALYER_2_FRAME_X_START = 16;
	constexpr int DRAW_PALYER_2_FRAME_Y_START = 600;
	constexpr int DRAW_PALYER_2_FRAME_X_END = 160;
	constexpr int DRAW_PALYER_2_FRAME_Y_END = 700;
	constexpr int DRAW_PLAYER_2_IMG_X = 64;
	constexpr int DRAW_PLAYER_2_IMG_Y = 650;
	constexpr double DRAW_PALYER_2_IMG_SIZE = 0.1875;
	constexpr int DRAW_PLAYER_2_STR_X = 108;
	constexpr int DRAW_PLAYER_2_STR_Y = 630;
	constexpr int DRAW_PLAYER_2_INTERVAL = 180;
	constexpr int DRAW_PLAYER_2_CAR_UI_X = 96;
	constexpr int DRAW_PLAYER_2_CAR_UI_Y = 96;
	constexpr double DRAW_PALYER_2_CAR_UI_SIZE = 0.375;
	constexpr int DRAW_PLAYER_2_STANDING_X = 560;
	constexpr int DRAW_PLAYER_2_STANDING_Y = 620;
	constexpr double DRAW_PLAYER_2_STANDING_SIZE = 0.375;
	constexpr int DRAW_PLAYER_2_GOAL_CURTAIN_X_START = 0;
	constexpr int DRAW_PLAYER_2_GOAL_CURTAIN_Y_START = 270;
	constexpr int DRAW_PLAYER_2_GOAL_CURTAIN_X_END = 640;
	constexpr int DRAW_PLAYER_2_GOAL_CURTAIN_Y_END = 450;
	constexpr int DRAW_PLAYER_2_GOAL_IMG_X = 320;
	constexpr int DRAW_PLAYER_2_GOAL_IMG_Y = 360;
	constexpr double DRAW_PLAYER_2_GOAL_IMG_SIZE = 0.5;
	constexpr int DRAW_PLAYER_2_ADD = 640;
	// 3、4人プレイ時の描画情報
	constexpr int DRAW_TIME_X_PLAYER_OVER = 550;
	constexpr int DRAW_PALYER_OVER_FRAME_X_START = 8;
	constexpr int DRAW_PALYER_OVER_FRAME_Y_START = 280;
	constexpr int DRAW_PALYER_OVER_FRAME_X_END = 128;
	constexpr int DRAW_PALYER_OVER_FRAME_Y_END = 340;
	constexpr int DRAW_PLAYER_OVER_IMG_X = 40;
	constexpr int DRAW_PLAYER_OVER_IMG_Y = 308;
	constexpr double DRAW_PALYER_OVER_IMG_SIZE = 0.125;
	constexpr int DRAW_PLAYER_OVER_STR_X = 80;
	constexpr int DRAW_PLAYER_OVER_STR_Y = 294;
	constexpr int DRAW_PLAYER_OVER_INTERVAL = 152;
	constexpr int DRAW_PLAYER_OVER_CAR_UI_X = 72;
	constexpr int DRAW_PLAYER_OVER_CAR_UI_Y = 72;
	constexpr double DRAW_PALYER_OVER_CAR_UI_SIZE = 0.25;
	constexpr int DRAW_PLAYER_OVER_STANDING_X = 80;
	constexpr int DRAW_PLAYER_OVER_STANDING_Y = 220;
	constexpr double DRAW_PLAYER_OVER_STANDING_SIZE = 0.25;
	constexpr int DRAW_PLAYER_OVER_GOAL_CURTAIN_X_START = 0;
	constexpr int DRAW_PLAYER_OVER_GOAL_CURTAIN_Y_START = 90;
	constexpr int DRAW_PLAYER_OVER_GOAL_CURTAIN_X_END = 640;
	constexpr int DRAW_PLAYER_OVER_GOAL_CURTAIN_Y_END = 270;
	constexpr int DRAW_PLAYER_OVER_GOAL_IMG_X = 320;
	constexpr int DRAW_PLAYER_OVER_GOAL_IMG_Y = 180;
	constexpr double DRAW_PLAYER_OVER_GOAL_IMG_SIZE = 0.5;
	constexpr int DRAW_PLAYER_OVER_ADD_X = 960;
	constexpr int DRAW_PLAYER_OVER_ADD_Y = 360;
	constexpr int DRAW_PLAYER_OVER_ADD_CAR_UI_X = 1150;
	constexpr int DRAW_PLAYER_OVER_ADD_STANDING_X = 1128;
	constexpr int DRAW_PLAYER_OVER_ADD_GOAL_X = 640;
}

SceneMain::SceneMain() :
	SceneBase(Kind::MAIN),
	m_playerNum(1),
	m_screenType(ScreenType::PLAYER_1),
	m_updateFunc(&SceneMain::CountDownUpdate),
	m_drawFunc(&SceneMain::CountDownDraw),
	m_isPlaySeCountDown(true),
	m_countDown(COUNT_DOWN_FRAME),
	m_countIndex(0),
	m_isDrawStartCurtain(true),
	m_finishCount(0),
	m_changeFrame(0),
	m_carNo(0),
	m_bgmVol(0),
	m_bgmVolRate(0),
	m_time(0)
{
}

void SceneMain::Init()
{
	LoadFiles();

	m_scnMgr.ChangeBgmH(m_files[FileIndex::BGM_MAIN]->GetHandle());
	auto& sndSys = SoundSystem::GetInstance();
	m_bgmVol = sndSys.GetBgmVol();
	m_bgmVolRate = sndSys.GetBgmVolRate() * 0.4f;
	sndSys.ChangeBgmVol(static_cast<int>(-m_bgmVol * m_bgmVolRate), false);
	
	auto skyH = m_files[FileIndex::SKY_DOME]->GetHandle();
	MV1SetScale(skyH, VGet(10, 10, 10));
	MV1SetScale(skyH, VGet(15, 15, 15));

	m_raceMgr = std::make_shared<RaceManager>();
	m_objMgr = std::make_shared<ObjectManager>();
	m_objMgr->Init(m_raceMgr, m_playerNum);
	m_raceMgr->Init(m_objMgr->GetCpMgr(), m_objMgr->GetCarMgr(), m_playerNum);

	m_scnMgr.ChangeSkyDomeTexIndex();
	MV1SetTextureGraphHandle(m_files[FileIndex::SKY_DOME]->GetHandle(), 0, m_scnMgr.GetSkyDomeTexIndex	(), false);
}

void SceneMain::Update(bool isFade)
{
	if (isFade) return;

	(this->*m_updateFunc)();
	for (auto& data : m_rapTimeDatas)
	{
		if (!data.isUpdate) continue;

		++data.frame;
		if (data.frame > DRAW_RAP_UPDATE_FLUSH_FRAME)
		{
			data.isUpdate = false;
		}
	}
	UpdateEffekseer3D();
}

void SceneMain::Draw() const
{
#ifdef _DEBUG
	DrawString(8, 8, L"SceneMain", 0xffffff);
#endif
	(this->*m_drawFunc)();
}

void SceneMain::End()
{
	m_objMgr->End();
}

void SceneMain::InitPlayerNum(int num)
{
	m_playerNum = num;
	CreateScreen();
}

void SceneMain::CountDownUpdate()
{
	if (m_isPlaySeCountDown)
	{
		m_isPlaySeCountDown = false;
		auto& sndSys = SoundSystem::GetInstance();
		sndSys.PlaySe(m_files[FileIndex::SE_COUNT_DOWN]->GetHandle());
	}
	--m_countDown;
	if (m_countDown < 0)
	{
		m_countDown = COUNT_DOWN_FRAME;
		++m_countIndex;
		if (m_countIndex >= 3)
		{
			m_updateFunc = &SceneMain::NormalUpdate;
			m_drawFunc = &SceneMain::NormalDraw;
		}
	}
}

void SceneMain::NormalUpdate()
{
	if (m_isDrawStartCurtain)
	{
		--m_countDown;
		if (m_countDown < 0)
		{
			m_countDown = COUNT_DOWN_FRAME;
			++m_countIndex;
			m_isDrawStartCurtain = false;
		}
		else
		{
			m_bgmVolRate = 0.4f / COUNT_DOWN_FRAME;
			auto& sndSys = SoundSystem::GetInstance();
			int add = static_cast<int>(m_bgmVol * m_bgmVolRate);
			sndSys.ChangeBgmVol(add);
		}
	}
	for (int i = 0; i < 3; ++i)
	{
		auto& info = m_rapTimeDatas[i];
		if (info.isDraw) continue;

		if (m_raceMgr->IsRapUpdate(0))
		{
			info.isDraw = true;
			info.isUpdate = true;
			info.frame = 0;
			int ans = 0;
			for (int j = 0; j < i; ++j)
			{
				ans += m_rapTimeDatas[j].rapTime;
			}
			info.rapTime = m_time - ans;
			break;
		}
	}
	++m_time;

	m_objMgr->Update();
	m_raceMgr->Update();
	for (int i = 0; i < m_playerNum; ++i)
	{
		auto& data = m_itemUIDatas[i];
		bool isPreDraw = data.isDraw;
		data.isDraw = m_raceMgr->HasItem(i);
		
		if (!data.isDraw) continue;

		if (!isPreDraw)
		{
			data.isBlink = true;
			data.count = 0;
		}
		if (data.isBlink)
		{
			++data.count;
			if (data.count > ITEM_BLINK_TIME)
			{
				data.isBlink = false;
			}
		}
	}

	--m_changeFrame;
	if (m_changeFrame < 0)
	{
		m_changeFrame = BACK_SCREEN_CHANGE_FRAME;
		m_carNo = (m_carNo + 1) % 3;
	}

	for (int i = 0; i < m_playerNum; ++i)
	{
		if (!m_raceMgr->IsGoal(i)) return;
	}
	//auto& sndSys = SoundSystem::GetInstance();
	//sndSys.PlaySe(m_files[FileIndex::SE_GOAL]->GetHandle());
	m_raceMgr->CarAllGaol();
	m_updateFunc = &SceneMain::FinishUpdate;
	m_drawFunc = &SceneMain::FinishDraw;
	m_finishCount = FNISH_COUNT;
}

void SceneMain::FinishUpdate()
{
	--m_finishCount;
	if (m_finishCount > 0) return;

	m_nextSceneKind = Kind::RESULT;
	auto next = std::dynamic_pointer_cast<SceneResult>(Change());
	next->Init(m_playerNum, m_raceMgr->GetData());
	m_scnMgr.Change(next);
}

void SceneMain::CountDownDraw() const
{
	ScreenDraw();

	CarDrawUI();
	CountDownDrawUI(false);
}

void SceneMain::NormalDraw() const
{
	ScreenDraw();

	CarDrawUI();
	if (m_isDrawStartCurtain)
	{
		CountDownDrawUI(true);
	}
}

void SceneMain::FinishDraw() const
{
	ScreenDraw();
	CountDownDraw();
}

void SceneMain::ScreenDraw() const
{
	auto skyH = m_files[FileIndex::SKY_DOME]->GetHandle();
	auto& camera = Camera::GetInstance();

	if (m_screenType == ScreenType::PLAYER_1)
	{
		m_objMgr->Draw(0);

		MV1SetPosition(skyH, camera.GetInfo(0).targetPos.VGet());
		MV1DrawModel(skyH);

		// 3Dの情報をDxLibとEffekseerで合わせる
		Effekseer_Sync3DSetting();
		DrawEffekseer3D();
	}
	else
	{
		for (int i = 0; i < m_playerNum; ++i)
		{
			SetDrawScreen(m_screenH[i]);
			ClearDrawScreen();
			m_objMgr->Draw(i);

			MV1SetPosition(skyH, camera.GetInfo(i).targetPos.VGet());
			MV1DrawModel(skyH);

			// 3Dの情報をDxLibとEffekseerで合わせる
			Effekseer_Sync3DSetting();
			DrawEffekseer3D();
		}
		SetDrawScreen(DX_SCREEN_BACK);
		if (m_screenType == ScreenType::PLAYER_3)
		{
			DrawRotaGraph(static_cast<int>(Game::WINDOW_W * 0.5f), static_cast<int>(Game::WINDOW_H * 0.5f), 2.0, 0.0, m_screenH[m_carNo], true);
		}

		int hX = static_cast<int>(Game::WINDOW_W * 0.5f);
		DrawGraph(0, 0, m_screenH[0], true);
		DrawGraph(hX, 0, m_screenH[1], true);

		if (m_screenType != ScreenType::PLAYER_2)
		{
			int hY = static_cast<int>(Game::WINDOW_H * 0.5f);
			DrawGraph(0, hY, m_screenH[2], true);
			if (m_screenType == ScreenType::PLAYER_4)
			{
				DrawGraph(hX, hY, m_screenH[3], true);
			}
			DrawLineAA(0, Game::WINDOW_H * 0.5f, Game::WINDOW_W, Game::WINDOW_H * 0.5f, 0x000000, 3.0f);
		}
		DrawLineAA(Game::WINDOW_W * 0.5f, 0, Game::WINDOW_W * 0.5f, Game::WINDOW_H, 0x000000, 3.0f);
	}
}

void SceneMain::LoadFiles()
{
	auto& fileMgr = FileManager::GetInstance();
	m_files.push_back(fileMgr.Load(FileID::Img::COUNT_DOWN_3));
	m_files.push_back(fileMgr.Load(FileID::Img::COUNT_DOWN_2));
	m_files.push_back(fileMgr.Load(FileID::Img::COUNT_DOWN_1));
	m_files.push_back(fileMgr.Load(FileID::Img::COUNT_DOWN_GO));
	m_files.push_back(fileMgr.Load(FileID::Img::COUNT_DOWN_FINISH));
	m_files.push_back(fileMgr.Load(FileID::Img::COUNT_DOWN_CURTAIN));
	m_files.push_back(fileMgr.Load(FileID::Img::COIN));
	m_files.push_back(fileMgr.Load(FileID::Img::RAP));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_BACK));
	m_files.push_back(fileMgr.Load(FileID::Img::STANDING_1));
	m_files.push_back(fileMgr.Load(FileID::Img::STANDING_2));
	m_files.push_back(fileMgr.Load(FileID::Img::STANDING_3));
	m_files.push_back(fileMgr.Load(FileID::Img::STANDING_4));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_ITEM));
	m_files.push_back(fileMgr.Load(FileID::Img::ITEM_KINOKO));
	m_files.push_back(fileMgr.Load(FileID::Img::ITEM_KINOKO_TIRPLE_1));
	m_files.push_back(fileMgr.Load(FileID::Img::ITEM_KINOKO_TIRPLE_2));
	m_files.push_back(fileMgr.Load(FileID::Img::ITEM_KINOKO_TIRPLE_3));
	m_files.push_back(fileMgr.Load(FileID::Img::ITEM_CARAPACE_GREEN));
	m_files.push_back(fileMgr.Load(FileID::Img::ITEM_CARAPACE_RED));
	m_files.push_back(fileMgr.Load(FileID::Img::ITEM_BANANA));
	m_files.push_back(fileMgr.Load(FileID::Img::MINI_MAP));
	m_files.push_back(fileMgr.Load(FileID::Img::MAP_PLAYER_1));
	m_files.push_back(fileMgr.Load(FileID::Img::MAP_PLAYER_2));
	m_files.push_back(fileMgr.Load(FileID::Img::MAP_PLAYER_3));
	m_files.push_back(fileMgr.Load(FileID::Img::MAP_PLAYER_4));
	m_files.push_back(fileMgr.Load(FileID::Mdl::SKY_DOME));
	m_files.push_back(fileMgr.Load(FileID::Snd::BGM::MAIN));
	m_files.push_back(fileMgr.Load(FileID::Snd::SE::COUNT_DOWN));
	m_files.push_back(fileMgr.Load(FileID::Snd::SE::GOAL));
}

void SceneMain::CreateScreen()
{
	if (m_playerNum == 1)
	{
		m_screenType = ScreenType::PLAYER_1;
	}
	else if (m_playerNum == 2)
	{
		m_screenH.push_back(MakeScreen(static_cast<int>(Game::WINDOW_W * 0.5f), Game::WINDOW_H, true));
		m_screenH.push_back(MakeScreen(static_cast<int>(Game::WINDOW_W * 0.5f), Game::WINDOW_H, true));
		m_screenType = ScreenType::PLAYER_2;
	}
	else
	{
		for (int i = 0; i < m_playerNum; ++i)
		{
			m_screenH.push_back(MakeScreen(static_cast<int>(Game::WINDOW_W * 0.5f), static_cast<int>(Game::WINDOW_H * 0.5f), true));
		}

		if (m_playerNum == 3)
		{
			m_screenType = ScreenType::PLAYER_3;
		}
		else
		{
			m_screenType = ScreenType::PLAYER_4;
		}
	}
}

void SceneMain::CountDownDrawUI(bool isCurtainAlpha) const
{
	SetWriteZBuffer3D(false);

	auto countH = m_files[m_countIndex]->GetHandle();
	auto curtainH = m_files[FileIndex::COUNT_DOWN_CURTAIN]->GetHandle();

	int centerX = static_cast<int>(Game::WINDOW_W * 0.5f);
	int centerY = static_cast<int>(Game::WINDOW_H * 0.5f);

	if (!isCurtainAlpha)
	{
		DrawRotaGraph(centerX, centerY, 1.0, 0.0, curtainH, true);
	}

	float t = m_countDown / static_cast<float>(COUNT_DOWN_FRAME);
	int alpha = EaseOut(0, 255, t, 8.0f);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	if (isCurtainAlpha)
	{
		DrawRotaGraph(centerX, centerY, 1.0, 0.0, curtainH, true);
	}
	DrawRotaGraph(centerX, centerY, 1.0, 0.0, countH, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	SetWriteZBuffer3D(true);
}

void SceneMain::CarDrawUI() const
{
	auto& carMgr = m_objMgr->GetCarMgr();
	auto& font = FontSystem::GetInstance();

	int fontH = font.GetHandle(FONT_SIZE);
	
	int coinH = m_files[FileIndex::COIN]->GetHandle();
	int rapH = m_files[FileIndex::RAP]->GetHandle();
	int frameH = m_files[FileIndex::BACK_FRAME]->GetHandle();

	int drawTimeX = DRAW_TIME_X;

	if (m_screenType == ScreenType::PLAYER_1)
	{
		DrawExtendGraph(DRAW_PALYER_1_FRAME_X_START, DRAW_PALYER_1_FRAME_Y_START, DRAW_PALYER_1_FRAME_X_END, DRAW_PALYER_1_FRAME_Y_END, frameH, true);
		DrawRotaGraph(DRAW_PLAYER_1_IMG_X, DRAW_PLAYER_1_IMG_Y, DRAW_PALYER_1_IMG_SIZE, 0.0, coinH, true);
		DrawFormatStringToHandle(DRAW_PLAYER_1_STR_X, DRAW_PLAYER_1_STR_Y, 0xffffff, fontH, L"%2d", carMgr->GetCoinNum(0));

		DrawExtendGraph(DRAW_PALYER_1_FRAME_X_START + DRAW_PLAYER_1_INTERVAL, DRAW_PALYER_1_FRAME_Y_START, DRAW_PALYER_1_FRAME_X_END + DRAW_PLAYER_1_INTERVAL, DRAW_PALYER_1_FRAME_Y_END, frameH, true);
		DrawRotaGraph(DRAW_PLAYER_1_IMG_X + DRAW_PLAYER_1_INTERVAL, DRAW_PLAYER_1_IMG_Y, DRAW_PALYER_1_IMG_SIZE, 0.0, rapH, true);
		DrawFormatStringToHandle(DRAW_PLAYER_1_STR_X + DRAW_PLAYER_1_INTERVAL, DRAW_PLAYER_1_STR_Y, 0xffffff, fontH, L"%d", m_raceMgr->GetRap(0));

		DrawRotaGraph(DRAW_MINMAP_LEFT_X, DRAW_MINMAP_LEFT_Y, 1.0, 0.0, m_files[FileIndex::MINI_MAP]->GetHandle(), true);
		DrawMiniMap(0, DRAW_MINMAP_LEFT_X, DRAW_MINMAP_LEFT_Y);

		DrawCarItemUI(0, DRAW_PLAYER_1_CAR_UI_X, DRAW_PLAYER_1_CAR_UI_Y, 0.375);
		
		for (int i = 0; i < 3; ++i)
		{
			DrawRapTimeUpdate(i, DRAW_RAP_UPDATE_X, DRAW_RAP_UPDATE_Y + i * DRAW_RAP_UPDATE_INTERVAL);
		}
	}
	else if(m_screenType == ScreenType::PLAYER_2)
	{
		
		DrawRotaGraph(DRAW_MINMAP_CENTER_X, DRAW_MINMAP_CENTER_Y, 1.0, 0.0, m_files[FileIndex::MINI_MAP]->GetHandle(), true);
		for (int i = 0; i < m_playerNum; ++i)
		{
			int add = i * DRAW_PLAYER_2_ADD;

			DrawExtendGraph(DRAW_PALYER_2_FRAME_X_START + add, DRAW_PALYER_2_FRAME_Y_START, DRAW_PALYER_2_FRAME_X_END + add, DRAW_PALYER_2_FRAME_Y_END, frameH, true);
			DrawRotaGraph(DRAW_PLAYER_2_IMG_X + add, DRAW_PLAYER_2_IMG_Y, DRAW_PALYER_2_IMG_SIZE, 0.0, coinH, true);
			DrawFormatStringToHandle(DRAW_PLAYER_2_STR_X + add, DRAW_PLAYER_2_STR_Y, 0xffffff, fontH, L"%2d", carMgr->GetCoinNum(i));

			DrawExtendGraph(DRAW_PALYER_2_FRAME_X_START + DRAW_PLAYER_2_INTERVAL + add, DRAW_PALYER_2_FRAME_Y_START, DRAW_PALYER_2_FRAME_X_END + DRAW_PLAYER_2_INTERVAL + add, DRAW_PALYER_2_FRAME_Y_END, frameH, true);
			DrawRotaGraph(DRAW_PLAYER_2_IMG_X + DRAW_PLAYER_2_INTERVAL + add, DRAW_PLAYER_2_IMG_Y, DRAW_PALYER_2_IMG_SIZE, 0.0, rapH, true);
			DrawFormatStringToHandle(DRAW_PLAYER_2_STR_X + DRAW_PLAYER_2_INTERVAL + add, DRAW_PLAYER_2_STR_Y, 0xffffff, fontH, L"%d", m_raceMgr->GetRap(i));

			DrawMiniMap(i, DRAW_MINMAP_CENTER_X, DRAW_MINMAP_CENTER_Y);
			DrawCarItemUI(i, DRAW_PLAYER_2_CAR_UI_X + add, DRAW_PLAYER_2_CAR_UI_Y, DRAW_PALYER_2_CAR_UI_SIZE);

			int index = m_raceMgr->GetStanding(i) + FileIndex::STANDING_1 - 1;
			DrawRotaGraph(DRAW_PLAYER_2_STANDING_X + add, DRAW_PLAYER_2_STANDING_Y, DRAW_PLAYER_2_STANDING_SIZE, 0.0, m_files[index]->GetHandle(), true);

			if (m_raceMgr->IsGoal(i))
			{
				DrawExtendGraph(DRAW_PLAYER_2_GOAL_CURTAIN_X_START + add, DRAW_PLAYER_2_GOAL_CURTAIN_Y_START, DRAW_PLAYER_2_GOAL_CURTAIN_X_END + add, DRAW_PLAYER_2_GOAL_CURTAIN_Y_END, m_files[FileIndex::COUNT_DOWN_CURTAIN]->GetHandle(), true);
				DrawRotaGraph(DRAW_PLAYER_2_GOAL_IMG_X + add, DRAW_PLAYER_2_GOAL_IMG_Y, DRAW_PLAYER_2_GOAL_IMG_SIZE, 0.0, m_files[FileIndex::COUNT_DOWN_FINISH]->GetHandle(), true);
			}
		}
	}
	else
	{
		drawTimeX = DRAW_TIME_X_PLAYER_OVER;
		DrawRotaGraph(DRAW_MINMAP_CENTER_X, DRAW_MINMAP_CENTER_Y, 1.0, 0.0, m_files[FileIndex::MINI_MAP]->GetHandle(), true);
		for (int i = 0; i < m_playerNum; ++i)
		{
			int xRate = (i % 2);
			int yRate = (i / 2);
			int addX = xRate * DRAW_PLAYER_OVER_ADD_X;
			int addY = yRate * DRAW_PLAYER_OVER_ADD_Y;

			DrawExtendGraph(DRAW_PALYER_OVER_FRAME_X_START + addX, DRAW_PALYER_OVER_FRAME_Y_START + addY, DRAW_PALYER_OVER_FRAME_X_END + addX, DRAW_PALYER_OVER_FRAME_Y_END + addY, frameH, true);
			DrawRotaGraph(DRAW_PLAYER_OVER_IMG_X + addX, DRAW_PLAYER_OVER_IMG_Y + addY, DRAW_PALYER_OVER_IMG_SIZE, 0.0, coinH, true);
			DrawFormatStringToHandle(DRAW_PLAYER_OVER_STR_X + addX, DRAW_PLAYER_OVER_STR_Y + addY, 0xffffff, fontH, L"%2d", carMgr->GetCoinNum(i));

			DrawExtendGraph(DRAW_PALYER_OVER_FRAME_X_START + DRAW_PLAYER_OVER_INTERVAL + addX, DRAW_PALYER_OVER_FRAME_Y_START + addY, DRAW_PALYER_OVER_FRAME_X_END + DRAW_PLAYER_OVER_INTERVAL + addX, DRAW_PALYER_OVER_FRAME_Y_END + addY, frameH, true);
			DrawRotaGraph(DRAW_PLAYER_OVER_IMG_X + DRAW_PLAYER_OVER_INTERVAL + addX, DRAW_PLAYER_OVER_IMG_Y + addY, DRAW_PALYER_OVER_IMG_SIZE, 0.0, rapH, true);
			DrawFormatStringToHandle(DRAW_PLAYER_OVER_STR_X + DRAW_PLAYER_OVER_INTERVAL + addX, DRAW_PLAYER_OVER_STR_Y + addY, 0xffffff, fontH, L"%d", m_raceMgr->GetRap(i));

			DrawMiniMap(i, DRAW_MINMAP_CENTER_X, DRAW_MINMAP_CENTER_Y);
			addX = xRate * DRAW_PLAYER_OVER_ADD_CAR_UI_X;
			DrawCarItemUI(i, DRAW_PLAYER_OVER_CAR_UI_X + addX, DRAW_PLAYER_OVER_CAR_UI_Y + addY, DRAW_PALYER_OVER_CAR_UI_SIZE);

			addX = xRate * DRAW_PLAYER_OVER_ADD_STANDING_X;
			int index = m_raceMgr->GetStanding(i) + FileIndex::STANDING_1 - 1;
			DrawRotaGraph(DRAW_PLAYER_OVER_STANDING_X + addX, DRAW_PLAYER_OVER_STANDING_Y + addY, DRAW_PLAYER_OVER_STANDING_SIZE, 0.0, m_files[index]->GetHandle(), true);

			if (m_raceMgr->IsGoal(i))
			{
				addX = xRate * DRAW_PLAYER_OVER_ADD_GOAL_X;
				DrawExtendGraph(DRAW_PLAYER_OVER_GOAL_CURTAIN_X_START + addX, DRAW_PLAYER_OVER_GOAL_CURTAIN_Y_START + addY, DRAW_PLAYER_OVER_GOAL_CURTAIN_X_END + addX, DRAW_PLAYER_OVER_GOAL_CURTAIN_Y_END + addY, m_files[FileIndex::COUNT_DOWN_CURTAIN]->GetHandle(), true);
				DrawRotaGraph(DRAW_PLAYER_OVER_GOAL_IMG_X + addX, DRAW_PLAYER_OVER_GOAL_IMG_Y + addY, DRAW_PLAYER_OVER_GOAL_IMG_SIZE, 0.0, m_files[FileIndex::COUNT_DOWN_FINISH]->GetHandle(), true);
			}
		}
	}

	int time = m_time;
	int min = time / 3600;
	time %= 3600;
	int sec = time / 60;
	time %= 60;
	int conma = static_cast<int>(time / 60.0f * 100);
	DrawExtendGraph(drawTimeX + DRAW_TIME_IMG_ADD_X_START, DRAW_TIME_Y + DRAW_TIME_IMG_ADD_Y_START, drawTimeX + DRAW_TIME_IMG_ADD_X_END, DRAW_TIME_IMG_Y_END, frameH, true);
	std::wostringstream ss;
	ss << std::setfill(L'0') << std::setw(2) << min << L":";
	ss << std::setfill(L'0') << std::setw(2) << sec << L".";
	ss << std::setfill(L'0') << std::setw(2) << conma;
	DrawStringToHandle(drawTimeX, DRAW_TIME_Y, ss.str().c_str(), 0xffffff, font.GetHandle(32));
}

void SceneMain::DrawCarItemUI(int carNo, int x, int y, double rate) const
{
	auto& data = m_itemUIDatas[carNo];
	if (!data.isDraw) return;
	
	DrawRotaGraph(x, y, rate, 0.0, m_files[FileIndex::FRAME_ITEM]->GetHandle(), true);

	if (data.isBlink)
	{
		if ((data.count / 10) % 2) return;
	}

	auto item = m_raceMgr->GetItem(carNo);
	auto kind = item->GetKind();
	int handle = -1;
	if (kind == ItemBase::Kind::COIN)
	{
		handle = m_files[FileIndex::COIN]->GetHandle();
		rate *= 0.8;
	}
	else if (kind == ItemBase::Kind::KINOKO)
	{
		handle = m_files[FileIndex::ITEM_KINOKO]->GetHandle();
	}
	else if (kind == ItemBase::Kind::KINOKO_TRIPLE)
	{
		auto kinokoTriple = std::dynamic_pointer_cast<ItemKinokoTriple>(item);
		int count = kinokoTriple->GetCount() - 1;
		handle = m_files[FileIndex::ITEM_KINOKO_TRIPLE + count]->GetHandle();
	}
	else if (kind == ItemBase::Kind::CARAPACE_GREEN)
	{
		handle = m_files[FileIndex::ITEM_CARAPACE_GREEN]->GetHandle();
	}
	else if (kind == ItemBase::Kind::CARAPACE_RED)
	{
		handle = m_files[FileIndex::ITEM_CARAPACE_RED]->GetHandle();
	}
	else if (kind == ItemBase::Kind::BANANA)
	{
		handle = m_files[FileIndex::ITEM_BANANA]->GetHandle();
	}
	assert(handle >= 0);
	DrawRotaGraph(x, y, rate, 0.0, handle, true);
}

void SceneMain::DrawMiniMap(int carNo, int centerX, int centerY) const
{
	auto carPos = m_raceMgr->GetCarPos(carNo) * 0.02f;
	
	centerX += static_cast<int>(carPos.x) - 45 + 25;
	centerY -= static_cast<int>(carPos.z) + 30;

	DrawRotaGraph(centerX, centerY, 1.0, 0.0, m_files[FileIndex::MAP_PLAYER + carNo]->GetHandle(), true);
}

void SceneMain::DrawRapTimeUpdate(int carNo, int x, int y) const
{
	auto& data = m_rapTimeDatas[carNo];
	if (!data.isDraw) return;

	unsigned int color = 0xffffff;
	if (data.isUpdate && data.frame < DRAW_RAP_UPDATE_FLUSH_FRAME && data.frame % DRAW_RAP_UPDATE_FLUSH_INTERVAL < static_cast<int>(DRAW_RAP_UPDATE_FLUSH_INTERVAL * 0.5f))
	{
		color = 0xff8844;
	}

	int time = data.rapTime;
	int min = time / 3600;
	time %= 3600;
	int sec = time / 60;
	time %= 60;
	int conma = static_cast<int>(time / 60.0f * 100);
	std::wostringstream ss;
	const wchar_t* const STR[] =
	{
		L"First.",
		L"Second.",
		L"Last.",
	};
	ss << STR[carNo] << std::endl;
	ss << std::setfill(L'0') << std::setw(2) << min << L":";
	ss << std::setfill(L'0') << std::setw(2) << sec << L".";
	ss << std::setfill(L'0') << std::setw(2) << conma;
	auto& font = FontSystem::GetInstance();
	DrawStringToHandle(x, y, ss.str().c_str(), color, font.GetHandle(32));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
