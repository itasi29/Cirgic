#pragma once

namespace Game
{
	// ��ʃT�C�Y
	constexpr int WINDOW_W = 1280;
	constexpr int WINDOW_H = 720;

	// FPS
	constexpr int FPS_FRAME = 16667;

	// ���w�n
	constexpr double PI = 3.14159265358979323846;
	constexpr float PI_F = 3.14159265358979323846f;
	constexpr float DEG_2_RAD = PI_F / 180.0f;
	constexpr float RAD_2_DEG = 180.0f / PI_F;

	// �J�����n
	constexpr float CAMERA_NEAR = 10;
	constexpr float CAMERA_FAR = 2000;

	// �A�i���O���͌n
	constexpr float ANALOG_TRIGGER_RANGE = 255.0f;	// ��肤��l�͈̔�
	constexpr float ANALOG_STICK_RANGE = 32768.0f;	// ��肤��l�͈̔�
	constexpr float ANALOG_DEAD_ZONE_INSIDE = 0.1f;		// �����f�b�h�]�[��
	constexpr float ANALOG_DEAD_ZONE_OUTSIDE = 0.9f;	// �O���f�b�h�]�[��

	// �d��
	constexpr float GRAVITY = 0.16f;
	constexpr float GRAVITY_MAX_SPEED = 2.6f;
	constexpr float GRAVITY_MAX_SPEED_SQ = GRAVITY_MAX_SPEED * GRAVITY_MAX_SPEED;

	/* TODO:�ȉ��͕ʂ̃w�b�_�[�ɕ����� */

	// �Ԃ̐�
//	constexpr int CAR_NUM = 6;
	// �ő�v���C�l��
	constexpr int MAX_PLAYER_NUM = 4;

	// �A�C�e���n
	constexpr int COIN_MAX = 10;
	constexpr int COIN_GET_NUM = 3;
}