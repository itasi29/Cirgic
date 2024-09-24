#include "CarBase.h"
#include <EffekseerForDXLib.h>
#include <cmath>
#include "Common/Application.h"
#include "Common/SoundSystem.h"
#include "Common/Game.h"
#include "Geometry/Quaternion.h"
#include "Geometry/Matrix4x4.h"
#include "Geometry/Easing.h"
#include "Geometry/MathHelp.h"
#include "File/FileID.h"
#include "File/FileBase.h"
#include "File/FileManager.h"
#include "MyEngine/Physics/Collidable.h"
#include "MyEngine/Physics/ColliderSphere.h"
#include "MyEngine/Physics/ColliderTerrain.h"
#include "MyEngine/Physics/ColliderCapsule.h"
#include "MyEngine/DebugDraw.h"
#include "Object/Stage/Road.h"
#include "Object/CheckPoint/CheckPoint.h"
#include "Object/CheckPoint/CheckPointManager.h"
#include "Object/Item/ItemBase.h"
#include "Object/Item/ItemKinokoTriple.h"
#include "Object/Item/ItemManager.h"

namespace
{
	constexpr float JUMP_POWER = 2.0f;

	constexpr float HANDLE_ANGLE = 0.6f;

	// ���x�֌W
	constexpr float DEF_ACCEL = 0.18f;
	constexpr float DEF_SPEED_MAX = 12.48f;
	constexpr float DEF_SPEED_MIN = -6.33f;
	constexpr float DASH_MAX_RATE = 1.75f * 1.75f;

	// ���l�ȉ��𑬓x�[���Ƃ���
	constexpr float MIN_SPEED_CONSTANT = 0.0001f;

	// �h���t�g�֌W
	constexpr float DRIFT_START_SPEED = 9.0f;
	constexpr float DRIFT_END_SPEED = 9.0f;
	constexpr float DRIFT_OBJ_ANGLE = 5;
	constexpr float DRIFT_ACCEL_ANGLE = 0.7f;
	constexpr float DRIFT_TURBO_MAGNIFICATTION = 1.5f;
	constexpr int DRIFT_TURBO_INTERVAL = 180;
	constexpr int DRIFT_TURBO_START_BOOST = 60;
	constexpr int DRIFT_TURBO_MAX_LEVEL = 8;

	// �X���b�v�X�g���[���֌W
	constexpr float SLIPSTREAM_RATE_MAX = 1.2f;
	constexpr float SLIPSTREAM_ADD = 0.001f;
	constexpr float SLIPSTREAM_SUB = 0.002f;

	// �W�����v�A�N�V�����֌W
	constexpr float JUMP_BOOST_SPEED = 5.0f;

	// �_���[�W�����֌W
	constexpr int DAMAGE_FRAME_INVINCIBLE = 90;
	constexpr int DAMAGE_FRAME_STOP = 30;

	// �G�t�F�N�g�֌W
	constexpr int EFF_FRAME_DASH = 90;
	constexpr int EFF_FRAME_DASH_NEXT = 180;
	constexpr float EFF_SMOKE_SPEED_RATE = 0.7f;
	constexpr int EFF_FRAME_SMOKE = 5;
	constexpr int EFF_EXIST_SMOKE = 60;
	constexpr float EFF_SMOKE_CREATE_RANGE = 25.0f;
	constexpr float EFF_SMOKE_MOVE_RATE = 0.1f;
	constexpr int EFF_DRIFT_FRONT_DIS = -45;
	constexpr int EFF_DRIFT_RIGHT_DIS = 15;
	constexpr int EFF_DASH_FRONT_DIS = 110;

	// �����n
	constexpr float COIN_SPEED_BUFFER_RATE = 0.2f;

	// �A�C�e���n
	constexpr float ITEM_HOLD_DISTANACE = 75.0f;

	// �n���h���ɂ��^�C���̉�]�
	constexpr int WHEEL_ROT_ANGLE = 30;
}

