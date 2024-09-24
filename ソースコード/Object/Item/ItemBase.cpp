#include "ItemBase.h"
#include <string>
#include "ItemManager.h"
#include "MyEngine/DebugDraw.h"
#include "MyEngine/Physics/Physics.h"
#include "MyEngine/Physics/ColliderSphere.h"
#include "MyEngine/Physics/ColliderTerrain.h"

namespace
{
	// 実体化しているフレーム　だいたい5分
	constexpr int EXIST_FRAME = 50 * 60 * 5;
}

ItemBase::ItemBase(Kind kind, ItemManager& itemMgr) :
	Object3D(MyEngine::Collidable::Priority::MIDDLE, ObjectTag::ITEM),
	m_kind(kind),
	m_itemMgr(itemMgr),
	m_existCount(0),
	m_isCountExist(false),
	m_isEntryPhysics(false),
	m_isDestoyEntity(false),
	m_isDraw(false)
{
}

ItemBase::~ItemBase()
{
}

void ItemBase::Update()
{
	if (!m_isCountExist) return;

	++m_existCount;
	if (m_existCount > EXIST_FRAME)
	{
		m_isDestoyEntity = true;
	}
}

void ItemBase::Draw() const
{
}

void ItemBase::End()
{
	if (!m_isEntryPhysics) return;
	
	auto& physics = MyEngine::Physics::GetInstance();
	physics.Exit(shared_from_this());
	m_isEntryPhysics = false;
	m_isDestoyEntity = true;
}

void ItemBase::Release(CarBase* car, float power)
{
	m_isCountExist = true;
}

void ItemBase::UpdateEntityPos(const Vec3& pos)
{
	m_rigid.SetPos(pos);
}

void ItemBase::OnCollideEnter(Collidable* colider)
{
#ifdef _DEBUG
	std::wstring message = L"Collide: アイテムが";
#endif

	auto kind = colider->GetTag();
	if (kind == ObjectTag::SYSTEM_WALL)
	{
#ifdef _DEBUG
		message += L"システム壁";
#endif
		auto terrain = dynamic_cast<MyEngine::ColliderTerrain*>(colider->GetColliderData());
		auto vel = m_rigid.GetVelocity();

		auto refVel = Reflection(vel, terrain->norm);
		m_rigid.SetVelocity(refVel);
	}

#ifdef _DEBUG
	message += L"と当たった\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}

void ItemBase::OnEntity(float radius)
{
	if (m_isEntryPhysics) return;
	auto& physics = MyEngine::Physics::GetInstance();
	physics.Entry(shared_from_this());

	// TODO: アイテムのモデルをロード

	m_rigid.SetGravity(false);
	m_throughTag.push_back(ObjectTag::CAR);

#if false
	auto sphere = dynamic_cast<MyEngine::ColliderSphere*>(AddCollider(MyEngine::ColliderBase::Kind::Sphere).get());
#endif
	m_sphere = std::dynamic_pointer_cast<MyEngine::ColliderSphere>(CreateCollider(MyEngine::ColliderBase::Kind::Sphere));
	m_sphere->center = Vec3();
	m_sphere->radius = radius;
	m_sphere->isTrigger = false;
	
	m_isEntryPhysics = true;
	m_itemMgr.AddEntity(shared_from_this());
}
