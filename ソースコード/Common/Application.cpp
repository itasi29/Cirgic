#include "Application.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Game.h"
#include "Input.h"
#include "Camera.h"
#include "SoundSystem.h"
#include "File/FileManager.h"
#include "File/FontSystem.h"
#include "File/RankingSystem.h"
#include "Scene/SceneManager.h"
#include "MyEngine/MyEngine.h"

namespace
{
    const wchar_t* const kWindowText = L"サージック！";
}

Application::Application() :
    m_isEnd(false),
#ifdef _DEBUG
    m_isWindows(true),
#else
    m_isWindows(false),
#endif
    m_screenLight(255)
{
}

Application& Application::GetInstance()
{
    static Application app;
    return app;
}

bool Application::Init()
{
    ChangeWindowMode(m_isWindows);
    // スクリーン変更しても画像をDeleteしないように
    SetChangeScreenModeGraphicsSystemResetFlag(false);

    // 画面サイズ変更
    SetGraphMode(Game::WINDOW_W, Game::WINDOW_H, 32);
    SetWindowText(kWindowText);

    if (DxLib_Init() == -1)
    {
        return false;
    }
    SetDrawScreen(DX_SCREEN_BACK);

    // ZBufferの使用
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(true);

    // Effekseerの初期化
    // 引数：パーティクル数
    Effekseer_Init(8000);
    // 歪みエフェクトの初期化
    Effekseer_InitDistortion();
    // フルスク切り替え時におかしくならないように設定
    SetChangeScreenModeGraphicsSystemResetFlag(false);
    Effekseer_SetGraphicsDeviceLostCallbackFunctions();


    std::random_device rd;
    m_mt = std::mt19937(rd());

    return true;
}

void Application::Run()
{
    auto& fileMgr = FileManager::GetInstance();
    auto& scnMgr = SceneManager::GetInstance();
    auto& fontSys = FontSystem::GetInstance();
    auto& input = Input::GetInstance();
    auto& physics = MyEngine::Physics::GetInstance();
    auto& camera = Camera::GetInstance();
    auto& rank = RankingSystem::GetInstance();
    auto& sndSys = SoundSystem::GetInstance();
#ifdef _DEBUG
    auto& debug = MyEngine::DebugDraw::GetInstance();
    bool isUpdate = true;
    bool isStep = false;
    bool isFreeCamera = false;

    LONGLONG updateTime;
    LONGLONG drawTime;
#endif
    fileMgr.Init();
    scnMgr.Init();
    fontSys.Init();
    physics.Init(4, Vec3(-5000, -10, -5000), Vec3(5000, 2500, 5000));
    rank.Init();

    long long time;
    while (ProcessMessage() != -1) 
    {
        time = GetNowHiPerformanceCount();

        ClearDrawScreen();

        for (int i = 0; i < Game::MAX_PLAYER_NUM; ++i)
        {
            input.Update(i);
            camera.Update(i);
        }

#ifdef _DEBUG
//        debug.Gulid();
        updateTime = GetNowHiPerformanceCount();

        if (isStep)
        {
            if (input.IsTriggerd(KEY_INPUT_P))
            {
                isUpdate = true;
            }
            if (input.IsTriggerd(KEY_INPUT_RETURN))
            {
                isStep = false;
            }
        }
        else
        {
            isUpdate = true;
            if (input.IsTriggerd(KEY_INPUT_P))
            {
                isStep = true;
            }
        }

        debug.SetStop(!isUpdate);

        if (isUpdate)
        {
            debug.Clear();
#endif
            if (!scnMgr.IsOpenOption())
            {
                physics.Update();
                 sndSys.Update();
            }
            scnMgr.Update();
#ifdef _DEBUG
            isUpdate = false;
        }

        if (input.IsTriggerd(KEY_INPUT_C))
        {
            isFreeCamera = !isFreeCamera;
        }
        camera.DebugUpdate(isFreeCamera);

        if (isStep)
        {
            debug.DrawString(16, 32, 0xff00ff, L"動作：ステップ中");
        }
        else
        {
            debug.DrawString(16, 32, 0xff00ff, L"動作：通常中");
        }
        drawTime = GetNowHiPerformanceCount();
        updateTime = drawTime - updateTime;
#endif

        scnMgr.Draw();

#ifdef _DEBUG
      debug.Draw();
        drawTime = GetNowHiPerformanceCount() - drawTime;
        DrawFormatString(16, 48, 0xff00ff, L"FPS : %.2f", GetFPS());
        DrawFormatString(16, 684, 0xff00ff, L"更新 : %ld", updateTime);
        DrawFormatString(16, 700, 0xff00ff, L"描画 : %ld", drawTime);
#endif
        ScreenFlip();

        if (m_isEnd) 
        {
            break;
        }
        while (Game::FPS_FRAME > GetNowHiPerformanceCount() - time);
    }

    scnMgr.End();
    rank.End();
}

void Application::Terminate()
{
    // Effekseerを終了する。
    Effkseer_End();
    DxLib_End();
}

/// <summary>
/// 乱数の取得
/// </summary>
/// <returns>乱数</returns>
int Application::GetRand()
{
    return m_mt();
}

/// <summary>
/// 乱数の取得(整数で範囲制限)
/// </summary>
/// <param name="min">最小値</param>
/// <param name="max">最大値</param>
/// <returns>乱数</returns>
int Application::GetRand(int min, int max)
{
    std::uniform_int_distribution<int> uid(min, max);
    return uid(m_mt);
}

/// <summary>
/// 乱数の取得(実数で範囲制限)
/// </summary>
/// <param name="min">最小値</param>
/// <param name="max">最大値</param>
/// <returns>乱数</returns>
float Application::GetRand(float min, float max)
{
    std::uniform_real_distribution<float> urd(min, max);
    return urd(m_mt);
}

/// <summary>
/// 画面の明るく変更
/// </summary>
/// <param name="val">追加値</param>
/// <returns>true: 変更した/ false: 変更していない</returns>
bool Application::AddScreenLight(int val)
{
    bool isChange = false;

    int temp = m_screenLight + val;
    if (temp > 255)
    {
        m_screenLight = 255;
        isChange = false;
    }
    else
    {
        m_screenLight = temp;
        isChange = true;
    }
    
    return isChange;
}

/// <summary>
/// 画面の暗く変更
/// </summary>
/// <param name="val">減少値</param>
/// <returns>true: 変更した/ false: 変更していない</returns>
bool Application::SubScreenLight(int val)
{
    bool isChange = false;

    int temp = m_screenLight - val;
    if (temp < 0)
    {
        m_screenLight = 0;
        isChange = false;
    }
    else
    {
        m_screenLight = temp;
        isChange = true;
    }

    return isChange;
}