CarBase::CarBase(ItemManager& itemMgr, CheckPointManager& cpMgr, const Vec3& pos) :
	Object3D(MyEngine::Collidable::Priority::HIGH, ObjectTag::CAR, FileID::Mdl::CAR),
	m_itemMgr(itemMgr),
	m_cpMgr(cpMgr),
	/* ����֌W */
	m_handle(0),
	m_throwPower(0),
	m_isDriftStart(false),
	m_isDriftEnd(false),
	m_isReleaseItem(false),
	m_isJump(false),
	m_isInJP(false),
	m_isJPDash(false),
	m_isDrift(false),
	m_isDriftRightTurn(false),
	/* �_���[�W�֌W */
	m_invincibleFrame(0),
	m_isInvincible(false),
	m_isStop(false),
	m_stopFrame(0),
	/* ���x�֌W������ */
	m_accelDir(m_front),
	m_accel(DEF_ACCEL),
	m_maxSpeed(DEF_SPEED_MAX* DEF_SPEED_MAX),
	m_minSpeed(DEF_SPEED_MIN* DEF_SPEED_MIN),
	m_maxSpeedRate(1.0f),
	m_drag(1.0f),
	m_turbo(0),
	m_friction(0.5f),
	m_standingSpeedRate(1.0f),
	/* �A�C�e���֌W */
	m_coinNum(0),
	m_hasItem(nullptr),
	m_holdItem(nullptr),
	m_isHoldItem(false),
	/* ���o�֌W */
	m_wheelRotation(Quaternion(0, 0, 0, 1)),
	m_preWheelRotation(m_wheelRotation),
	m_smokeEffFrame(0),
	m_driftEffFrme(0),
	m_dashEffFrame(0),
	m_dashEffTime(0),
	m_rightDriftPlayEffH(0),
	m_leftDriftPlayEffH(0),
	m_dashPlayEffHandle(0),
	m_isDashEff(false),
	m_isMiniDriftEff(false),
	m_isDrifftEff(false),
	/* ���̑� */
	m_isGround(false),
	m_groundNorm(Vec3::Up()),
	m_carNo(0),
	m_isAllGoal(false)
{
	// ���f���ɑ΂���
	m_rigid.SetGravity(true);
	m_rigid.Init(pos);
	m_scale = Vec3(0.01f, 0.01f, 0.01f);
	m_rotation = AngleAxis(180, Vec3::Up());

	// ����
	m_col = dynamic_cast<MyEngine::ColliderCapsule*>(CreateCollider(MyEngine::ColliderBase::Kind::Capsule).get());
	m_col->Init(Vec3(0, 17.0f, 0), m_front, 18.0f, 45.0f, false);
	m_col->SetMinMax(m_rigid.GetPos());

	// �A�C�e�����X���[�I�u�W�F�N�g�ɕύX
	m_throughTag.push_back(ObjectTag::ITEM);

	// �f�[�^�̎擾
	auto& fileMgr = FileManager::GetInstance();
	m_engineSe = DuplicateSoundMem(fileMgr.Load(FileID::Snd::SE::CAR_ENGINE)->GetHandle());
	m_driftSe = DuplicateSoundMem(fileMgr.Load(FileID::Snd::SE::DRIFT)->GetHandle());
	m_dashSe = fileMgr.Load(FileID::Snd::SE::DASH);
	m_miniDriftEffFile = fileMgr.Load(FileID::Eff::DRIFET_SMALE);
	m_driftEffFile = fileMgr.Load(FileID::Eff::DRIFET_LARGE);
	m_smokeEffFile = fileMgr.Load(FileID::Eff::SMOKE);
	m_dashEffFile = fileMgr.Load(FileID::Eff::DASH);
	m_wheelFLFrameH = MV1SearchFrame(m_modelH, L"FrontLeftWheel");
	m_wheelFRFrameH = MV1SearchFrame(m_modelH, L"FrontRightWheel");
}

CarBase::~CarBase()
{
}

const Vec3& CarBase::GetPos() const
{
	return m_rigid.GetPos();
}

bool CarBase::HasItem() const
{
	return m_hasItem != nullptr;
}

ItemBase::Kind CarBase::GetItemKind() const
{
	return m_hasItem->GetKind();
}

void CarBase::GetCoin()
{
	m_coinNum = std::min<int>(m_coinNum + Game::COIN_GET_NUM, Game::COIN_MAX);
}

void CarBase::InitCarNo(int no)
{
	m_carNo = no;

	// �ԍ��ɍ��킹�ăe�N�X�`���̕ύX
	auto& fileMgr = FileManager::GetInstance();
	if (no == 0)
	{
		m_textImg = fileMgr.Load(FileID::Img::TEX_CAR_RED);
	}
	else if (no == 1)
	{
		m_textImg = fileMgr.Load(FileID::Img::TEX_CAR_BULE);
	}
	else if (no == 2)
	{
		m_textImg = fileMgr.Load(FileID::Img::TEX_CAR_YELLOW);
	}
	else if (no == 3)
	{
		m_textImg = fileMgr.Load(FileID::Img::TEX_CAR_PURPLE);
	}
	int texIdx = MV1GetMaterialDifMapTexture(m_modelH, 0);
	MV1SetTextureGraphHandle(m_modelH, texIdx, m_textImg->GetHandle(), false);
}

void CarBase::Update()
{
	HoldItem();
	NormUpdate();
	DamageUpdate();
	UpdateEff();
	UpdateSound();

	UpdateWheel(m_wheelFRFrameH);
	UpdateWheel(m_wheelFLFrameH);
	m_preWheelRotation = m_wheelRotation;
}

