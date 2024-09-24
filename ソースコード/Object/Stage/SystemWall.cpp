#include "SystemWall.h"
#include "MyEngine/Physics/ColliderTerrain.h"
#include "MyEngine/Physics/Physics.h"
#include "MyEngine/DebugDraw.h"
#include "Common/Game.h"

SystemWall::SystemWall() :
	Object3D(MyEngine::Collidable::Priority::STATIC, ObjectTag::SYSTEM_WALL)
{
	m_rigid.SetGravity(false);
#if false
	m_terrain = dynamic_cast<MyEngine::ColliderTerrain*>(AddCollider(MyEngine::ColliderBase::Kind::Terrain).get());
#endif
	m_terrain = dynamic_cast<MyEngine::ColliderTerrain*>(CreateCollider(MyEngine::ColliderBase::Kind::Terrain).get());
	m_terrain->norm = Vec3::Up();
	m_terrain->rotation = AngleAxis(0, m_terrain->norm);
	m_terrain->size = Vec2(0, 0);
}

void SystemWall::Init(const Vec3& pos, const Quaternion& rot, const Vec3& norm, float width, float height)
{
	m_rigid.SetPos(pos);
	m_terrain->Init(Vec3(), rot, norm, height, width, false);
	m_terrain->SetMinMax(pos);
}

void SystemWall::Draw() const
{
	auto& pos = m_rigid.GetPos();
	auto& data = m_terrain;
#ifdef _DEBUG
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawTerrain(pos + data->center, data->norm, data->size, data->rotation, 0x888888, false);
#endif
}
