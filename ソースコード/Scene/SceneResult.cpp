#include "SceneResult.h"
#include <DxLib.h>
#include <iomanip>
#include <sstream>
#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "Common/Game.h"
#include "Common/Application.h"
#include "Common/CommandList.h"
#include "Common/Input.h"
#include "Common/SoundSystem.h"
#include "Geometry/Easing.h"
#include "File/FileManager.h"
#include "File/FileBase.h"
#include "File/FontSystem.h"
#include "File/RankingSystem.h"
#include "File/FileID.h"

namespace
{
	const enum FileIndex : int
	{
		FRAME_1P,
		FRAME_2P,
		FRAME_3P,
		FRAME_4P,
		FRAME_NPC,
		S_1,
		S_2,
		S_3,
		S_4,
		S_5,
		S_6,
		FRAME_BACK,
		FRAME,
		FRAME_SELECT_R,
		FRAME_SELECT_L,
		TEX_CAR,
		BGM_RESULT = TEX_CAR + 4,
		SE_RANKING_PUSH,
		SE_CURSOR_MOVE,
		SE_SELECT,
		SKY_DOME,
		ARCH,
		CAR,
		ROAD,
		GROUND,
		STAND,
		PODIUM,
	};

	constexpr float RANDOM_ROT_AXIS_MIN = -1.0f;
	constexpr float RANDOM_ROT_AXIS_MAX = 1.0f;
	constexpr float FRAME_SPEED_PODIUM = 0.05f;
	constexpr int ANGLE_POPDIUM = 9;

	constexpr int FRAME_DELAY_BASE_PODIUM = 40;
	constexpr float CAR_UP_POS_Y_BASE = 100.0f;

	constexpr int STANDINT_FRAME_DRAW_X_START = Game::WINDOW_W;
	constexpr int STANDINT_FRAME_DRAW_X_END = 128;
	constexpr int STANDINT_FRAME_DRAW_Y = 96;

	constexpr int STANDINT_FRAME_INTERVAL_DRAW_Y = 100;

	constexpr int STANDING_FRAME_INTEVAL_CREATE = 30;
	constexpr float STANDING_FRAME_SLIDE_SPEED = 0.0625f;

	constexpr int FRAME_Y = 300;
	constexpr int START_FRAME_LEFT = 395;
	constexpr int START_FRAME_RIGHT = 885;
	constexpr int SWING_FRAME_HEIGHT = 120;
	constexpr int SWING_FRAME_WIDTH = 5;

	constexpr int DELTA_TIME = 24;	

	const Vec3 START_POS[3] = {
		Vec3(-42, 90, -112),
		Vec3(-85, 73, -155),
		Vec3(1.5, 55, -70),
	};

	constexpr int FRAME_CAR_Y_ADD = -43;
	constexpr int STANDING_CAR_X_ADD = 64;
	constexpr int DELTA_Y_ADD = -16;
	constexpr int DELTA_RAP_X_ADD = 128;
	constexpr int DELTA_TOTAL_X_ADD = 560;


	constexpr int FRAME_CONTINUE_RETURN_X = 640;
	constexpr int FRAME_BACK_CONTINUE_RETURN_Y = 360;
	constexpr int FRAME_CONTINUE_Y = 300;
	constexpr int FRAME_RETURN_Y = 420;
	constexpr int STR_CONTINUE_RETURN_X = 530;
	constexpr int STR_CONTINUE_Y = 275;
	constexpr int STR_RETURN_Y = 395;
}

SceneResult::SceneResult() :
	SceneBase(Kind::RESULT),
	m_updateFunc(&SceneResult::UpdateStanding),
	m_lightH(0),
	m_playerNum(0),
	m_createNum(0),
	m_createCount(0),
	m_isAllCreate(false),
	m_isAllMoved(false),
	m_isRankUpdate(false),
	m_index(0),
	m_shakeFrame(0.0f),
	m_selectFrame{ START_FRAME_LEFT, START_FRAME_RIGHT, FRAME_Y }
{
}

