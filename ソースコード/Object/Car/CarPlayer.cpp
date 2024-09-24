#include "CarPlayer.h"
#include "Common/Input.h"
#include "Common/Game.h"
#include "Common/Camera.h"
#include "Common/CommandList.h"
#include "File/FileBase.h"
#include "File/FileManager.h"
#include "Object/Item/ItemManager.h"

namespace
{
	constexpr float ADD_FAR = 1000.0f;
	constexpr int CAMERA_FOV_DASH = 120;
}

CarPlayer::CarPlayer(ItemManager& itemMgr, CheckPointManager& cpMgr, const Vec3& pos) :
	CarBase(itemMgr, cpMgr, pos),
	m_padNo(0)
{
}

void CarPlayer::InitPadNo(int no)
{
	m_padNo = no;

	auto& camera = Camera::GetInstance();
	m_cameraFront = m_front;
	camera.SetNearFar(m_padNo, Game::CAMERA_NEAR, Game::CAMERA_FAR);
	camera.SetPos(m_padNo, m_rigid.GetPos(), m_cameraFront);
}

void CarPlayer::Update()
{
	auto& input = Input::GetInstance();
	m_handle = input.GetLStick(m_padNo).x;
	m_throwPower = input.GetLStick(m_padNo).y;
	m_isDriftStart = input.IsPress(m_padNo, Command::BTN_DRIFT);
	m_isDriftEnd = !input.IsPress(m_padNo, Command::BTN_DRIFT);
	if (m_holdItem)
	{
		m_isReleaseItem = !input.IsPress(m_padNo, Command::BTN_ITEM);
	}

	if (m_isGround)
	{
		Decel();

		if (!m_isStop)
		{
			if (input.IsPress(m_padNo, Command::BTN_ACCEL))
			{
				Accel();
			}
			if (input.IsPress(m_padNo, Command::BTN_BACK))
			{
				Back();
			}
			if (input.IsTriggerd(m_padNo, Command::BTN_JUMP))
			{
				JumpAction();
			}
			Drift();
			Handle();
			Slipstream();
		}
	}
	if (input.IsTriggerd(m_padNo, Command::BTN_ITEM) && !m_isStop)
	{
		OnUseItem();
	}
	
	CarBase::Update();
	
	auto& camera = Camera::GetInstance();
	m_cameraFront = m_front;
	camera.SetNearFar(m_padNo, Game::CAMERA_NEAR, Game::CAMERA_FAR + ADD_FAR);
	camera.SetPos(m_padNo, m_rigid.GetPos(), m_cameraFront);

#ifdef _DEBUG
	if (input.IsTriggerd(KEY_INPUT_I))
	{
		if (!m_hasItem)
		{
			m_hasItem = m_itemMgr.GetItem(m_carNo);
		}
	}
#endif
}

void CarPlayer::Draw() const
{
	CarBase::Draw();
}

void CarPlayer::OnDash(float dash)
{
	CarBase::OnDash(dash);

	if (m_isDashEff)
	{
		auto& camera = Camera::GetInstance();
		camera.SetFov(m_padNo, CAMERA_FOV_DASH, m_dashEffTime);
	}

}
