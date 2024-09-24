#include "Input.h"
#include <DxLib.h>
#include <cassert>
#include <array>
#include "Game.h"
#include "CommandList.h"

Input::Input()
#ifdef _DEBUG
	: m_keystate {},
	  m_preKeystate {}
#endif
{
	// 上
	m_commandTable[Command::BTN_UP]			= PAD_INPUT_UP;
	// 下
	m_commandTable[Command::BTN_DOWN]		= PAD_INPUT_DOWN;
	// 右
	m_commandTable[Command::BTN_RIGHT]		= PAD_INPUT_RIGHT;
	// 左
	m_commandTable[Command::BTN_LEFT]		= PAD_INPUT_LEFT;
	// オプション
	m_commandTable[Command::BTN_OPTION]		= PAD_INPUT_8;
	// 決定
	m_commandTable[Command::BTN_OK]			= PAD_INPUT_1;
	// キャンセル
	m_commandTable[Command::BTN_CANCEL]		= PAD_INPUT_2;
	// アクセル
	m_commandTable[Command::BTN_ACCEL]		= PAD_INPUT_1;
	// バック
	m_commandTable[Command::BTN_BACK]		= PAD_INPUT_2;
	// ジャンプ
	m_commandTable[Command::BTN_JUMP]		= PAD_INPUT_6;
	// ドリフト
	m_commandTable[Command::BTN_DRIFT]		= PAD_INPUT_6;
	// アイテム
	m_commandTable[Command::BTN_ITEM]		= PAD_INPUT_5;
}

Input& Input::GetInstance()
{
	static Input input;
	return input;
}

void Input::Init()
{
}

void Input::Update(int no)
{
	// 事前の情報を保存
	m_prePadstate[no] = m_padstate[no];

	// 入力情報の取得
	m_padstate[no] = GetJoypadInputState(GetInputType(no));
	XINPUT_STATE temp;
	GetJoypadXInputState(GetInputType(no), &temp);
	auto& xinput = m_trigger[no];
	xinput.LT = GetTriggerRate(temp.LeftTrigger);
	xinput.RT = GetTriggerRate(temp.RightTrigger);
	xinput.LStick.x = GetStickRate(temp.ThumbLX);
	xinput.LStick.y = GetStickRate(temp.ThumbLY);
	xinput.RStick.x = GetStickRate(temp.ThumbRX);
	xinput.RStick.y = GetStickRate(temp.ThumbRY);

#ifdef _DEBUG
	if (no != 0) return;

	for (int i = 0; i < 256; ++i)
	{
		m_preKeystate[i] = m_keystate[i];
	}
	GetHitKeyStateAll(m_keystate);
#endif
}

bool Input::IsPress(int no, const char* const command) const
{
	const auto& cmd = m_commandTable.at(command);
	return m_padstate.at(no) & cmd;
}

bool Input::IsTriggerd(int no, const char* const command) const
{
	const auto& cmd = m_commandTable.at(command);
	return ((m_padstate.at(no)) & cmd) && (!(m_prePadstate.at(no) & cmd));
}

bool Input::IsReleased(int no, const char* const command) const
{
	const auto& cmd = m_commandTable.at(command);
	return !(m_padstate.at(no) & cmd) && (m_prePadstate.at(no) & cmd);
}

float Input::GetLT(int no) const
{
	return m_trigger.at(no).LT;
}

float Input::GetRT(int no) const
{
	return m_trigger.at(no).RT;
}

const Vec2& Input::GetLStick(int no) const
{
	return m_trigger.at(no).LStick;
}

const Vec2& Input::GetRStick(int no) const
{
	return m_trigger.at(no).RStick;
}

#ifdef _DEBUG
bool Input::IsPress(int key) const
{
	return m_keystate[key];
}

bool Input::IsTriggerd(int key) const
{
	return m_keystate[key] && !m_preKeystate[key];
}
#endif

int Input::GetInputType(int no)
{
	int inputType;

	// 1番目
	if (no == 0)
	{
		inputType = DX_INPUT_PAD1;
	}
	// 2番目
	else if (no == 1)
	{
		inputType = DX_INPUT_PAD2;
	}
	// 3番目
	else if (no == 2)
	{
		inputType = DX_INPUT_PAD3;
	}
	// 4番目
	else if (no == 3)
	{
		inputType = DX_INPUT_PAD4;
	}
	// 5番目
	else if (no == 4)
	{
		inputType = DX_INPUT_PAD5;
	}
	// 6番目
	else if (no == 5)
	{
		inputType = DX_INPUT_PAD6;
	}
	// 7番目
	else if (no == 6)
	{
		inputType = DX_INPUT_PAD7;
	}
	// 8番目
	else if (no == 7)
	{
		inputType = DX_INPUT_PAD8;
	}
	// その他の場合
	else
	{
		assert(false);
		inputType = DX_INPUT_PAD1;
	}

	return inputType;
}

float Input::GetTriggerRate(unsigned char trigger)
{
	return ApplyDeadZone(trigger / Game::ANALOG_TRIGGER_RANGE);
}

float Input::GetStickRate(short val)
{
	float rate = val / Game::ANALOG_STICK_RANGE;
	bool isReverse = rate < 0.0f;
	if (isReverse) rate = std::abs(rate);
	rate = ApplyDeadZone(rate);
	if (isReverse) rate *= -1;
	return rate;
}

float Input::ApplyDeadZone(float rate)
{
	rate = (rate - Game::ANALOG_DEAD_ZONE_INSIDE) / (Game::ANALOG_DEAD_ZONE_OUTSIDE - Game::ANALOG_DEAD_ZONE_INSIDE);
	rate = std::min<float>(std::max<float>(rate, 0.0f), 1.0f);

	return rate;
}