void SceneResult::Init()
{
	// 画像読み込み
	auto& fileMgr = FileManager::GetInstance();
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_STANDING_1));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_STANDING_2));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_STANDING_3));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_STANDING_4));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_STANDING_NPC));
	m_files.push_back(fileMgr.Load(FileID::Img::STANDING_1));
	m_files.push_back(fileMgr.Load(FileID::Img::STANDING_2));
	m_files.push_back(fileMgr.Load(FileID::Img::STANDING_3));
	m_files.push_back(fileMgr.Load(FileID::Img::STANDING_4));
	m_files.push_back(fileMgr.Load(FileID::Img::STANDING_5));
	m_files.push_back(fileMgr.Load(FileID::Img::STANDING_6));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_OPTION));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_SELECT_RIGHT));
	m_files.push_back(fileMgr.Load(FileID::Img::FRAME_SELECT_LEFT));
	m_files.push_back(fileMgr.Load(FileID::Img::TEX_CAR_RED));
	m_files.push_back(fileMgr.Load(FileID::Img::TEX_CAR_BULE));
	m_files.push_back(fileMgr.Load(FileID::Img::TEX_CAR_YELLOW));
	m_files.push_back(fileMgr.Load(FileID::Img::TEX_CAR_PURPLE));
	m_files.push_back(fileMgr.Load(FileID::Snd::BGM::RESULT));
	m_files.push_back(fileMgr.Load(FileID::Snd::SE::RANKING_PUSH));
	m_files.push_back(fileMgr.Load(FileID::Snd::SE::CURSOR_MOVE));
	m_files.push_back(fileMgr.Load(FileID::Snd::SE::SELECT));
	m_files.push_back(fileMgr.Load(FileID::Mdl::SKY_DOME));
	m_files.push_back(fileMgr.Load(FileID::Mdl::ARCH_GOAL));
	m_files.push_back(fileMgr.Load(FileID::Mdl::CAR));
	m_files.push_back(fileMgr.Load(FileID::Mdl::LOAD_STRAIGHT));
	m_files.push_back(fileMgr.Load(FileID::Mdl::GROUND));
	m_files.push_back(fileMgr.Load(FileID::Mdl::STAND));
	m_files.push_back(fileMgr.Load(FileID::Mdl::PUDIUM));

	m_scnMgr.ChangeBgmH(m_files[FileIndex::BGM_RESULT]->GetHandle());

	LoadAndCreateObjects(L"Data/Stage/Other/Road.loc", FileIndex::ROAD);
	LoadAndCreateObjects(L"Data/Stage/Other/Ground.loc", FileIndex::GROUND);
	LoadAndCreateObjects(L"Data/Stage/Other/Stand.loc", FileIndex::STAND);
	LoadAndCreateObject(FileIndex::ARCH, Vec3(), AngleAxis(180, Vec3::Up()), Vec3(0.01f, 0.01f, 0.01f));
	LoadAndCreateObject(FileIndex::SKY_DOME, Vec3(), Quaternion(0, 0, 0, 1), Vec3(10, 10, 10));
	LoadAndCreateObject(FileIndex::PODIUM, Vec3(-25, 60, -130), AngleAxis(180, Vec3::Up()), Vec3(0.01f, 0.01f, 0.01f));
	for (auto& data : m_createPodiumCarList)
	{
		LoadAndCreateCar(data.carNo, data.standing);
	}

	auto pos = Vec3(300, 150, -350);
	auto target = Vec3(-50, 15, -100);
	m_lightH = CreateDirLightHandle((target - pos).GetNormalized().VGet());
	SetCameraNearFar(Game::CAMERA_NEAR, Game::CAMERA_FAR);
	SetupCamera_Perspective(60.0f * Game::DEG_2_RAD);
	SetCameraPositionAndTarget_UpVecY(pos.VGet(), target.VGet());

	MV1SetTextureGraphHandle(m_files[FileIndex::SKY_DOME]->GetHandle(), 0, m_scnMgr.GetSkyDomeTexIndex(), false);
}

