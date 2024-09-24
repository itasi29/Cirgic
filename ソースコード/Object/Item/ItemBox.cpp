#include "ItemBox.h"
#include <EffekseerForDXLib.h>
#include "ItemManager.h"
#include "Common/Camera.h"
#include "Geometry/Matrix4x4.h"
#include "File/FileID.h"
#include "File/FileBase.h"
#include "File/FileManager.h"
#include "MyEngine/Physics/Physics.h"
#include "MyEngine/Physics/ColliderSphere.h"

namespace
{
	constexpr float COLLISION_RADIUS = 30.0f;
}

ItemBox::ItemBox(ItemManager& itemMgr) :
	Object3D(MyEngine::Collidable::Priority::STATIC, ObjectTag::ITEM_BOX),
	m_itemMgr(itemMgr),
	m_isEnd(false)
{
}

void ItemBox::Init(const Vec3& pos, const Quaternion& rot)
{
	auto& physics = MyEngine::Physics::GetInstance();
	physics.Entry(shared_from_this());

	m_rigid.SetPos(pos);
	m_rigid.SetVelocity(Vec3());
	m_rigid.SetGravity(false);
	m_rotation = rot;
	m_scale = Vec3(0.015f, 0.015f, 0.015f);

	auto sphere = std::dynamic_pointer_cast<MyEngine::ColliderSphere>(CreateCollider(MyEngine::ColliderBase::Kind::Sphere));
	sphere->center = Vec3();
	sphere->radius = COLLISION_RADIUS;
	sphere->isTrigger = true;

	auto& fileMgr = FileManager::GetInstance();
	m_model = fileMgr.Load(FileID::Mdl::CRYSTAL);
	m_modelH = MV1DuplicateModel(m_model->GetHandle());

	auto mat = m_rotation.GetMat() * Scale(m_scale) * Pos(m_rigid.GetPos());
	MV1SetMatrix(m_modelH, mat.GetMATRIX());
}

void ItemBox::End()
{
	if (m_isEnd) return;

	auto& physics = MyEngine::Physics::GetInstance();
	physics.Exit(shared_from_this());

	m_isEnd = true;
	m_itemMgr.RecreateBox(m_rigid.GetPos(), m_rotation);
}

void ItemBox::Update()
{
	m_rotation = m_rotation * AngleAxis(1.5f, Vec3::Up());

	auto mat = m_rotation.GetMat() * Scale(m_scale) * Pos(m_rigid.GetPos());
	MV1SetMatrix(m_modelH, mat.GetMATRIX());
}

void ItemBox::Draw() const
{
	MV1DrawModel(m_modelH);
}

void ItemBox::OnTriggerEnter(Collidable* colider)
{
	auto tag = colider->GetTag();

	if (tag == ObjectTag::CAR)
	{
		End();
	}
}
