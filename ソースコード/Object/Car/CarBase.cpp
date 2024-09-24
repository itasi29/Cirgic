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

	// 速度関係
	constexpr float DEF_ACCEL = 0.18f;
	constexpr float DEF_SPEED_MAX = 12.48f;
	constexpr float DEF_SPEED_MIN = -6.33f;
	constexpr float DASH_MAX_RATE = 1.75f * 1.75f;

	// 一定値以下を速度ゼロとする
	constexpr float MIN_SPEED_CONSTANT = 0.0001f;

	// ドリフト関係
	constexpr float DRIFT_START_SPEED = 9.0f;
	constexpr float DRIFT_END_SPEED = 9.0f;
	constexpr float DRIFT_OBJ_ANGLE = 5;
	constexpr float DRIFT_ACCEL_ANGLE = 0.7f;
	constexpr float DRIFT_TURBO_MAGNIFICATTION = 1.5f;
	constexpr int DRIFT_TURBO_INTERVAL = 180;
	constexpr int DRIFT_TURBO_START_BOOST = 60;
	constexpr int DRIFT_TURBO_MAX_LEVEL = 8;

	// スリップストリーム関係
	constexpr float SLIPSTREAM_RATE_MAX = 1.2f;
	constexpr float SLIPSTREAM_ADD = 0.001f;
	constexpr float SLIPSTREAM_SUB = 0.002f;

	// ジャンプアクション関係
	constexpr float JUMP_BOOST_SPEED = 5.0f;

	// ダメージ処理関係
	constexpr int DAMAGE_FRAME_INVINCIBLE = 90;
	constexpr int DAMAGE_FRAME_STOP = 30;

	// エフェクト関係
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

	// 加速系
	constexpr float COIN_SPEED_BUFFER_RATE = 0.2f;

	// アイテム系
	constexpr float ITEM_HOLD_DISTANACE = 75.0f;

	// ハンドルによるタイヤの回転具合
	constexpr int WHEEL_ROT_ANGLE = 30;
}

CarBase::CarBase(ItemManager& itemMgr, CheckPointManager& cpMgr, const Vec3& pos) :
	Object3D(MyEngine::Collidable::Priority::HIGH, ObjectTag::CAR, FileID::Mdl::CAR),
	m_itemMgr(itemMgr),
	m_cpMgr(cpMgr),
	/* 操作関係 */
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
	/* ダメージ関係 */
	m_invincibleFrame(0),
	m_isInvincible(false),
	m_isStop(false),
	m_stopFrame(0),
	/* 速度関係初期化 */
	m_accelDir(m_front),
	m_accel(DEF_ACCEL),
	m_maxSpeed(DEF_SPEED_MAX* DEF_SPEED_MAX),
	m_minSpeed(DEF_SPEED_MIN* DEF_SPEED_MIN),
	m_maxSpeedRate(1.0f),
	m_drag(1.0f),
	m_turbo(0),
	m_friction(0.5f),
	m_standingSpeedRate(1.0f),
	/* アイテム関係 */
	m_coinNum(0),
	m_hasItem(nullptr),
	m_holdItem(nullptr),
	m_isHoldItem(false),
	/* 演出関係 */
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
	/* その他 */
	m_isGround(false),
	m_groundNorm(Vec3::Up()),
	m_carNo(0),
	m_isAllGoal(false)
{
	// モデルに対して
	m_rigid.SetGravity(true);
	m_rigid.Init(pos);
	m_scale = Vec3(0.01f, 0.01f, 0.01f);
	m_rotation = AngleAxis(180, Vec3::Up());

	// 判定
	m_col = dynamic_cast<MyEngine::ColliderCapsule*>(CreateCollider(MyEngine::ColliderBase::Kind::Capsule).get());
	m_col->Init(Vec3(0, 17.0f, 0), m_front, 18.0f, 45.0f, false);
	m_col->SetMinMax(m_rigid.GetPos());

	// アイテムをスルーオブジェクトに変更
	m_throughTag.push_back(ObjectTag::ITEM);

	// データの取得
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

	// 番号に合わせてテクスチャの変更
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
		{ ItemBase::Kind::COIN, L"コイン" },
		{ ItemBase::Kind::KINOKO, L"キノコ" },
		{ ItemBase::Kind::KINOKO_TRIPLE, L"３キノ" },
		{ ItemBase::Kind::BANANA, L"バナナ" },
		{ ItemBase::Kind::CARAPACE_GREEN, L"緑甲羅" },
		{ ItemBase::Kind::CARAPACE_RED, L"赤甲羅" },
	};
	if (m_hasItem)
	{
		auto kind = m_hasItem->GetKind();
		debug.DrawString(160, 32, 0xffffff, L"所持アイテム：" + STR_ITEM.at(kind));
	}
	else
	{
		debug.DrawString(160, 32, 0xffffff, L"所持アイテム：もってない");
	}
	if (m_holdItem)
	{
		auto kind = m_holdItem->GetKind();
		debug.DrawString(160, 32, 0xffffff, L"ホールドアイテム：" + STR_ITEM.at(kind));
	}
	else
	{
		debug.DrawString(160, 32, 0xffffff, L"ホールドアイテム：もってない");
	}
