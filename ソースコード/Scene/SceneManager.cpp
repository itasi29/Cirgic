#include "SceneManager.h"
#include <DxLib.h>
#ifdef _DEBUG
#include "SceneDebug.h"
#else
#include "SceneTitle.h"
#endif
#include "SceneOption.h"
#include "Common/Game.h"
#include "Common/CommandList.h"
#include "Common/Application.h"
#include "Common/Input.h"
#include "Common/SoundSystem.h"
#include "File/FileID.h"
#include "File/FileBase.h"
#include "File/FileManager.h"

namespace
{
	constexpr float FADE_FRAME = 30.0f;
	constexpr float FADE_SPEED = 1.0f / FADE_FRAME;
}

SceneManager::SceneManager() :
	m_scene(nullptr),
	m_updateFunc(&SceneManager::NormalUpdate),
	m_drawFunc(&SceneManager::NormalDraw),
	m_isOpenOption(false),
	m_bgmH(-1),
	m_fadeFunc(&SceneManager::FadeNone),
	m_fadeRate(0.0f),
	m_isFade(false)
{
}

SceneManager::~SceneManager()
{
}

SceneManager& SceneManager::GetInstance()
{
	static SceneManager mgr;
	return mgr;
}

void SceneManager::Init()
{
	// 最初のシーンのメモリ確保
#ifdef _DEBUG
	m_scene = std::make_shared<SceneDebug>();
#else
	m_scene = std::make_shared<SceneTitle>();
	m_fadeFunc = &SceneManager::FadeIn;
	m_fadeRate = 1.0f;
	m_isFade = true;
#endif

	ChangeSkyDomeTexIndex();
	m_scene->Init();

	CheckFileLoadingStart();
}

void SceneManager::Update()
{
	auto& input = Input::GetInstance();
	if (input.IsTriggerd(0, Command::BTN_OPTION))
	{
		OptionOpen();
	}
	if (m_optionScnene)
	{
		m_optionScnene->Update(false);
		return;
	}
	(this->*m_updateFunc)();
}

void SceneManager::Draw() const
{
	(this->*m_drawFunc)();
	if (m_optionScnene)
	{
		m_optionScnene->Draw();
	}

	auto& app = Application::GetInstance();
	int light = app.GetScreenLight();
	int alpha = 255 - light - 50;
	alpha = std::max<int>(alpha, 0);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, Game::WINDOW_W, Game::WINDOW_H, 0, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneManager::End()
{
	m_scene->End();
}

void SceneManager::Change(const std::shared_ptr<SceneBase>& next, bool isFade)
{
	if (isFade)
	{
		// フェード処理初期化
		m_fadeRate = 0.0f;
		m_isFade = true;
		// フェードアウトへ
		m_fadeFunc = &SceneManager::FadeOut;

		m_nextScene = next;
	}
	else
	{
		// 終了処理
		m_scene->End();

		// シーン変更
		m_scene = next;
		// 初期化処理
		m_scene->Init();

		CheckFileLoadingStart();
	}
}

void SceneManager::ChangeBgmH(int handle)
{
	m_bgmH = handle;
}

void SceneManager::OptionOpen()
{
	if (m_isOpenOption) return;

	auto scene = std::make_shared<SceneOption>();
	scene->Init();
	scene->SetReturnTitle(m_scene->GetNowKind() != SceneBase::Kind::TITLE);
	m_optionScnene = scene;
	m_isOpenOption = true;
}

void SceneManager::OptionClose()
{
	if (!m_isOpenOption) return;

	m_optionScnene->End();
	m_optionScnene = nullptr;
	m_isOpenOption = false;
}

void SceneManager::ChangeSkyDomeTexIndex()
{
	auto& app = Application::GetInstance();
	auto& mgr = FileManager::GetInstance();

	const wchar_t* const id[] =
	{
		FileID::Img::TEX_SKY_DOME_1,
		FileID::Img::TEX_SKY_DOME_2,
		FileID::Img::TEX_SKY_DOME_3,
		FileID::Img::TEX_SKY_DOME_4,
	};
	int rand = app.GetRand(0, 3);
	m_skyDomeTexIndex = mgr.Load(id[rand]);
}

int SceneManager::GetSkyDomeTexIndex() const
{
	return m_skyDomeTexIndex->GetHandle();
}

void SceneManager::NormalUpdate()
{
	(this->*m_fadeFunc)();
	m_scene->Update(m_isFade);
}

void SceneManager::FileLoadingUpdate()
{
	// TODO: ここにロード終わる野を確認してNormalに代わる処理をかく
	auto num = GetASyncLoadNum();
	if (num == 0)
	{
		m_updateFunc = &SceneManager::NormalUpdate;
		m_drawFunc = &SceneManager::NormalDraw;
	}
}

void SceneManager::NormalDraw() const
{
	m_scene->Draw();

	if (!m_isFade) return;

	// フェード処理
	int alpha = static_cast<int>(255 * m_fadeRate);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, Game::WINDOW_W, Game::WINDOW_H, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneManager::FileLoadingDraw() const
{
	// TODO: ロード中はNowLoadingを出すようにする
	DrawString(600, 360, L"ロード中", 0xffffff);
}

void SceneManager::FadeNone()
{
	// ハンドルが入っていな場合は流さない
	if (m_bgmH > -1)
	{
		auto& snd = SoundSystem::GetInstance();
		snd.PlayBgm(m_bgmH);
	}
}

void SceneManager::FadeIn()
{
	m_fadeRate -= FADE_SPEED;

	// フェードイン完了したら
	if (m_fadeRate < 0.0f)
	{
		// フェード終了
		m_isFade = false;
		// シーンの処理へ
		m_fadeFunc = &SceneManager::FadeNone;
	}

	// ハンドルが入っていな場合は流さない
	if (m_bgmH > -1)
	{
		auto& snd = SoundSystem::GetInstance();
		snd.PlayFadeBgm(m_bgmH, 1.0f - m_fadeRate);
	}
}

void SceneManager::FadeOut()
{
	m_fadeRate += FADE_SPEED;

	// フェードアウト完了したら
	if (m_fadeRate > 1.0f)
	{
		// 終了処理
		m_scene->End();
		// シーン変更
		m_scene = m_nextScene;
		// 初期化処理
		m_scene->Init();
		// フェード割合補正
		m_fadeRate = 1.0f;
		// フェードインへ
		m_fadeFunc = &SceneManager::FadeIn;

		CheckFileLoadingStart();
		return;
	}

	// ハンドルが入っていな場合は流さない
	if (m_bgmH > -1)
	{
		auto& snd = SoundSystem::GetInstance();
		snd.PlayFadeBgm(m_bgmH, 1.0f - m_fadeRate);
	}
}

void SceneManager::CheckFileLoadingStart()
{
	auto num = GetASyncLoadNum();
	// 非同期ロードしているものがあったらFileLoadingに変更
	if (num > 0)
	{
		// 非同期ロードの開始
		m_updateFunc = &SceneManager::FileLoadingUpdate;
		m_drawFunc = &SceneManager::FileLoadingDraw;
	}
}

