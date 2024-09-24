#include "Collidable.h"
#include <cassert>
#include "Colliders.h"
#include "Geometry/Vec3.h"

using namespace MyEngine;

Collidable::Collidable(Priority priority, ObjectTag tag) :
	m_priority(priority),
	m_tag(tag)
{
}

Collidable::~Collidable()
{
}

#if false
ColliderBase* MyEngine::Collidable::GetColliderData(int index) const
{
	auto size = m_colliders.size();
	if (index >= size)
	{
		assert(false && "配列参照外アクセスです");
		return nullptr;
	}

	int i = 0;
	for (const auto& col : m_colliders)
	{
		if (i == index)
		{
			return col.get();
		}
		++i;
	}

	return m_colliders.end()->get();
}
#endif

ColliderBase* MyEngine::Collidable::GetColliderData() const
{
	return m_collider.get();
}

const Vec3& MyEngine::Collidable::GetPos() const
{
	return m_rigid.GetPos();
}

void Collidable::AddForce(const Vec3& dir, float power)
{
	m_rigid.AddForce(dir, power);
}

#if false
std::shared_ptr<ColliderBase> MyEngine::Collidable::AddCollider(const ColliderBase::Kind& kind)
{
	std::shared_ptr<ColliderBase> collider;

	if (kind == ColliderBase::Kind::Sphere)
	{
		collider = std::make_shared<ColliderSphere>();
	}
	else if (kind == ColliderBase::Kind::Capsule)
	{
		collider = std::make_shared<ColliderCapsule>();
	}
	else if (kind == ColliderBase::Kind::Terrain)
	{
		collider = std::make_shared<ColliderTerrain>();
	}
	else if (kind == ColliderBase::Kind::Box)
	{
		collider = std::make_shared<ColliderBox>();
	}

	if (!collider)
	{
		assert(false);
		collider = std::make_shared<ColliderSphere>();
	}

	m_colliders.emplace_back(collider);

	return collider;
}
#endif

std::shared_ptr<ColliderBase> MyEngine::Collidable::CreateCollider(const ColliderBase::Kind& kind)
{
	if (kind == ColliderBase::Kind::Sphere)
	{
		m_collider = std::make_shared<ColliderSphere>();
	}
	else if (kind == ColliderBase::Kind::Capsule)
	{
		m_collider = std::make_shared<ColliderCapsule>();
	}
	else if (kind == ColliderBase::Kind::Terrain)
	{
		m_collider = std::make_shared<ColliderTerrain>();
	}
	else if (kind == ColliderBase::Kind::Box)
	{
		m_collider = std::make_shared<ColliderBox>();
	}

	if (!m_collider)
	{
		assert(false);
		m_collider = std::make_shared<ColliderSphere>();
	}

	return m_collider;
}