void SceneResult::Update(bool isFade)
{
	(this->*m_updateFunc)();
	for (auto& info : m_podiumCarList)
	{
		if (info.isDelay)
		{
			--info.moveDelayFrame;
			info.isDelay = info.moveDelayFrame > 0;

			auto& app = Application::GetInstance();
			float x = app.GetRand(RANDOM_ROT_AXIS_MIN, RANDOM_ROT_AXIS_MAX);
			float y = app.GetRand(RANDOM_ROT_AXIS_MIN, RANDOM_ROT_AXIS_MAX);
			float z = app.GetRand(RANDOM_ROT_AXIS_MIN, RANDOM_ROT_AXIS_MAX);
			info.axis = Vec3(x, y, z);
			if (info.axis.SqLength() == 0)
			{
				info.axis = Vec3::Up();
			}
			else
			{
				info.axis.Normalize();
			}
		}
		else
		{
			if (info.isRateUp)
			{
				info.rate += FRAME_SPEED_PODIUM;
				if (info.rate > 1.0f)
				{
					info.rate = 1.0f;
					info.isRateUp = false;
				}
			}
			else
			{
				info.rate -= FRAME_SPEED_PODIUM;
				if (info.rate < 0.0f)
				{
					info.rate = 0.0f;
					info.isRateUp = true;

					info.isDelay = true;
					info.moveDelayFrame = FRAME_DELAY_BASE_PODIUM;
				}
			}
			
			info.rot = info.rot * AngleAxis(ANGLE_POPDIUM, info.axis);

			info.pos = EaseOut(info.startPos, info.upPos, info.rate);

			auto mat = Scale(Vec3(0.01f, 0.01f, 0.01f)) * info.rot.GetMat() * Pos(info.pos);
			MV1SetMatrix(info.handle, mat.GetMATRIX());
		}
	}
}

void SceneResult::Draw() const
{
#ifdef _DEBUG
	DrawString(8, 8, L"SceneResult", 0xffffff);
#endif
	for (auto& h : m_entityModelHnaldeList)
	{
		MV1DrawModel(h);
	}

	auto& font = FontSystem::GetInstance();
	for (int i = 0; i < m_standingFrames.size(); ++i)
	{
		auto& info = m_standingFrames[i];
		DrawRotaGraphFast3(info.x, info.y + FRAME_CAR_Y_ADD, 0, 0, 12.0f, 1.0f, 0.0f, m_files[info.fileIndex]->GetHandle(), true);

		if (!info.isMove)
		{
			DrawRotaGraph(info.x + STANDING_CAR_X_ADD, info.y, 0.25f, 0.0f, m_files[i + FileIndex::S_1]->GetHandle(), true);

			DrawTime(info.deltRapTime, info.x + DELTA_RAP_X_ADD, info.y + DELTA_Y_ADD, L"ラップタイム:　");
			DrawTime(info.deltTotalTime, info.x + DELTA_TOTAL_X_ADD, info.y + DELTA_Y_ADD, L"トータルタイム:　");
		}
	}

	if (m_isAllMoved)
	{
		DrawRotaGraph(FRAME_CONTINUE_RETURN_X, FRAME_BACK_CONTINUE_RETURN_Y, 0.5, 0.0, m_files[FileIndex::FRAME_BACK]->GetHandle(), true);

		DrawRotaGraph(FRAME_CONTINUE_RETURN_X, FRAME_CONTINUE_Y, 0.4, 0.0, m_files[FileIndex::FRAME]->GetHandle(), true);
		DrawRotaGraph(FRAME_CONTINUE_RETURN_X, FRAME_RETURN_Y, 0.4, 0.0, m_files[FileIndex::FRAME]->GetHandle(), true);

		DrawStringToHandle(STR_CONTINUE_RETURN_X, STR_CONTINUE_Y, L"続けて遊ぶ", 0x0, font.GetHandle(48));
		DrawStringToHandle(STR_CONTINUE_RETURN_X, STR_RETURN_Y, L"タイトルへ", 0x0, font.GetHandle(48));

		auto& info = m_selectFrame;
		DrawRotaGraph(info.lX, info.y, 0.5, 0.0, m_files[FileIndex::FRAME_SELECT_L]->GetHandle(), true);
		DrawRotaGraph(info.rX, info.y, 0.5, 0.0, m_files[FileIndex::FRAME_SELECT_R]->GetHandle(), true);
	}

#ifdef _DEBUG
	DrawLine(640, 0, 640, 720, 0xff0000);
#endif
}

void SceneResult::End()
{
	DeleteLightHandle(m_lightH);
	for (auto& h : m_entityModelHnaldeList)
	{
		MV1DeleteModel(h);
	}
}

