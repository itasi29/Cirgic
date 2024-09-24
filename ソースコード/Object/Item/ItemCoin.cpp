#include "ItemCoin.h"
#include "ItemManager.h"
#include "Common/SoundSystem.h"
#include "Object/Car/CarBase.h"
#include "MyEngine/Physics/ColliderSphere.h"
#include "File/FileBase.h"
#include "File/FileManager.h"
#include "File/FileID.h"

namespace
{
	constexpr int ROTATION_ANGLE_MODEL = 3;
}

ItemCoin::ItemCoin(ItemManager& itemMgr) :
	ItemBase(Kind::COIN, itemMgr),
	m_isEntiry(false)
{
	auto& fileMgr = FileManager::GetInstance();
	m_se = fileMgr.Load(FileID::Snd::SE::COIN);
	m_model = fileMgr.Load(FileID::Mdl::COIN);
	m_modelH = MV1DuplicateModel(m_model->GetHandle());
}

ItemCoin::~ItemCoin()
{
}

void ItemCoin::Update()
{
	
	// Žc‚è‘±‚¯‚é‚æ‚¤‚Éoverride‚µ‚½‚¾‚¯
	m_rotation = m_rotation * AngleAxis(ROTATION_ANGLE_MODEL, Vec3::Up());
	
	MV1SetRotationMatrix(m_modelH, m_rotation.GetMat().GetMATRIX());
}

void ItemCoin::End()
{
	ItemBase::End();

	if (!m_isEntiry) return;
	m_itemMgr.RecreateCoin(m_rigid.GetPos(), m_scale);
	m_isEntiry = false;
}

void ItemCoin::Use(CarBase* car)
{
	car->GetCoin();
	auto& sndSys = SoundSystem::GetInstance();
	sndSys.PlaySe(m_se->GetHandle());
}

void ItemCoin::Hit(CarBase* car)
{
	Use(car);
}

void ItemCoin::Draw() const
{
//	DrawSphere3D(m_rigid.GetPos().VGet(), m_sphere->radius, 16, 0xff0000, 0, false);
	MV1DrawModel(m_modelH);
}

void ItemCoin::OnEntiry(float radius, const Vec3& pos, const Vec3& scale)
{
	if (m_isEntiry) return;
	ItemBase::OnEntity(radius);

	m_isEntiry = true;

	m_rigid.SetPos(pos);
	m_rigid.SetVelocity(Vec3());
	m_rigid.SetGravity(false);
	m_scale = scale;

	MV1SetPosition(m_modelH, pos.VGet());
	MV1SetScale(m_modelH, scale.VGet());
}
