#include "Object3D.h"
#include <DxLib.h>
#include "File/FileManager.h"
#include "File/FileBase.h"
#include "MyEngine/Physics/Physics.h"

Object3D::Object3D(Collidable::Priority priority, ObjectTag tag) :
	Collidable(priority, tag),
	m_modelH(-1),
	m_meshNum(-1),
	m_isEntry(false)
{
	m_rotation = AngleAxis(0, Vec3::Up());
	m_front = Vec3::Front();
	m_right = Vec3::Right();
	m_top = Cross(m_front, m_right);
	m_cameraFront = m_front;
}

Object3D::Object3D(Collidable::Priority priority, ObjectTag tag, const wchar_t* const path) :
	Collidable(priority, tag),
	m_isEntry(false)
{
	auto& fileMgr = FileManager::GetInstance();
	m_model = fileMgr.Load(path);
	m_modelH = MV1DuplicateModel(m_model->GetHandle());
	m_meshNum = MV1GetMeshNum(m_modelH);

	m_rotation = AngleAxis(0, Vec3::Up());
	m_front = Vec3::Front();
	m_right = Vec3::Right();
	m_top = Cross(m_front, m_right);
	m_cameraFront = m_front;
}

Object3D::~Object3D()
{
	if (m_modelH > 0)
	{
		MV1DeleteModel(m_modelH);
		m_modelH = -1;
	}
}

void Object3D::Entry()
{
	auto& physics = MyEngine::Physics::GetInstance();
	physics.Entry(shared_from_this());
	m_isEntry = true;
}

void Object3D::Init(const Vec3& pos, const Quaternion& rot, const Vec3& size)
{
	m_rigid.SetPos(pos);
	m_rotation = rot;
	m_scale = size;

	Matrix4x4 mat;
	mat = Scale(size) * Rot(rot) * Pos(pos);

	MV1SetMatrix(m_modelH, mat.GetMATRIX());
}

void Object3D::End()
{
	MV1DeleteModel(m_modelH);
	m_modelH = -1;

	if (m_isEntry)
	{
		auto& physics = MyEngine::Physics::GetInstance();
		physics.Exit(shared_from_this());
	}
}

void Object3D::Draw() const
{
	if (m_modelH < 0) return;

	MV1DrawModel(m_modelH);
}
