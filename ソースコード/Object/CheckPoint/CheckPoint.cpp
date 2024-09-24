#include "CheckPoint.h"
#include "Common/Game.h"
#include "MyEngine/Physics/ColliderTerrain.h"
#include "MyEngine/Physics/Physics.h"
#include "MyEngine/DebugDraw.h"

CheckPoint::CheckPoint() :
	Collidable(MyEngine::Collidable::Priority::STATIC, ObjectTag::CHECK_POINT),
	m_no(-1),
	m_kind(Kind::NORMAL),
	m_terrain(nullptr)
{
}

CheckPoint::~CheckPoint()
{
}


void CheckPoint::Init(const Vec3& pos, const Quaternion& rot, const Vec3& norm, float width, float height, Kind kind, int no)
{
	m_kind = kind;
	m_no = no;

	m_rigid.SetGravity(false);
	m_rigid.SetPos(pos);

#if false
	m_terrain = dynamic_cast<MyEngine::ColliderTerrain*>(AddCollider(MyEngine::ColliderBase::Kind::Terrain).get());
#endif
	m_terrain = dynamic_cast<MyEngine::ColliderTerrain*>(CreateCollider(MyEngine::ColliderBase::Kind::Terrain).get());
	m_terrain->Init(pos, rot, norm, width, height, true);

	auto& physics = MyEngine::Physics::GetInstance();
	physics.Entry(shared_from_this());
}

void CheckPoint::End()
{
	auto& physics = MyEngine::Physics::GetInstance();
	physics.Exit(shared_from_this());
}

void CheckPoint::DebugDraw() const
{
	auto& pos = m_rigid.GetPos();
	auto& data = m_terrain;
#ifdef _DEBUG
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawTerrain(pos + data->center, data->norm, data->size, data->rotation, 0x880088, false);
#endif
}

const Vec3& CheckPoint::GetPos() const
{
	return m_rigid.GetPos();
}
