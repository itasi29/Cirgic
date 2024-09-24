#include "ItemCarapaceRed.h"
#include <string>
#include "File/FileID.h"
#include "Geometry/MathHelp.h"
#include "Geometry/Easing.h"
#include "MyEngine/DebugDraw.h"
#include "Object/Car/CarBase.h"
#include "Race/RaceManager.h"

namespace
{
	constexpr float HOMING_DIS = 500.0f * 500.0f;
	constexpr float FRONT_DIS = 60.0f;
	constexpr float SPEED = 20.0f;
}

ItemCarapaceRed::ItemCarapaceRed(ItemManager& itemMgr, const std::weak_ptr<RaceManager>& raceMgr) :
	ItemCarapaceBase(Kind::CARAPACE_RED, itemMgr, FileID::Eff::CARAPCE_RED),
	m_raceMgr(raceMgr),
	m_isHoming(false),
	m_isInHomingDis(false),
	m_homingCar(nullptr)
{
}

ItemCarapaceRed::~ItemCarapaceRed()
{
}

void ItemCarapaceRed::Update()
{
	ItemCarapaceBase::Update();

	// 
	// �R�[�X�̓��ɉ����Đi�܂��遨����͌�ł��(�Ƃ肠�������i����܂܂�)
	// �R���C�_�[��ǉ�����(isTrigger)
	// ���͈͓̔��ɓ�������Car���z�[�~���O������
	// ���R���C�_�[��S��IsTrigger�Ɠ�������Ă邩�狗����
	// �����ɃO��������Ȃ��āA���X�Ɍ�����������
	// 

	// 
	// �R�[�X�̓��ɉ����Đi�܂��遨����͌�ł��(�Ƃ肠�������i����܂܂�)
	// �ړI�̌��݂�CP��ʉ߂�����G�̌��̃��C���Ɉړ�
	// ���̌�͂Ԃ���܂Ō��̃��C�����z�[�~���O
	// 

	if (m_isHoming)
	{
		// �z�[�~���O�������ɓ�����
		if (m_isInHomingDis)
		{
			// �b������ړI�̎Ԃ܂ł̃x�N�g��
			auto thisToCar = m_homingCar->GetPos() - m_rigid.GetPos();
			// �ړ��x�N�g�������X�ɕύX
			auto newVel = Slerp(m_rigid.GetVelocity(), thisToCar, 0.1f);
			m_rigid.SetVelocity(newVel);
			
		}
		// �����Ă��Ȃ����
		else
		{
			// TODO: �R�[�X�ɉ����Đi�܂���

			// �͈͓��ɓ��������̔���
			auto sqLen = (m_rigid.GetPos() - m_homingCar->GetPos()).SqLength();
			m_isInHomingDis = sqLen < HOMING_DIS;
		}
	}
}

void ItemCarapaceRed::Release(CarBase* car, float power)
{
	ItemCarapaceBase::Release(car, power);
	auto front = car->GetFront();
	Vec3 pos;
	Vec3 vel;

	// ���ɓ|���Ă���
	if (power < 0)
	{
		pos = m_rigid.GetPos();
		vel = front * -SPEED;
	}
	// �O�ɓ|���Ă���or�|���Ă��Ȃ�
	else
	{
		pos = car->GetPos() + front * FRONT_DIS;
		vel = front * SPEED;

		// �O�ɔ�΂����Ƃ������z�[�~���O
		// ���ʎ擾
		// 1�ʂȂ�z�[�~���O���Ȃ�
		// ����ȏ�̏��ʂȂ�z�[�~���O����
		// �z�[�~���O����Ԃ̏��擾
		// 
		auto standing = m_raceMgr.lock()->GetStanding(car->GetCarNo());
		if (standing != 1)
		{
			m_homingCar = m_raceMgr.lock()->GetCarToStanding(standing - 1);
			m_isHoming = true;
			m_isInHomingDis = false;
		}

		
	}

	m_rigid.SetPos(pos);
	m_rigid.SetVelocity(vel);
	m_rigid.SetGravity(true);
}

void ItemCarapaceRed::OnCollideEnter(Collidable* colider)
{
	ItemCarapaceBase::OnCollideEnter(colider);
#ifdef _DEBUG
	std::wstring message = L"Collide: �ԍb����";
#endif

	auto kind = colider->GetTag();
	if (kind == ObjectTag::SYSTEM_WALL)
	{
#ifdef _DEBUG
		message += L"�V�X�e����";
#endif

		End();
	}

#ifdef _DEBUG
	message += L"�Ɠ�������\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}
