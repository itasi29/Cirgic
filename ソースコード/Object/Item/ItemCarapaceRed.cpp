#include "ItemCarapaceRed.h"
#include <string>
#include "File/FileID.h"
#include "Geometry/MathHelp.h"
#include "Geometry/Easing.h"
#include "MyEngine/DebugDraw.h"
#include "Object/Car/CarBase.h"
#include "Race/RaceManager.h"

namespace
{
	constexpr float HOMING_DIS = 500.0f * 500.0f;
	constexpr float FRONT_DIS = 60.0f;
	constexpr float SPEED = 20.0f;
}

ItemCarapaceRed::ItemCarapaceRed(ItemManager& itemMgr, const std::weak_ptr<RaceManager>& raceMgr) :
	ItemCarapaceBase(Kind::CARAPACE_RED, itemMgr, FileID::Eff::CARAPCE_RED),
	m_raceMgr(raceMgr),
	m_isHoming(false),
	m_isInHomingDis(false),
	m_homingCar(nullptr)
{
}

ItemCarapaceRed::~ItemCarapaceRed()
{
}

void ItemCarapaceRed::Update()
{
	ItemCarapaceBase::Update();

	// 
	// コースの道に沿って進ませる→これは後でやる(とりあえず直進するままで)
	// コライダーを追加して(isTrigger)
	// その範囲内に入ったらCarをホーミングしだす
	// →コライダーを全部IsTriggerと同じやってるから距離で
	// すぐにグワンじゃなくて、徐々に向かう感じで
	// 

	// 
	// コースの道に沿って進ませる→これは後でやる(とりあえず直進するままで)
	// 目的の現在のCPを通過したら敵の後ろのラインに移動
	// その後はぶつかるまで後ろのラインをホーミング
	// 

	if (m_isHoming)
	{
		// ホーミング距離内に入った
		if (m_isInHomingDis)
		{
			// 甲羅から目的の車までのベクトル
			auto thisToCar = m_homingCar->GetPos() - m_rigid.GetPos();
			// 移動ベクトルを徐々に変更
			auto newVel = Slerp(m_rigid.GetVelocity(), thisToCar, 0.1f);
			m_rigid.SetVelocity(newVel);
			
		}
		// 入っていない状態
		else
		{
			// TODO: コースに沿って進ませる

			// 範囲内に入ったかの判定
			auto sqLen = (m_rigid.GetPos() - m_homingCar->GetPos()).SqLength();
			m_isInHomingDis = sqLen < HOMING_DIS;
		}
	}
}

void ItemCarapaceRed::Release(CarBase* car, float power)
{
	ItemCarapaceBase::Release(car, power);
	auto front = car->GetFront();
	Vec3 pos;
	Vec3 vel;

	// 後ろに倒している
	if (power < 0)
	{
		pos = m_rigid.GetPos();
		vel = front * -SPEED;
	}
	// 前に倒しているor倒していない
	else
	{
		pos = car->GetPos() + front * FRONT_DIS;
		vel = front * SPEED;

		// 前に飛ばしたときだけホーミング
		// 順位取得
		// 1位ならホーミングしない
		// それ以上の順位ならホーミングする
		// ホーミングする車の情報取得
		// 
		auto standing = m_raceMgr.lock()->GetStanding(car->GetCarNo());
		if (standing != 1)
		{
			m_homingCar = m_raceMgr.lock()->GetCarToStanding(standing - 1);
			m_isHoming = true;
			m_isInHomingDis = false;
		}

		
	}

	m_rigid.SetPos(pos);
	m_rigid.SetVelocity(vel);
	m_rigid.SetGravity(true);
}

void ItemCarapaceRed::OnCollideEnter(Collidable* colider)
{
	ItemCarapaceBase::OnCollideEnter(colider);
#ifdef _DEBUG
	std::wstring message = L"Collide: 赤甲羅が";
#endif

	auto kind = colider->GetTag();
	if (kind == ObjectTag::SYSTEM_WALL)
	{
#ifdef _DEBUG
		message += L"システム壁";
#endif

		End();
	}

#ifdef _DEBUG
	message += L"と当たった\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}
