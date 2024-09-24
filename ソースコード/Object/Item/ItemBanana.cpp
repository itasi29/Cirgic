#include "ItemBanana.h"
#include "Object/Car/CarBase.h"
#include "Common/Input.h"
#include "MyEngine/DebugDraw.h"

ItemBanana::ItemBanana(ItemManager& itemMgr) :
	ItemBase(Kind::BANANA, itemMgr)
{
}

ItemBanana::~ItemBanana()
{
}

void ItemBanana::Use(CarBase* car)
{
	car->OnHoldItem();
	OnEntity(15.0f);
}

void ItemBanana::Hit(CarBase* car)
{
	car->OnDamage();
}

void ItemBanana::Release(CarBase* car, float power)
{
	ItemBase::Release(car, power);
	// 後ろに倒している
	if (power < 0)
	{
		// 動作としては何もしない
	}
	// 前に倒しているor倒していない
	else
	{
		auto pos = car->GetPos();
		auto top = car->GetTop();
		auto front = car->GetFront();
		m_rigid.SetPos(pos + top * 50.0f + front * - 50.0f);
		auto& rot = car->GetRot();
		auto vel = rot * AngleAxis(30, Vec3::Right()) * -Vec3::Front() * 15.0f;
		m_rigid.SetVelocity(vel);
		m_rigid.SetGravity(true);
	}
}

void ItemBanana::OnCollideEnter(Collidable* colider)
{
	ItemBase::OnCollideEnter(colider);
#ifdef _DEBUG
	std::wstring message = L"Collide: バナナが";
#endif

	auto kind = colider->GetTag();
	if (kind == ObjectTag::GROUND)
	{
#ifdef _DEBUG
		message += L"地面と";
#endif
		m_rigid.SetVelocity(Vec3());
	}

#ifdef _DEBUG
	message += L"と当たった\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}
