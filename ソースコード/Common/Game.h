#pragma once

namespace Game
{
	// 画面サイズ
	constexpr int WINDOW_W = 1280;
	constexpr int WINDOW_H = 720;

	// FPS
	constexpr int FPS_FRAME = 16667;

	// 数学系
	constexpr double PI = 3.14159265358979323846;
	constexpr float PI_F = 3.14159265358979323846f;
	constexpr float DEG_2_RAD = PI_F / 180.0f;
	constexpr float RAD_2_DEG = 180.0f / PI_F;

	// カメラ系
	constexpr float CAMERA_NEAR = 10;
	constexpr float CAMERA_FAR = 2000;

	// アナログ入力系
	constexpr float ANALOG_TRIGGER_RANGE = 255.0f;	// 取りうる値の範囲
	constexpr float ANALOG_STICK_RANGE = 32768.0f;	// 取りうる値の範囲
	constexpr float ANALOG_DEAD_ZONE_INSIDE = 0.1f;		// 内側デッドゾーン
	constexpr float ANALOG_DEAD_ZONE_OUTSIDE = 0.9f;	// 外側デッドゾーン

	// 重力
	constexpr float GRAVITY = 0.16f;
	constexpr float GRAVITY_MAX_SPEED = 2.6f;
	constexpr float GRAVITY_MAX_SPEED_SQ = GRAVITY_MAX_SPEED * GRAVITY_MAX_SPEED;

	/* TODO:以下は別のヘッダーに分ける */

	// 車の数
//	constexpr int CAR_NUM = 6;
	// 最大プレイ人数
	constexpr int MAX_PLAYER_NUM = 4;

	// アイテム系
	constexpr int COIN_MAX = 10;
	constexpr int COIN_GET_NUM = 3;
}