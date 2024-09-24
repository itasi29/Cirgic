#include "JumpPoint.h"
#include "MyEngine/Physics/ColliderCapsule.h"

JumpPoint::JumpPoint() :
	Object3D(MyEngine::Collidable::Priority::STATIC, ObjectTag::JUMP_POINT)
{
	m_rigid.SetGravity(false);
	
	m_rigid.SetPos(Vec3(0, 25, 0));

#if false
	m_capsule = dynamic_cast<MyEngine::ColliderCapsule*>(AddCollider(MyEngine::ColliderBase::Kind::Capsule).get());
#endif
	m_capsule = dynamic_cast<MyEngine::ColliderCapsule*>(CreateCollider(MyEngine::ColliderBase::Kind::Capsule).get());
	m_capsule->Init(Vec3(), Vec3::Front(), 15.0f, 10.0f, true);
	m_capsule->SetMinMax(m_rigid.GetPos());
}