void SceneResult::Init(int playerNum, std::vector<RaceData> data)
{
	m_playerNum = playerNum;
	m_data = data;

	int i = 0;
	auto& rank = RankingSystem::GetInstance();
	for (auto& item : data)
	{
		int totalTime = 0;
		int rapTime = 999999;
		for (auto& time : item.time)
		{
			if (time < rapTime)
			{
				rapTime = time;
			}
			totalTime += time;
		}
		
		if (rank.Update(totalTime, rapTime))
		{
			m_isRankUpdate = true;
		}

		if (item.stading < 4)
		{
			m_createPodiumCarList.emplace_back(CreatePodiumCarData{i, item.stading});
		}
		++i;
	}
}

void SceneResult::UpdateStanding()
{
	CreateStandingFrame();
	UpdateStandingFrame();

	if (m_isAllMoved)
	{
		m_updateFunc = &SceneResult::UpdateReturn;
	}
}

void SceneResult::UpdateReturn()
{
	auto& input = Input::GetInstance();
	auto& sndSys = SoundSystem::GetInstance();
	// 左
	if (input.IsTriggerd(0, Command::BTN_UP))
	{
		sndSys.PlaySe(m_files[FileIndex::SE_CURSOR_MOVE]->GetHandle());
		m_index = (m_index - 1 + 2) % 2;
		StartSelectFrameMove();
	}
	// 右
	if (input.IsTriggerd(0, Command::BTN_DOWN))
	{
		sndSys.PlaySe(m_files[FileIndex::SE_CURSOR_MOVE]->GetHandle());
		m_index = (m_index + 1) % 2;
		StartSelectFrameMove();
	}
	// 決定
	if (input.IsTriggerd(0, Command::BTN_OK))
	{
		sndSys.PlaySe(m_files[FileIndex::SE_SELECT]->GetHandle());
		if (m_index == 0)
		{
			m_nextSceneKind = Kind::MAIN;
			auto next = std::dynamic_pointer_cast<SceneMain>(Change());
			next->InitPlayerNum(m_playerNum);
			m_scnMgr.Change(next);
		}
		else
		{
			m_nextSceneKind = Kind::TITLE;
			auto next = std::dynamic_pointer_cast<SceneTitle>(Change());
			m_scnMgr.Change(next);
		}
	}

	auto& info = m_selectFrame;
	m_shakeFrame += 0.02f;
	float sin = std::abs(std::sin(m_shakeFrame));
	info.lX = EaseIn(START_FRAME_LEFT - SWING_FRAME_WIDTH, START_FRAME_LEFT + SWING_FRAME_WIDTH, sin);
	info.rX = EaseIn(START_FRAME_RIGHT + SWING_FRAME_WIDTH, START_FRAME_RIGHT - SWING_FRAME_WIDTH, sin);
	if (info.isMove)
	{
		info.rate += 0.2f;
		if (info.rate > 1.0f)
		{
			info.rate = 1.0f;
			info.isMove = false;
		}

		info.y = EaseIn(info.preY, info.nextY, info.rate);
	}
}

void SceneResult::DrawTime(int time, int x, int y, const wchar_t* const baseStr) const
{
	int min = time / 3600;
	time %= 3600;
	int sec = time / 60;
	time %= 60;
	int conma = static_cast<int>(time / 60.0f * 100);
	std::wostringstream ss;
	ss << baseStr;
	ss << std::setfill(L'0') << std::setw(2) << min << L":";
	ss << std::setfill(L'0') << std::setw(2) << sec << L".";
	ss << std::setfill(L'0') << std::setw(2) << conma;
	auto& font = FontSystem::GetInstance();
	DrawStringToHandle(x, y, ss.str().c_str(), 0xffffff, font.GetHandle(32));
}

void SceneResult::CreateStandingFrame()
{
	if (m_isAllCreate) return;
	auto& input = Input::GetInstance();
	bool isAccel = input.IsTriggerd(0, Command::BTN_OK);

	++m_createCount;
	if (m_createCount > STANDING_FRAME_INTEVAL_CREATE || isAccel)
	{
		int y = STANDINT_FRAME_DRAW_Y + m_createNum * STANDINT_FRAME_INTERVAL_DRAW_Y;
		int i = 0;
		for (; i < m_data.size(); ++i)
		{
			if (m_data[i].stading == m_createNum + 1) break;
		}
		auto& sndSys = SoundSystem::GetInstance();
		sndSys.PlaySe(m_files[FileIndex::SE_RANKING_PUSH]->GetHandle());
		CreateStandingFrame(i, FileIndex::FRAME_1P, y);

		m_createCount = 0;
		++m_createNum;
		//if (m_createNum > Game::CAR_NUM - 1)
		if (m_createNum > m_playerNum - 1)
		{
			m_isAllCreate = true;
		}
	}
}