void CarBase::Draw() const
{
	MV1SetPosition(m_modelH, m_rigid.GetPos().VGet());
	MV1SetScale(m_modelH, m_scale.VGet());
	MV1SetRotationMatrix(m_modelH, m_rotation.GetMat().GetMATRIX());

	Object3D::Draw();

#ifdef _DEBUG
	auto& debug = MyEngine::DebugDraw::GetInstance();
#if true
	wchar_t str[256];
	swprintf_s(str, L"cp : %d", m_cpMgr.GetNowCpNo(m_carNo));
	debug.DrawString(160, 32, 0xffffff, str);
	auto pos = m_rigid.GetPos();
	swprintf_s(str, L"pos : %.4f, %.4f, %.4f", pos.x, pos.y, pos.z);
	debug.DrawString(160, 32, 0xffffff, str);
	auto speed = m_rigid.GetVelocity();
	swprintf_s(str, L"speed : %.4f, %.4f, %.4f", speed.x, speed.y, speed.z);
	debug.DrawString(160, 32, 0xffffff, str);
	swprintf_s(str, L"speed : %.4f", speed.Length());
	debug.DrawString(160, 32, 0xffffff, str);
	swprintf_s(str, L"invincible : %d", m_invincibleFrame);
	debug.DrawString(160, 32, 0xffffff, str);
#endif

	const std::unordered_map<ItemBase::Kind, std::wstring> STR_ITEM =
	{
		{ ItemBase::Kind::COIN, L"�R�C��" },
		{ ItemBase::Kind::KINOKO, L"�L�m�R" },
		{ ItemBase::Kind::KINOKO_TRIPLE, L"�R�L�m" },
		{ ItemBase::Kind::BANANA, L"�o�i�i" },
		{ ItemBase::Kind::CARAPACE_GREEN, L"�΍b��" },
		{ ItemBase::Kind::CARAPACE_RED, L"�ԍb��" },
	};
	if (m_hasItem)
	{
		auto kind = m_hasItem->GetKind();
		debug.DrawString(160, 32, 0xffffff, L"�����A�C�e���F" + STR_ITEM.at(kind));
	}
	else
	{
		debug.DrawString(160, 32, 0xffffff, L"�����A�C�e���F�����ĂȂ�");
	}
	if (m_holdItem)
	{
		auto kind = m_holdItem->GetKind();
		debug.DrawString(160, 32, 0xffffff, L"�z�[���h�A�C�e���F" + STR_ITEM.at(kind));
	}
	else
	{
		debug.DrawString(160, 32, 0xffffff, L"�z�[���h�A�C�e���F�����ĂȂ�");
	}
#endif
}

void CarBase::End()
{
	Object3D::End();
	// �f�[�^�폜
	if (m_isDashEff)
	{
		StopEffekseer3DEffect(m_dashPlayEffHandle);
	}
	if (m_isMiniDriftEff)
	{
		StopEffekseer3DEffect(m_rightDriftPlayEffH);
		StopEffekseer3DEffect(m_leftDriftPlayEffH);
	}
	DeleteSoundMem(m_engineSe);
	DeleteSoundMem(m_driftSe);
}

/// <summary>
/// �_�b�V��
/// </summary>
/// <param name="dash">�_�b�V�����x</param>
void CarBase::OnDash(float dash)
{
	// �ő呬�x = �ő呬�x * (1 + �R�C���̖��� * 0.02) * ���ʉ��� *  �_�b�V���̍ō����x
	float max = m_maxSpeed * (1.0f + COIN_SPEED_BUFFER_RATE * (m_coinNum / static_cast<float>(Game::COIN_MAX))) * m_standingSpeedRate * DASH_MAX_RATE;
	m_rigid.AddForce(m_front, dash, true, max, true);

	// dash ���Ă���΃T�E���h�炷
	if (dash > 0)
	{
		auto& sndSys = SoundSystem::GetInstance();
		sndSys.PlaySe(m_dashSe->GetHandle());
		if (!m_isDashEff)
		{
			m_dashPlayEffHandle = PlayEffekseer3DEffect(m_dashEffFile->GetHandle());
		}
		m_isDashEff = true;
		m_dashEffFrame = 0;
		m_dashEffTime = EFF_FRAME_DASH;
	}
}

void CarBase::OnHoldItem()
{
	if (m_holdItem) return;

	m_holdItem = m_hasItem;
}

void CarBase::OnDamage()
{
	if (m_isInvincible) return;

	m_stopFrame = DAMAGE_FRAME_STOP;
	m_invincibleFrame = DAMAGE_FRAME_INVINCIBLE;
	m_isStop = true;
	m_isInvincible = true;
}

void CarBase::AllGoal()
{
	m_isAllGoal = true;
	if (m_isDashEff)
	{
		m_isDashEff = false;
		StopEffekseer3DEffect(m_dashPlayEffHandle);
	}
	if (m_isMiniDriftEff)
	{
		m_isMiniDriftEff = false;
		StopEffekseer3DEffect(m_leftDriftPlayEffH);
		StopEffekseer3DEffect(m_rightDriftPlayEffH);
	}
}

