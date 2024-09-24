#include "ItemCarapaceBase.h"
#include <EffekseerForDXLib.h>
#include "Object/Car/CarBase.h"
#include "File/FileBase.h"
#include "File/FileManager.h"

namespace
{
	constexpr float FRONT_DIS = 60.0f;
	constexpr float BACK_DIS = 15.0f;
	constexpr float SPEED = 20.0f;
}

ItemCarapaceBase::ItemCarapaceBase(Kind kind, ItemManager& itemMgr, const wchar_t* const path) :
	ItemBase(kind, itemMgr)
{
	auto& mgr = FileManager::GetInstance();
}

ItemCarapaceBase::~ItemCarapaceBase()
{
}

void ItemCarapaceBase::Update()
{
	ItemBase::Update();

	const auto& pos = m_rigid.GetPos();
}

void ItemCarapaceBase::Draw() const
{
	DrawSphere3D(m_rigid.GetPos().VGet(), 10.0f, 16, 0xff0000, 0, false);
}

void ItemCarapaceBase::Use(CarBase* car)
{
	car->OnHoldItem();
	OnEntity(10.0f);
}

void ItemCarapaceBase::Hit(CarBase* car)
{
	car->OnDamage();
}

void ItemCarapaceBase::Release(CarBase* car, float power)
{
	ItemBase::Release(car, power);
	auto front = car->GetFront();
	Vec3 pos;
	Vec3 vel;

	// Œã‚ë‚É“|‚µ‚Ä‚¢‚é
	if (power < 0)
	{
		pos = m_rigid.GetPos() - front * BACK_DIS;
		vel = front * -SPEED;
	}
	// ‘O‚É“|‚µ‚Ä‚¢‚éor“|‚µ‚Ä‚¢‚È‚¢
	else
	{
		pos = car->GetPos() + front * FRONT_DIS;
		vel = front * SPEED;
	}

	pos.y += 5.0f;

	m_rigid.SetPos(pos);
	m_rigid.SetVelocity(vel);
	m_rigid.SetGravity(false);
}

void ItemCarapaceBase::OnCollideEnter(Collidable* colider)
{
	ItemBase::OnCollideEnter(colider);
}