void SceneResult::CreateStandingFrame(int index, int fileIndex, int y)
{
	StandingFrame sf;
	sf.index = index;
	sf.totalTime = 0;
	sf.rapTime = 999999;
	for (int i = 0; i < 3; ++i) 
	{
		auto time = m_data[index].time[i];
		if (time < sf.rapTime)
		{
			sf.rapTime = time;
		}
		sf.totalTime += time;
	}
	sf.deltTotalTime = 0;
	sf.deltRapTime = 0;

	sf.x = STANDINT_FRAME_DRAW_X_START;
	sf.y = y;
	sf.fileIndex = index;
	sf.rate = 0.0f;
	sf.isMove = true;

	m_standingFrames.push_back(sf);
}

void SceneResult::UpdateStandingFrame()
{
	auto& input = Input::GetInstance();
	bool isAccel = input.IsTriggerd(0, Command::BTN_OK);
	bool isMoved = true;
	for (auto& info : m_standingFrames)
	{
		if (isAccel)
		{
			info.rate = 1.0f;
			info.isMove = false;
			info.x = STANDINT_FRAME_DRAW_X_END;
			info.deltRapTime = info.rapTime;
			info.deltTotalTime = info.totalTime;
		}
		else if (info.isMove)
		{
			isMoved = false;
			info.rate += STANDING_FRAME_SLIDE_SPEED;
			if (info.rate > 1.0f)
			{
				info.rate = 1.0f;
				info.isMove = false;
			}
			info.x = EaseOut(STANDINT_FRAME_DRAW_X_START, STANDINT_FRAME_DRAW_X_END, info.rate);
		}
		else
		{
			if (AddTime(info.deltRapTime, info.rapTime))
			{
				isMoved = false;
			}
			if (AddTime(info.deltTotalTime, info.totalTime))
			{
				isMoved = false;
			}
		}
	}

	m_isAllMoved = isMoved && m_isAllCreate;
}

bool SceneResult::AddTime(int& deltTime, int time)
{
	if (deltTime < time) 
	{
		deltTime += DELTA_TIME;
		if (deltTime >= time)
		{
			deltTime = time;
		}

		return true;
	}

	return false;
}

void SceneResult::StartSelectFrameMove()
{
	m_selectFrame.isMove = true;
	m_selectFrame.rate = 0.0f;

	m_selectFrame.preY = m_selectFrame.y;
	m_selectFrame.nextY = FRAME_Y + m_index * SWING_FRAME_HEIGHT;
}

void SceneResult::LoadAndCreateObjects(const wchar_t* path, int fileIndex)
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

void SceneResult::LoadAndCreateObject(int fileIndex, const Vec3& pos, const Quaternion& rot, const Vec3& scale)
{
	int h = MV1DuplicateModel(m_files[fileIndex]->GetHandle());
	Matrix4x4 mat = Scale(scale) * rot.GetMat() * Pos(pos);
	MV1SetMatrix(h, mat.GetMATRIX());

	m_entityModelHnaldeList.emplace_back(h);
}

void SceneResult::LoadAndCreateCar(int carNo, int standing)
{
	PodiumCarData data;

	data.handle = MV1DuplicateModel(m_files[FileIndex::CAR]->GetHandle());
	int tex = m_files[FileIndex::TEX_CAR + carNo]->GetHandle();
	int texIdx = MV1GetMaterialDifMapTexture(data.handle, 0);
	MV1SetTextureGraphHandle(data.handle, texIdx, tex, false);


	--standing;

	data.rate = 0.0f;
	data.isRateUp = true;
	data.isDelay = true;
	data.moveDelayFrame = FRAME_DELAY_BASE_PODIUM * standing;
	data.pos = START_POS[standing];
	data.startPos = data.pos;
	data.upPos = data.pos + Vec3::Up() * CAR_UP_POS_Y_BASE;
	data.rot = AngleAxis(-45, Vec3::Up());

	auto mat = Scale(Vec3(0.01f, 0.01f, 0.01f)) * data.rot.GetMat() * Pos(data.pos);
	MV1SetMatrix(data.handle, mat.GetMATRIX());

	m_entityModelHnaldeList.emplace_back(data.handle);
	m_podiumCarList.emplace_back(data);
}
