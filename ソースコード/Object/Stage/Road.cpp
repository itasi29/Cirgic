#include "Road.h"
#include "MyEngine/Physics/ColliderTerrain.h"
#include "Common/Game.h"
#include "MyEngine/DebugDraw.h"

namespace
{

}

Road::Road() :
	Object3D(MyEngine::Collidable::Priority::STATIC, ObjectTag::GROUND)
{
	m_rigid.SetGravity(false);
#if false
	m_terrain = dynamic_cast<MyEngine::ColliderTerrain*>(AddCollider(MyEngine::ColliderBase::Kind::Terrain).get());
#endif
	m_terrain = dynamic_cast<MyEngine::ColliderTerrain*>(CreateCollider(MyEngine::ColliderBase::Kind::Terrain).get());
	m_terrain->norm = Vec3::Up();
	m_terrain->rotation = AngleAxis(0, m_terrain->norm);
	m_terrain->size = Vec2(0, 0);

	m_friction = 0.5f;
	m_maxSpeedRate = 1.0f;
}

void Road::Init(const Vec3& pos, const Quaternion& rot, const Vec3& norm, float width, float length, float friction, float rate, unsigned int color)
{
	m_rigid.SetPos(pos);
	m_terrain->Init(Vec3(), rot, norm, width, length, false);
	m_terrain->SetMinMax(pos);

	m_friction = friction;
	m_maxSpeedRate = rate;

	m_color = color;
}

void Road::Draw() const
{
	auto& pos = m_rigid.GetPos();
	auto& data = m_terrain;
#ifdef _DEBUG
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawTerrain(pos + data->center, data->norm, data->size, data->rotation, m_color, false);
#endif
}