void CarBase::OnCollideEnter(Collidable* colider)
{
#ifdef _DEBUG
	std::wstring message = L"Collide: �Ԃ�";
#endif
	auto kind = colider->GetTag();
#ifdef _DEBUG
	if (kind == ObjectTag::CAR)
	{
		message += L"��";
	}
	else if (kind == ObjectTag::GROUND)
#endif
	if (kind == ObjectTag::GROUND)
	{
#ifdef _DEBUG
		message += L"�n��";
#endif
		if (m_isJump)
		{
			m_isJump = false;
			auto vel = m_rigid.GetVelocity();
			vel.y -= JUMP_POWER;
			m_rigid.SetVelocity(vel);
		}
		if (!m_isGround)
		{
			if (m_isJPDash)
			{
				OnDash(JUMP_BOOST_SPEED);
			}
			CheckDriftStart();
		}
		m_isGround = true;
	}
#ifdef _DEBUG
	else if (kind == ObjectTag::JUMP_POINT)
	{
		message += L"�W�����v�|�C���g";
	}
	else if (kind == ObjectTag::SYSTEM_WALL)
	{
		message += L"�V�X�e����";
	}
	else if (kind == ObjectTag::CHECK_POINT)
	{
		message += L"�`�F�b�N�|�C���g";
	}
	else if (kind == ObjectTag::ITEM)
	{
		message += L"�A�C�e��";

	}
	else if (kind == ObjectTag::ITEM_BOX)
	{
		message += L"�A�C�e���{�b�N�X";
	}
	else
	{
		message += L"����";
	}

	message += L"�Ɠ�������\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}

void CarBase::OnCollideStay(Collidable* colider)
{
#ifdef _DEBUG
	std::wstring message = L"Collide: �Ԃ�";
#endif

	auto kind = colider->GetTag();
#ifdef _DEBUG
	if (kind == ObjectTag::CAR)
	{
		message += L"��";
	}
	else if (kind == ObjectTag::GROUND)
#endif
	if (kind == ObjectTag::GROUND)
	{
#ifdef _DEBUG
		message += L"�n��";
#endif
		// �d�͉����x��0��
		m_rigid.ZeroGravity();
		// �n�ʂ̖@�������ɏ������ύX
		auto terrain = dynamic_cast<MyEngine::ColliderTerrain*>(colider->GetColliderData());
		auto temp = Slerp(m_groundNorm, terrain->norm, 1.0f);
		m_groundNorm = temp;

		// ���̍ō����x�ɍ��킹�čō����x������ύX����
		auto stage = dynamic_cast<Road*>(colider);
		m_friction = Lerp(m_friction, stage->GetFriction(), 0.05f);
		auto maxSpeedRate = stage->GetMaxSpeedRate();
		if (maxSpeedRate > 0.9f)
		{
			m_maxSpeedRate = maxSpeedRate;
		}
		else
		{
			m_maxSpeedRate = Lerp(m_maxSpeedRate, maxSpeedRate, 0.05f);
		}
	}
#ifdef _DEBUG
	else if (kind == ObjectTag::JUMP_POINT)
	{
		message += L"�W�����v�|�C���g";
	}
	else if (kind == ObjectTag::SYSTEM_WALL)
	{
		message += L"�V�X�e����";
		Decel();
	}
	else if (kind == ObjectTag::CHECK_POINT)
	{
		message += L"�`�F�b�N�|�C���g";
	}
	else if (kind == ObjectTag::ITEM)
	{
		message += L"�A�C�e��";
	}
	else if (kind == ObjectTag::ITEM_BOX)
	{
		message += L"�A�C�e���{�b�N�X";
	}
	else
	{
		message += L"����";
	}

	message += L"�Ɠ������Ă���\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}

void CarBase::OnCollideExit(Collidable* colider)
{
#ifdef _DEBUG
	std::wstring message = L"Collide: �Ԃ�";

	auto kind = colider->GetTag();
	if (kind == ObjectTag::CAR)
	{
		message += L"��";
	}
	else if (kind == ObjectTag::GROUND)
	{
		message += L"�n��";
	}
	else if (kind == ObjectTag::JUMP_POINT)
	{
		message += L"�W�����v�|�C���g";
	}
	else if (kind == ObjectTag::SYSTEM_WALL)
	{
		message += L"�V�X�e����";
	}
	else if (kind == ObjectTag::CHECK_POINT)
	{
		message += L"�`�F�b�N�|�C���g";
	}
	else if (kind == ObjectTag::ITEM)
	{
		message += L"�A�C�e��";
	}
	else if (kind == ObjectTag::ITEM_BOX)
	{
		message += L"�A�C�e���{�b�N�X";
	}
	else
	{
		message += L"����";
	}

	message += L"�Ɨ��ꂽ\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}

void CarBase::OnTriggerEnter(Collidable* colider)
{
#ifdef _DEBUG
	std::wstring message = L"Trigger: �Ԃ�";
#endif

	auto kind = colider->GetTag();
#ifdef _DEBUG
	if (kind == ObjectTag::CAR)
	{
		message += L"��";
#if false
		// ���g���瑊��̒��S�ւ̃x�N�g�����쐬���A
		// ���݂̈ړ��x�N�g�����ˉe���āA
		// �o�Ă����x�N�g���𑊎�֑��x�Ƃ��ĉ�����
		auto toVec = colider->GetPos() - m_rigid.GetPos();
		auto vel = Projection(m_rigid.GetVelocity(), toVec);
		colider->AddForce(vel, 1.0f);
#endif
	}
	else if (kind == ObjectTag::GROUND)
	{
		message += L"�n��";
	}
	else if (kind == ObjectTag::JUMP_POINT)
	{
		message += L"�W�����v�|�C���g";
		m_isInJP = true;
	}
	else if (kind == ObjectTag::SYSTEM_WALL)
	{
		message += L"�V�X�e����";
	}
	else if (kind == ObjectTag::CHECK_POINT)
#else
	if (kind == ObjectTag::CHECK_POINT)
#endif
	{
#ifdef _DEBUG
		message += L"�`�F�b�N�|�C���g";
#endif
		auto cp = dynamic_cast<CheckPoint*>(colider);
		m_cpMgr.UpdateCheckPoint(m_carNo, cp->GetNo());
	}
	else if (kind == ObjectTag::ITEM)
	{
#ifdef _DEBUG
		message += L"�A�C�e��";
#endif
		if (!m_isAllGoal)
		{
			auto item = dynamic_cast<ItemBase*>(colider);
			item->Hit(this);
			item->End();
		}
	}
	else if (kind == ObjectTag::ITEM_BOX)
	{
#ifdef _DEBUG
		message += L"�A�C�e���{�b�N�X";
#endif
		if (!m_hasItem  && !m_isAllGoal)
		{
			m_hasItem = m_itemMgr.GetItem(m_carNo);
		}
	}
#ifdef _DEBUG
	else
	{
		message += L"����";
	}

	message += L"�Ɠ�������\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}

void CarBase::OnTriggerStay(Collidable* colider)
{
#ifdef _DEBUG
	std::wstring message = L"Trigger: �Ԃ�";

	auto kind = colider->GetTag();
	if (kind == ObjectTag::CAR)
	{
		message += L"��";		
	}
	else if (kind == ObjectTag::GROUND)
	{
		message += L"�n��";
	}
	else if (kind == ObjectTag::JUMP_POINT)
	{
		message += L"�W�����v�|�C���g";
	}
	else if (kind == ObjectTag::SYSTEM_WALL)
	{
		message += L"�V�X�e����";
	}
	else if (kind == ObjectTag::CHECK_POINT)
	{
		message += L"�`�F�b�N�|�C���g";
	}
	else if (kind == ObjectTag::ITEM)
	{
		message += L"�A�C�e��";
	}
	else if (kind == ObjectTag::ITEM_BOX)
	{
		message += L"�A�C�e���{�b�N�X";
	}
	else
	{
		message += L"����";
	}

	message += L"�Ɠ������Ă���\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}

void CarBase::OnTriggerExit(Collidable* colider)
{
#ifdef _DEBUG
	std::wstring message = L"Trigger: �Ԃ�";

	auto kind = colider->GetTag();
	if (kind == ObjectTag::CAR)
	{
		message += L"��";
	}
	else if (kind == ObjectTag::GROUND)
	{
		message += L"�n��";
	}
	else if (kind == ObjectTag::JUMP_POINT)
	{
		message += L"�W�����v�|�C���g";
		m_isInJP = false;
	}
	else if (kind == ObjectTag::SYSTEM_WALL)
	{
		message += L"�V�X�e����";
	}
	else if (kind == ObjectTag::CHECK_POINT)
	{
		message += L"�`�F�b�N�|�C���g";
	}
	else if (kind == ObjectTag::ITEM)
	{
		message += L"�A�C�e��";
	}
	else if (kind == ObjectTag::ITEM_BOX)
	{
		message += L"�A�C�e���{�b�N�X";
	}
	else
	{
		message += L"����";
	}

	message += L"�Ɨ��ꂽ\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}

/// <summary>
/// ����
/// </summary>
void CarBase::Decel()
{
	// ���݂̑��x���猸�����Əd�͕����x�����炷
	auto vel = m_rigid.GetVelocity();
	vel = vel - vel.GetNormalized() * m_friction * Game::GRAVITY;

	// �V�������x�ƑO�̑��x�̌������t�Ȃ�0�x�N�g���ɂ���
	if (!IsUniformDirection(vel, m_rigid.GetVelocity()))
	{
		vel = Vec3();
	}

	// ���x�̓K�p
	m_rigid.SetVelocity(vel);
}

/// <summary>
/// ����(�A�N�Z��)
/// </summary>
void CarBase::Accel()
{
	// �ő呬�x = �ő呬�x * �ō����x���� * (1 + �R�C���̖��� * 0.02) * ���ʉ���
	float max = m_maxSpeed * m_maxSpeedRate * (1.0f + COIN_SPEED_BUFFER_RATE * (m_coinNum / static_cast<float>(Game::COIN_MAX))) * m_standingSpeedRate;
	m_rigid.AddForce(m_accelDir, m_accel, true, max);
}

/// <summary>
/// �o�b�N
/// </summary>
void CarBase::Back()
{
	m_rigid.AddForce(-m_accelDir, m_accel, true, m_minSpeed * m_maxSpeedRate);
}

/// <summary>
/// �Ԃ̉�]
/// </summary>
void CarBase::Handle()
{
	Quaternion temp;
	// �n���h���𓮂����Ă���
	if (m_handle != 0.0f)
	{
		// �����������Ȃ琳�����E�Ⴄ�Ȃ�t�����ɉ�]
		if (IsUniformDirection(m_front, m_rigid.GetDir()))
		{
			temp = AngleAxis(WHEEL_ROT_ANGLE * m_handle, m_top);
		}
		else
		{
			temp = AngleAxis(WHEEL_ROT_ANGLE * -m_handle, m_top);
		}
	}
	else
	{
		temp = Quaternion(0, 0, 0, 1);
	}
	// ��������]
	m_wheelRotation = Slerp(m_wheelRotation, temp, 0.9f);
	
	auto vel = m_rigid.GetVelocity();
	// �ړ����ĂȂ��Ȃ�n���h������͂��Ȃ�
	if (vel.SqLength() == 0.0f) return;

	// �X�e�B�b�N���擾
	if (m_handle == 0.0f) return;

	// �X�e�e�B�b�N�̌X���ɍ��킹�ĉ�]
	auto q = AngleAxis(HANDLE_ANGLE * m_handle, m_top);

	BaseRot(q);
}

/// <summary>
/// �W�����v
/// </summary>
void CarBase::JumpAction()
{
	// �n�ʂɂ��Ă��Ȃ���΃W�����v���Ȃ�
	if (!m_isGround) return;

	// �t���O�̕ύX
	m_isJump = true;
	m_isJPDash = m_isInJP;
	m_isGround = false;

	// ������ɗ͂�^����
	auto vel = m_rigid.GetVelocity();
	vel.y += JUMP_POWER;
	m_rigid.SetVelocity(vel);
}

/// <summary>
/// �h���t�g
/// </summary>
void CarBase::Drift()
{
	// �h���t�g���Ă��Ȃ���ΏI��
	if (!m_isDrift) return;

	// ��葬�x�ȉ��ɂȂ�����I��
	if (m_rigid.GetVelocity().SqLength() < DRIFT_END_SPEED * DRIFT_END_SPEED)
	{
		m_turbo = 0;
		EndDrift();
		return;
	}
	// �{�^���𗣂�����I��
	if (m_isDriftEnd)
	{
		EndDrift();
		return;
	}
	
	// �^�[�{�ǉ�
	++m_turbo;

	// ���x�������̂͏���
	Quaternion q;
	// �����ɍ��킹�ĉ�]�����ύX
	if (m_isDriftRightTurn)
	{
		q = AngleAxis(DRIFT_ACCEL_ANGLE, m_top);
	}
	else
	{
		q = AngleAxis(-DRIFT_ACCEL_ANGLE, m_top);
	}

	BaseRot(q);
}

/// <summary>
/// �X���b�v�X�g���[��
/// </summary>
void CarBase::Slipstream()
{
	// FIXME:���Ŏ���
	//auto& input = Input::GetInstance();
	//if (input.IsPress(KEY_INPUT_SPACE))
	//{
	//	m_drag = std::min<float>(m_drag + SLIPSTREAM_ADD, SLIPSTREAM_RATE_MAX);
	//}
	//else
	//{
	//	m_drag = std::max<float>(m_drag - SLIPSTREAM_SUB, 1.0f);
	//}
}

/// <summary>
/// �A�C�e���̎g�p
/// </summary>
void CarBase::OnUseItem()
{
	// �A�C�e�������Ă��Ȃ���ΏI��
	if (!m_hasItem) return;

	// �����A�C�e���̎g�p
	m_hasItem->Use(this);
	// �g���v���L�m�R�Ȃ�
	if (m_hasItem->GetKind() == ItemBase::Kind::KINOKO_TRIPLE)
	{
		// �񐔂�0�ɂȂ�܂ł͎̂ĂȂ�
		auto kinokoTriple = std::dynamic_pointer_cast<ItemKinokoTriple>(m_hasItem);
		if (kinokoTriple->GetCount() <= 0)
		{
			m_hasItem = nullptr;
		}
	}
	else
	{
		m_hasItem = nullptr;
	}
}

/// <summary>
/// �ێ��A�C�e��
/// </summary>
void CarBase::HoldItem()
{
	if (!m_holdItem) return;

	// ���g�̌��Ƀ|�W�V�����ɕύX
	auto itemPos = m_rigid.GetPos() + -m_front * ITEM_HOLD_DISTANACE;
	m_holdItem->UpdateEntityPos(itemPos);

	// �A�C�e���𗣂�����
	if (m_isReleaseItem)
	{
		m_holdItem->Release(this, m_throwPower);
		m_holdItem = nullptr;
		m_isReleaseItem = false;
	}
}

/// <summary>
/// �@�������̍X�V
/// </summary>
void CarBase::NormUpdate()
{
	// �@���Ə�̕����������Ȃ�I��
	if (IsSameDirection(m_groundNorm, m_top)) return;

	// ��]
	auto q = GetQuaternion(m_groundNorm, m_top);
	m_top = q * m_top;
	BaseRot(q);
}

/// <summary>
/// �_���[�W����
/// </summary>
void CarBase::DamageUpdate()
{
	// ��~���Ȃ�
	if (m_isStop)
	{
		// �t���[���X�V
		--m_stopFrame;
		// �I��������
		if (m_stopFrame < 0)
		{
			m_isStop = false;
		}
		// ���ĂȂ��Ȃ�
		else
		{
			// ��������
			auto vel = m_rigid.GetVelocity();
			vel = vel - vel.GetNormalized() * 0.2f;
			if (!IsUniformDirection(vel, m_rigid.GetVelocity()))
			{
				vel = Vec3();
			}
			m_rigid.SetVelocity(vel);
		}
	}
	// ���G���Ȃ�
	if (m_isInvincible)
	{
		// �X�V
		--m_invincibleFrame;
		if (m_invincibleFrame < 0)
		{
			m_isInvincible = false;
		}
	}
}

/// <summary>
/// �G�t�F�N�g�̍X�V
/// </summary>
void CarBase::UpdateEff()
{
	// �ړ����̃G�t�F�N�g
	auto vel = m_rigid.GetVelocity();
	// �ړ������ő呬�x������EFF_SMOKE_SPEED_RATE�����Ȃ�
	if (vel.SqLength() > 0.0f && m_maxSpeedRate < EFF_SMOKE_SPEED_RATE)
	{
		// �t���[���X�V
		++m_smokeEffFrame;
		// �����Ԋu�𒴂�����
		if (m_smokeEffFrame > EFF_FRAME_SMOKE)
		{
			// ������
			m_smokeEffFrame = 0;
			// ����
			EffData eff;
			eff.handle = PlayEffekseer3DEffect(m_smokeEffFile->GetHandle());
			eff.frame = 0;
			eff.existTime = EFF_EXIST_SMOKE;
			eff.isExist = true;
			// �ʒu�̌���
			auto& app = Application::GetInstance();
			auto r = app.GetRand(-EFF_SMOKE_CREATE_RANGE, EFF_SMOKE_CREATE_RANGE);
			const auto& pos = m_rigid.GetPos() + m_right * r;
			eff.pos = pos;
			eff.addPos = m_front * EFF_SMOKE_MOVE_RATE;
			// �ǉ�
			m_smokeEffList.emplace_back(eff);
		}
	}
	// �X�V
	for (auto& eff : m_smokeEffList)
	{
		// �X�V
		++eff.frame;
		// ���t���[����������I��
		if (eff.frame > eff.existTime)
		{
			StopEffekseer3DEffect(eff.handle);
			eff.isExist = false;
		}
		else
		{
			// �ʒu�̍X�V
			eff.pos += eff.addPos;
			SetPosPlayingEffekseer3DEffect(eff.handle, eff.pos.x, eff.pos.y, eff.pos.z);
		}
	}
	m_smokeEffList.remove_if(
		[](const auto& eff)
		{
			return !eff.isExist;
		});

	// �h���t�g���̃G�t�F�N�g
	if (m_isMiniDriftEff)
	{
		// �ʒu�̍X�V
		auto rightPos = m_rigid.GetPos() + m_front * EFF_DRIFT_FRONT_DIS + m_right * EFF_DRIFT_RIGHT_DIS;
		auto leftPos = m_rigid.GetPos() + m_front * EFF_DRIFT_FRONT_DIS - m_right * EFF_DRIFT_RIGHT_DIS;
		SetPosPlayingEffekseer3DEffect(m_rightDriftPlayEffH, rightPos.x, rightPos.y, rightPos.z);
		SetPosPlayingEffekseer3DEffect(m_leftDriftPlayEffH, leftPos.x, leftPos.y, leftPos.z);
	}
	if (m_isDrift && !m_isDrifftEff)
	{
		// �t���[���X�V
		++m_driftEffFrme;
		// ���t���[����������
		if (m_driftEffFrme > EFF_FRAME_DASH_NEXT)
		{
			// �h���t�g�̃G�t�F�N�g��ύX
			m_isDrifftEff = true;
			StopEffekseer3DEffect(m_rightDriftPlayEffH);
			StopEffekseer3DEffect(m_leftDriftPlayEffH);
			m_rightDriftPlayEffH = PlayEffekseer3DEffect(m_driftEffFile->GetHandle());
			m_leftDriftPlayEffH = PlayEffekseer3DEffect(m_driftEffFile->GetHandle());
		}
	}

	// �_�b�V�����̃G�t�F�N�g
	if (m_isDashEff)
	{
		++m_dashEffFrame;
		if (m_dashEffFrame > m_dashEffTime)
		{
			// �I��
			m_isDashEff = false;
			StopEffekseer3DEffect(m_dashPlayEffHandle);
		}
		else
		{
			// �ʒu�ƌ����̍X�V
			const auto& pos = m_rigid.GetPos() + m_col->center + m_front * EFF_DASH_FRONT_DIS;
			const auto& rot = m_rotation * AngleAxis(180.0f, Vec3::Up());
			Matrix4x4 mat;
			mat = rot.GetMat() * Pos(pos);

			auto effMgr = GetEffekseer3DManager();
			effMgr->SetBaseMatrix(m_dashPlayEffHandle, mat.GetEffMat());
		}
	}
}

void CarBase::UpdateSound()
{
	auto& sndSys = SoundSystem::GetInstance();
	// �h���t�g���Ȃ�
	if (m_isDrift)
	{
		// �G���W��SE���Ȃ炵�Ă���Ύ~�߂�
		if (sndSys.IsNowPlay(m_engineSe))
		{
			sndSys.Stop(m_engineSe);
		}

		// �~�߂Ă�����ēx�炷
		if (!sndSys.IsNowPlay(m_driftSe))
		{
			sndSys.PlaySe(m_driftSe);
		}
	}
	else
	{
		// �h���t�gSE���Ȃ炵�Ă���Ύ~�߂�
		if (sndSys.IsNowPlay(m_driftSe))
		{
			sndSys.Stop(m_driftSe);
		}

		float speed = m_rigid.GetVelocity().SqLength();
		// �����Ă����
		if (speed != 0.0f)
		{
			// ���x�ɍ��킹�ĉ���炷
			float rate = std::min<float>(speed / (DEF_SPEED_MAX * DEF_SPEED_MAX), 1.0f);
			sndSys.PlayFadeSe(m_engineSe, rate);
		}
		// �����ĂȂ��āA�Ȃ��Ă���Ύ~�߂�
		else if (sndSys.IsNowPlay(m_engineSe))
		{
			sndSys.Stop(m_engineSe);
		}
	}
}

/// <summary>
/// �z�C�[���̍X�V
/// </summary>
/// <param name="handle">�n���h��</param>
void CarBase::UpdateWheel(int handle)
{
	auto mat = MV1GetFrameLocalMatrix(m_modelH, handle);
	auto rot = m_preWheelRotation.Conjugated() * m_wheelRotation;
	mat = MMult(rot.GetMat().GetMATRIX(), mat);
	MV1SetFrameUserLocalMatrix(m_modelH, handle, mat);
}

/// <summary>
/// �h���t�g�\���m�F
/// </summary>
void CarBase::CheckDriftStart()
{
	// �h���t�g�{�^���������Ă��Ȃ��Ȃ疳��
	if (!m_isDriftStart) return;

	// �h���t�g�\�X�s�[�h�����x���ꍇ�͖���
	if (m_rigid.GetVelocity().SqLength() < DRIFT_START_SPEED) return;

	// �X�s�[�h�������A�N�Z�������łȂ��Ȃ疳��
	if (!IsUniformDirection(m_rigid.GetDir(), m_accelDir)) return;
	
	// �n���h���𓮂����Ă��Ȃ��Ȃ疳��
	if (m_handle == 0.0f) return;

	// �����܂ŗ�����h���t�g�J�n�̏����𖞂����Ă���
	OnDrift();
}

/// <summary>
/// �h���t�g�̊J�n
/// </summary>
void CarBase::OnDrift()
{
	m_isDriftRightTurn = m_handle > 0.0f;

	// �����ɍ��킹�ĉ�]�����ύX
	Quaternion q;
	if (m_isDriftRightTurn)
	{
		q = AngleAxis(DRIFT_OBJ_ANGLE, m_top);
	}
	else
	{
		q = AngleAxis(-DRIFT_OBJ_ANGLE, m_top);
	}
	// �I�u�W�F�N�g���̂̌���������]
	m_right = q * m_right;
	m_front = q * m_front;
	m_col->dir = m_front;
	m_rotation = q * m_rotation;

	m_driftEffFrme = 0;
	m_isMiniDriftEff = true;
	m_isDrifftEff = false;
	m_rightDriftPlayEffH = PlayEffekseer3DEffect(m_miniDriftEffFile->GetHandle());
	m_leftDriftPlayEffH = PlayEffekseer3DEffect(m_miniDriftEffFile->GetHandle());

	// �^�[�{������
	m_turbo = 0;

	m_isDrift = true;
}

/// <summary>
/// �h���t�g�̏I��
/// </summary>
void CarBase::EndDrift()
{
	// �A�N�Z���������I�u�W�F�N�g�̐��ʂ�
	m_accelDir = m_front;

	StopEffekseer3DEffect(m_rightDriftPlayEffH);
	StopEffekseer3DEffect(m_leftDriftPlayEffH);

	// �i�s�����։���
	float addSpeed = std::min<int>((m_turbo + DRIFT_TURBO_START_BOOST) / DRIFT_TURBO_INTERVAL, DRIFT_TURBO_MAX_LEVEL) * DRIFT_TURBO_MAGNIFICATTION;
	OnDash(addSpeed);

	m_isDrift = false;
}

/// <summary>
/// �x�[�X�ƂȂ��]
/// </summary>
/// <param name="q">��]�</param>
void CarBase::BaseRot(const Quaternion& q)
{
	m_right = q * m_right;
	m_front = q * m_front;
	m_col->dir = m_front;
	m_rotation = q * m_rotation;
	m_accelDir = q * m_accelDir;
	m_rigid.SetVelocity(q * m_rigid.GetVelocity());
}
