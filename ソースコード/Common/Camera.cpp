#include "Camera.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Common/Game.h"
#include "Geometry/Easing.h"

namespace
{
	constexpr float DEF_FOV_ANGLE = 60.0f * Game::DEG_2_RAD;
}

Camera::Camera()
{
	m_info.resize(Game::MAX_PLAYER_NUM);
	for (auto& info : m_info)
	{
		info.fov = DEF_FOV_ANGLE;
	}

#ifdef _DEBUG
	m_debugInfo.n = Game::CAMERA_NEAR;
	m_debugInfo.f = Game::CAMERA_FAR;
	m_debugInfo.fov = 60 * Game::DEG_2_RAD;
	m_debugInfo.cameraPos = Vec3(0, 0, 0);
	m_debugInfo.targetPos = Vec3(0, -50, 100);
	m_dFront = Vec3(0, 0, 1);
	m_dRright = Vec3(1, 0, 0);
#endif
}

Camera& Camera::GetInstance()
{
	static Camera instance;
	return instance;
}

void Camera::Update(int no)
{
	auto& info = m_info[no];

	// 位置を更新
	auto newCameraPos = info.targetPos - info.dir * 200;
	newCameraPos.y += 100;
	info.cameraPos = Lerp(info.cameraPos, newCameraPos, 0.1f);

	/* 画角の更新 */ 
	// 画角を狭くする
	if (info.isFovNear)
	{
		--info.fovFrame;
		if (info.fovFrame < 0)
		{
			info.isFovNear = false;
			info.fov = DEF_FOV_ANGLE;
		}
		else
		{
			float rate = info.fovFrame / static_cast<float>(info.fovNearTime);
			info.fov = EaseOut(DEF_FOV_ANGLE, info.fovC, rate);
		}
	}
	// 画角を広くする
	if (info.isFovFar)
	{
		--info.fovFrame;
		if (info.fovFrame < 0)
		{
			info.fovFrame = info.fovNearTime;
			info.isFovFar = false;
			info.isFovNear = true;
			info.fov = info.fovC;
		}
		else
		{
			float rate = info.fovFrame / static_cast<float>(info.fovFarTime);
			info.fov = EaseIn(info.fovC, DEF_FOV_ANGLE, rate);
		}
	}
}

const Vec3& Camera::NowAppPos() const
{
	return m_info[m_nowAppNo].cameraPos;
}

/// <summary>
/// NearFarの設定
/// </summary>
/// <param name="no">カメラ番号</param>
/// <param name="n">Near</param>
/// <param name="f">Far</param>
void Camera::SetNearFar(int no, float n, float f)
{
	auto& info = m_info[no];
	info.n = n;
	info.f = f;
}

/// <summary>
/// 画角の設定
/// </summary>
/// <param name="no">カメラ番号</param>
/// <param name="angle">角度(deg)</param>
/// <param name="time">時間</param>
void Camera::SetFov(int no, float angle, int time)
{
	auto& info = m_info[no];
	info.fovC = angle * Game::DEG_2_RAD;

	info.fovFarTime = static_cast<int>(time * 0.2f);
	info.fovNearTime = static_cast<int>(time * 0.8f);
	info.fovFrame = info.fovFarTime;
	info.isFovFar = true;
	info.isFovNear = false;
}

/// <summary>
/// 初期位置
/// </summary>
/// <param name="no">カメラ番号</param>
/// <param name="targetPos">ターゲットの座標</param>
/// <param name="dir">向き</param>
void Camera::SetInitPos(int no, const Vec3& targetPos, const Vec3& dir)
{
	auto& info = m_info[no];

	auto dirN = dir.GetNormalized();
	auto pos = targetPos - dirN * 100;
	pos.y += 100;
	info.cameraPos = pos;
}

/// <summary>
/// 位置の設定
/// </summary>
/// <param name="no">カメラの番号</param>
/// <param name="targetPos">ターゲットの座標</param>
/// <param name="dir">向き</param>
void Camera::SetPos(int no, const Vec3& targetPos, const Vec3& dir)
{
	auto& info = m_info[no];

	auto dirN = dir.GetNormalized();
	
	info.targetPos = targetPos + dirN * 100;
	info.dir = dirN;
}

void Camera::AppInfo(int no)
{
#ifdef _DEBUG
	if (m_isDebug) return;
#endif

	m_nowAppNo = no;
	const auto& info = m_info.at(no);
	SetCameraNearFar(info.n, info.f);
	SetupCamera_Perspective(info.fov);
	SetCameraPositionAndTarget_UpVecY(info.cameraPos.VGet(), info.targetPos.VGet());
//	SetCameraPositionAndTarget_UpVecY(info.targetPos.VGet(), info.cameraPos.VGet());
}

const CameraInfo& Camera::GetInfo(int no) const
{
#ifdef _DEBUG
	if (m_isDebug)
	{
		return m_debugInfo;
	}
#endif

	return m_info.at(no);
}

#ifdef _DEBUG
#include "Input.h"
#include "Geometry/Quaternion.h"
void Camera::DebugUpdate(bool isDebug)
{
	m_isDebug = isDebug;
	if (!isDebug) return;

	auto& input = Input::GetInstance();

	auto rs = input.GetLStick(0);

	const float move = 2;

	m_debugInfo.cameraPos += m_dRright * rs.x * move;
	m_debugInfo.cameraPos += m_dFront * rs.y * move;
	m_debugInfo.targetPos += m_dRright * rs.x * move;
	m_debugInfo.targetPos += m_dFront * rs.y * move;

	auto lt = input.GetLT(0);
	auto rt = input.GetRT(0);

	m_debugInfo.cameraPos.y += lt * move;
	m_debugInfo.targetPos.y += lt * move;
	m_debugInfo.cameraPos.y -= rt * move;
	m_debugInfo.targetPos.y -= rt * move;

	auto ls = input.GetRStick(0);

	auto temp = m_debugInfo.targetPos;
	temp -= m_debugInfo.cameraPos;
	auto q = AngleAxis(ls.x * 1, Vec3::Up());
	temp = q * temp;
	m_dFront = q * m_dFront;
	m_dRright = q * m_dRright;
	temp += m_debugInfo.cameraPos;
	m_debugInfo.targetPos = temp;

	m_debugInfo.dir = m_dFront;

	SetCameraNearFar(m_debugInfo.n, m_debugInfo.f);
	SetupCamera_Perspective(m_debugInfo.fov);
	SetCameraPositionAndTarget_UpVecY(m_debugInfo.cameraPos.VGet(), m_debugInfo.targetPos.VGet());

	DrawFormatString(640, 16, 0xff00ff, L"cameraPos: %.2f, %.2f, %.2f", m_debugInfo.cameraPos.x, m_debugInfo.cameraPos.y, m_debugInfo.cameraPos.z);
	DrawFormatString(640, 32, 0xff00ff, L"targetPos: %.2f, %.2f, %.2f", m_debugInfo.targetPos.x, m_debugInfo.targetPos.y, m_debugInfo.targetPos.z);
}
#endif