#endif
}

void CarBase::End()
{
	Object3D::End();
	// データ削除
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
/// ダッシュ
/// </summary>
/// <param name="dash">ダッシュ速度</param>
void CarBase::OnDash(float dash)
{
	// 最大速度 = 最大速度 * (1 + コインの枚数 * 0.02) * 順位加速 *  ダッシュの最高速度
	float max = m_maxSpeed * (1.0f + COIN_SPEED_BUFFER_RATE * (m_coinNum / static_cast<float>(Game::COIN_MAX))) * m_standingSpeedRate * DASH_MAX_RATE;
	m_rigid.AddForce(m_front, dash, true, max, true);

	// dash していればサウンド鳴らす
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
	std::wstring message = L"Collide: 車が";
#endif
	auto kind = colider->GetTag();
#ifdef _DEBUG
	if (kind == ObjectTag::CAR)
	{
		message += L"車";
	}
	else if (kind == ObjectTag::GROUND)
#endif
	if (kind == ObjectTag::GROUND)
	{
#ifdef _DEBUG
		message += L"地面";
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
		message += L"ジャンプポイント";
	}
	else if (kind == ObjectTag::SYSTEM_WALL)
	{
		message += L"システム壁";
	}
	else if (kind == ObjectTag::CHECK_POINT)
	{
		message += L"チェックポイント";
	}
	else if (kind == ObjectTag::ITEM)
	{
		message += L"アイテム";

	}
	else if (kind == ObjectTag::ITEM_BOX)
	{
		message += L"アイテムボックス";
	}
	else
	{
		message += L"何か";
	}

	message += L"と当たった\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}

void CarBase::OnCollideStay(Collidable* colider)
{
#ifdef _DEBUG
	std::wstring message = L"Collide: 車が";
#endif

	auto kind = colider->GetTag();
#ifdef _DEBUG
	if (kind == ObjectTag::CAR)
	{
		message += L"車";
	}
	else if (kind == ObjectTag::GROUND)
#endif
	if (kind == ObjectTag::GROUND)
	{
#ifdef _DEBUG
		message += L"地面";
#endif
		// 重力加速度を0に
		m_rigid.ZeroGravity();
		// 地面の法線方向に上方向を変更
		auto terrain = dynamic_cast<MyEngine::ColliderTerrain*>(colider->GetColliderData());
		auto temp = Slerp(m_groundNorm, terrain->norm, 1.0f);
		m_groundNorm = temp;

		// 道の最高速度に合わせて最高速度割合を変更する
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
		message += L"ジャンプポイント";
	}
	else if (kind == ObjectTag::SYSTEM_WALL)
	{
		message += L"システム壁";
		Decel();
	}
	else if (kind == ObjectTag::CHECK_POINT)
	{
		message += L"チェックポイント";
	}
	else if (kind == ObjectTag::ITEM)
	{
		message += L"アイテム";
	}
	else if (kind == ObjectTag::ITEM_BOX)
	{
		message += L"アイテムボックス";
	}
	else
	{
		message += L"何か";
	}

	message += L"と当たっている\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}

void CarBase::OnCollideExit(Collidable* colider)
{
#ifdef _DEBUG
	std::wstring message = L"Collide: 車が";

	auto kind = colider->GetTag();
	if (kind == ObjectTag::CAR)
	{
		message += L"車";
	}
	else if (kind == ObjectTag::GROUND)
	{
		message += L"地面";
	}
	else if (kind == ObjectTag::JUMP_POINT)
	{
		message += L"ジャンプポイント";
	}
	else if (kind == ObjectTag::SYSTEM_WALL)
	{
		message += L"システム壁";
	}
	else if (kind == ObjectTag::CHECK_POINT)
	{
		message += L"チェックポイント";
	}
	else if (kind == ObjectTag::ITEM)
	{
		message += L"アイテム";
	}
	else if (kind == ObjectTag::ITEM_BOX)
	{
		message += L"アイテムボックス";
	}
	else
	{
		message += L"何か";
	}

	message += L"と離れた\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}

void CarBase::OnTriggerEnter(Collidable* colider)
{
#ifdef _DEBUG
	std::wstring message = L"Trigger: 車が";
#endif

	auto kind = colider->GetTag();
#ifdef _DEBUG
	if (kind == ObjectTag::CAR)
	{
		message += L"車";
#if false
		// 自身から相手の中心へのベクトルを作成し、
		// 現在の移動ベクトルを射影して、
		// 出てきたベクトルを相手へ速度として加える
		auto toVec = colider->GetPos() - m_rigid.GetPos();
		auto vel = Projection(m_rigid.GetVelocity(), toVec);
		colider->AddForce(vel, 1.0f);
#endif
	}
	else if (kind == ObjectTag::GROUND)
	{
		message += L"地面";
	}
	else if (kind == ObjectTag::JUMP_POINT)
	{
		message += L"ジャンプポイント";
		m_isInJP = true;
	}
	else if (kind == ObjectTag::SYSTEM_WALL)
	{
		message += L"システム壁";
	}
	else if (kind == ObjectTag::CHECK_POINT)
#else
	if (kind == ObjectTag::CHECK_POINT)
#endif
	{
#ifdef _DEBUG
		message += L"チェックポイント";
#endif
		auto cp = dynamic_cast<CheckPoint*>(colider);
		m_cpMgr.UpdateCheckPoint(m_carNo, cp->GetNo());
	}
	else if (kind == ObjectTag::ITEM)
	{
#ifdef _DEBUG
		message += L"アイテム";
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
		message += L"アイテムボックス";
#endif
		if (!m_hasItem  && !m_isAllGoal)
		{
			m_hasItem = m_itemMgr.GetItem(m_carNo);
		}
	}
#ifdef _DEBUG
	else
	{
		message += L"何か";
	}

	message += L"と当たった\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}

void CarBase::OnTriggerStay(Collidable* colider)
{
#ifdef _DEBUG
	std::wstring message = L"Trigger: 車が";

	auto kind = colider->GetTag();
	if (kind == ObjectTag::CAR)
	{
		message += L"車";		
	}
	else if (kind == ObjectTag::GROUND)
	{
		message += L"地面";
	}
	else if (kind == ObjectTag::JUMP_POINT)
	{
		message += L"ジャンプポイント";
	}
	else if (kind == ObjectTag::SYSTEM_WALL)
	{
		message += L"システム壁";
	}
	else if (kind == ObjectTag::CHECK_POINT)
	{
		message += L"チェックポイント";
	}
	else if (kind == ObjectTag::ITEM)
	{
		message += L"アイテム";
	}
	else if (kind == ObjectTag::ITEM_BOX)
	{
		message += L"アイテムボックス";
	}
	else
	{
		message += L"何か";
	}

	message += L"と当たっている\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}

void CarBase::OnTriggerExit(Collidable* colider)
{
#ifdef _DEBUG
	std::wstring message = L"Trigger: 車が";

	auto kind = colider->GetTag();
	if (kind == ObjectTag::CAR)
	{
		message += L"車";
	}
	else if (kind == ObjectTag::GROUND)
	{
		message += L"地面";
	}
	else if (kind == ObjectTag::JUMP_POINT)
	{
		message += L"ジャンプポイント";
		m_isInJP = false;
	}
	else if (kind == ObjectTag::SYSTEM_WALL)
	{
		message += L"システム壁";
	}
	else if (kind == ObjectTag::CHECK_POINT)
	{
		message += L"チェックポイント";
	}
	else if (kind == ObjectTag::ITEM)
	{
		message += L"アイテム";
	}
	else if (kind == ObjectTag::ITEM_BOX)
	{
		message += L"アイテムボックス";
	}
	else
	{
		message += L"何か";
	}

	message += L"と離れた\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}

/// <summary>
/// 減速
/// </summary>
void CarBase::Decel()
{
	// 現在の速度から減速率と重力分速度を減らす
	auto vel = m_rigid.GetVelocity();
	vel = vel - vel.GetNormalized() * m_friction * Game::GRAVITY;

	// 新しい速度と前の速度の向きが逆なら0ベクトルにする
	if (!IsUniformDirection(vel, m_rigid.GetVelocity()))
	{
		vel = Vec3();
	}

	// 速度の適用
	m_rigid.SetVelocity(vel);
}

/// <summary>
/// 加速(アクセル)
/// </summary>
void CarBase::Accel()
{
	// 最大速度 = 最大速度 * 最高速度割合 * (1 + コインの枚数 * 0.02) * 順位加速
	float max = m_maxSpeed * m_maxSpeedRate * (1.0f + COIN_SPEED_BUFFER_RATE * (m_coinNum / static_cast<float>(Game::COIN_MAX))) * m_standingSpeedRate;
	m_rigid.AddForce(m_accelDir, m_accel, true, max);
}

/// <summary>
/// バック
/// </summary>
void CarBase::Back()
{
	m_rigid.AddForce(-m_accelDir, m_accel, true, m_minSpeed * m_maxSpeedRate);
}

/// <summary>
/// 車の回転
/// </summary>
void CarBase::Handle()
{
	Quaternion temp;
	// ハンドルを動かしている
	if (m_handle != 0.0f)
	{
		// 向きが同じなら正方向・違うなら逆方向に回転
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
	// 少しずつ回転
	m_wheelRotation = Slerp(m_wheelRotation, temp, 0.9f);
	
	auto vel = m_rigid.GetVelocity();
	// 移動してないならハンドル操作はしない
	if (vel.SqLength() == 0.0f) return;

	// スティック情報取得
	if (m_handle == 0.0f) return;

	// ステティックの傾きに合わせて回転
	auto q = AngleAxis(HANDLE_ANGLE * m_handle, m_top);

	BaseRot(q);
}

/// <summary>
/// ジャンプ
/// </summary>
void CarBase::JumpAction()
{
	// 地面についていなければジャンプしない
	if (!m_isGround) return;

	// フラグの変更
	m_isJump = true;
	m_isJPDash = m_isInJP;
	m_isGround = false;

	// 上方向に力を与える
	auto vel = m_rigid.GetVelocity();
	vel.y += JUMP_POWER;
	m_rigid.SetVelocity(vel);
}

/// <summary>
/// ドリフト
/// </summary>
void CarBase::Drift()
{
	// ドリフトしていなければ終了
	if (!m_isDrift) return;

	// 一定速度以下になったら終了
	if (m_rigid.GetVelocity().SqLength() < DRIFT_END_SPEED * DRIFT_END_SPEED)
	{
		m_turbo = 0;
		EndDrift();
		return;
	}
	// ボタンを離したら終了
	if (m_isDriftEnd)
	{
		EndDrift();
		return;
	}
	
	// ターボ追加
	++m_turbo;

	// 速度方向自体は少し
	Quaternion q;
	// 向きに合わせて回転方向変更
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
/// スリップストリーム
/// </summary>
void CarBase::Slipstream()
{
	// FIXME:仮で実装
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
/// アイテムの使用
/// </summary>
void CarBase::OnUseItem()
{
	// アイテムを持ていなければ終了
	if (!m_hasItem) return;

	// 所持アイテムの使用
	m_hasItem->Use(this);
	// トリプルキノコなら
	if (m_hasItem->GetKind() == ItemBase::Kind::KINOKO_TRIPLE)
	{
		// 回数が0になるまでは捨てない
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
/// 保持アイテム
/// </summary>
void CarBase::HoldItem()
{
	if (!m_holdItem) return;

	// 自身の後ろにポジションに変更
	auto itemPos = m_rigid.GetPos() + -m_front * ITEM_HOLD_DISTANACE;
	m_holdItem->UpdateEntityPos(itemPos);

	// アイテムを離したら
	if (m_isReleaseItem)
	{
		m_holdItem->Release(this, m_throwPower);
		m_holdItem = nullptr;
		m_isReleaseItem = false;
	}
}

/// <summary>
/// 法線方向の更新
/// </summary>
void CarBase::NormUpdate()
{
	// 法線と上の方向が同じなら終了
	if (IsSameDirection(m_groundNorm, m_top)) return;

	// 回転
	auto q = GetQuaternion(m_groundNorm, m_top);
	m_top = q * m_top;
	BaseRot(q);
}

/// <summary>
/// ダメージ処理
/// </summary>
void CarBase::DamageUpdate()
{
	// 停止中なら
	if (m_isStop)
	{
		// フレーム更新
		--m_stopFrame;
		// 終了したら
		if (m_stopFrame < 0)
		{
			m_isStop = false;
		}
		// してないなら
		else
		{
			// 減速処理
			auto vel = m_rigid.GetVelocity();
			vel = vel - vel.GetNormalized() * 0.2f;
			if (!IsUniformDirection(vel, m_rigid.GetVelocity()))
			{
				vel = Vec3();
			}
			m_rigid.SetVelocity(vel);
		}
	}
	// 無敵中なら
	if (m_isInvincible)
	{
		// 更新
		--m_invincibleFrame;
		if (m_invincibleFrame < 0)
		{
			m_isInvincible = false;
		}
	}
}

/// <summary>
/// エフェクトの更新
/// </summary>
void CarBase::UpdateEff()
{
	// 移動時のエフェクト
	auto vel = m_rigid.GetVelocity();
	// 移動中かつ最大速度割合がEFF_SMOKE_SPEED_RATE未満なら
	if (vel.SqLength() > 0.0f && m_maxSpeedRate < EFF_SMOKE_SPEED_RATE)
	{
		// フレーム更新
		++m_smokeEffFrame;
		// 生成間隔を超えたら
		if (m_smokeEffFrame > EFF_FRAME_SMOKE)
		{
			// 初期化
			m_smokeEffFrame = 0;
			// 生成
			EffData eff;
			eff.handle = PlayEffekseer3DEffect(m_smokeEffFile->GetHandle());
			eff.frame = 0;
			eff.existTime = EFF_EXIST_SMOKE;
			eff.isExist = true;
			// 位置の決定
			auto& app = Application::GetInstance();
			auto r = app.GetRand(-EFF_SMOKE_CREATE_RANGE, EFF_SMOKE_CREATE_RANGE);
			const auto& pos = m_rigid.GetPos() + m_right * r;
			eff.pos = pos;
			eff.addPos = m_front * EFF_SMOKE_MOVE_RATE;
			// 追加
			m_smokeEffList.emplace_back(eff);
		}
	}
	// 更新
	for (auto& eff : m_smokeEffList)
	{
		// 更新
		++eff.frame;
		// 一定フレーム超えたら終了
		if (eff.frame > eff.existTime)
		{
			StopEffekseer3DEffect(eff.handle);
			eff.isExist = false;
		}
		else
		{
			// 位置の更新
			eff.pos += eff.addPos;
			SetPosPlayingEffekseer3DEffect(eff.handle, eff.pos.x, eff.pos.y, eff.pos.z);
		}
	}
	m_smokeEffList.remove_if(
		[](const auto& eff)
		{
			return !eff.isExist;
		});

	// ドリフト時のエフェクト
	if (m_isMiniDriftEff)
	{
		// 位置の更新
		auto rightPos = m_rigid.GetPos() + m_front * EFF_DRIFT_FRONT_DIS + m_right * EFF_DRIFT_RIGHT_DIS;
		auto leftPos = m_rigid.GetPos() + m_front * EFF_DRIFT_FRONT_DIS - m_right * EFF_DRIFT_RIGHT_DIS;
		SetPosPlayingEffekseer3DEffect(m_rightDriftPlayEffH, rightPos.x, rightPos.y, rightPos.z);
		SetPosPlayingEffekseer3DEffect(m_leftDriftPlayEffH, leftPos.x, leftPos.y, leftPos.z);
	}
	if (m_isDrift && !m_isDrifftEff)
	{
		// フレーム更新
		++m_driftEffFrme;
		// 一定フレームたったら
		if (m_driftEffFrme > EFF_FRAME_DASH_NEXT)
		{
			// ドリフトのエフェクトを変更
			m_isDrifftEff = true;
			StopEffekseer3DEffect(m_rightDriftPlayEffH);
			StopEffekseer3DEffect(m_leftDriftPlayEffH);
			m_rightDriftPlayEffH = PlayEffekseer3DEffect(m_driftEffFile->GetHandle());
			m_leftDriftPlayEffH = PlayEffekseer3DEffect(m_driftEffFile->GetHandle());
		}
	}

	// ダッシュ時のエフェクト
	if (m_isDashEff)
	{
		++m_dashEffFrame;
		if (m_dashEffFrame > m_dashEffTime)
		{
			// 終了
			m_isDashEff = false;
			StopEffekseer3DEffect(m_dashPlayEffHandle);
		}
		else
		{
			// 位置と向きの更新
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
	// ドリフト中なら
	if (m_isDrift)
	{
		// エンジンSEをならしていれば止める
		if (sndSys.IsNowPlay(m_engineSe))
		{
			sndSys.Stop(m_engineSe);
		}

		// 止めていたら再度鳴らす
		if (!sndSys.IsNowPlay(m_driftSe))
		{
			sndSys.PlaySe(m_driftSe);
		}
	}
	else
	{
		// ドリフトSEをならしていれば止める
		if (sndSys.IsNowPlay(m_driftSe))
		{
			sndSys.Stop(m_driftSe);
		}

		float speed = m_rigid.GetVelocity().SqLength();
		// 動いていれば
		if (speed != 0.0f)
		{
			// 速度に合わせて音を鳴らす
			float rate = std::min<float>(speed / (DEF_SPEED_MAX * DEF_SPEED_MAX), 1.0f);
			sndSys.PlayFadeSe(m_engineSe, rate);
		}
		// 動いてなくて、なっていれば止める
		else if (sndSys.IsNowPlay(m_engineSe))
		{
			sndSys.Stop(m_engineSe);
		}
	}
}

/// <summary>
/// ホイールの更新
/// </summary>
/// <param name="handle">ハンドル</param>
void CarBase::UpdateWheel(int handle)
{
	auto mat = MV1GetFrameLocalMatrix(m_modelH, handle);
	auto rot = m_preWheelRotation.Conjugated() * m_wheelRotation;
	mat = MMult(rot.GetMat().GetMATRIX(), mat);
	MV1SetFrameUserLocalMatrix(m_modelH, handle, mat);
}

/// <summary>
/// ドリフト可能か確認
/// </summary>
void CarBase::CheckDriftStart()
{
	// ドリフトボタンを押していないなら無視
	if (!m_isDriftStart) return;

	// ドリフト可能スピードよりも遅い場合は無視
	if (m_rigid.GetVelocity().SqLength() < DRIFT_START_SPEED) return;

	// スピード方向がアクセル方向でないなら無視
	if (!IsUniformDirection(m_rigid.GetDir(), m_accelDir)) return;
	
	// ハンドルを動かしていないなら無視
	if (m_handle == 0.0f) return;

	// ここまで来たらドリフト開始の条件を満たしている
	OnDrift();
}

/// <summary>
/// ドリフトの開始
/// </summary>
void CarBase::OnDrift()
{
	m_isDriftRightTurn = m_handle > 0.0f;

	// 向きに合わせて回転方向変更
	Quaternion q;
	if (m_isDriftRightTurn)
	{
		q = AngleAxis(DRIFT_OBJ_ANGLE, m_top);
	}
	else
	{
		q = AngleAxis(-DRIFT_OBJ_ANGLE, m_top);
	}
	// オブジェクト自体の向きだけ回転
	m_right = q * m_right;
	m_front = q * m_front;
	m_col->dir = m_front;
	m_rotation = q * m_rotation;

	m_driftEffFrme = 0;
	m_isMiniDriftEff = true;
	m_isDrifftEff = false;
	m_rightDriftPlayEffH = PlayEffekseer3DEffect(m_miniDriftEffFile->GetHandle());
	m_leftDriftPlayEffH = PlayEffekseer3DEffect(m_miniDriftEffFile->GetHandle());

	// ターボ初期化
	m_turbo = 0;

	m_isDrift = true;
}

/// <summary>
/// ドリフトの終了
/// </summary>
void CarBase::EndDrift()
{
	// アクセル方向をオブジェクトの正面へ
	m_accelDir = m_front;

	StopEffekseer3DEffect(m_rightDriftPlayEffH);
	StopEffekseer3DEffect(m_leftDriftPlayEffH);

	// 進行方向へ加速
	float addSpeed = std::min<int>((m_turbo + DRIFT_TURBO_START_BOOST) / DRIFT_TURBO_INTERVAL, DRIFT_TURBO_MAX_LEVEL) * DRIFT_TURBO_MAGNIFICATTION;
	OnDash(addSpeed);

	m_isDrift = false;
}

/// <summary>
/// ベースとなる回転
/// </summary>
/// <param name="q">回転具合</param>
void CarBase::BaseRot(const Quaternion& q)
{
	m_right = q * m_right;
	m_front = q * m_front;
	m_col->dir = m_front;
	m_rotation = q * m_rotation;
	m_accelDir = q * m_accelDir;
	m_rigid.SetVelocity(q * m_rigid.GetVelocity());
}
