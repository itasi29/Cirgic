#pragma once
#include <memory>
#include <list>
#include "Object/Object3D.h"
#include "Geometry/Vec3.h"
#include "Geometry/Matrix4x4.h"
#include "Object/Item/ItemBase.h"

namespace MyEngine
{
	class ColliderCapsule;
}

struct EffData
{
	int handle = 0;
	int frame = 0;
	int existTime = 0;
	Vec3 pos = Vec3();
	Vec3 addPos = Vec3();
	bool isExist = false;
};

class ItemManager;
class CheckPointManager;
class FileBase;

class CarBase : public Object3D
{
public:
	CarBase(ItemManager& itemMgr, CheckPointManager& cpMgr, const Vec3& pos);
	virtual ~CarBase();

	void InitCarNo(int no);
	virtual void Update();
	void Draw() const override;
	void End() override;

	virtual void OnDash(float dash);
	void OnHoldItem();
	void OnDamage();

	int GetCarNo() const { return m_carNo; }
	int GetCoinNum() const { return m_coinNum; }
	const Vec3& GetPos() const;
	bool HasItem() const;
	ItemBase::Kind GetItemKind() const;
	const std::shared_ptr<ItemBase> GetItem() const { return m_hasItem; }

	void GetCoin();

	void SetStandingSpeedRate(float rate) { m_standingSpeedRate = rate; }
	void AllGoal();

	virtual void OnCollideEnter(Collidable* colider) override;
	virtual void OnCollideStay(Collidable* colider) override;
	virtual void OnCollideExit(Collidable* colider) override;
	virtual void OnTriggerEnter(Collidable* colider) override;
	virtual void OnTriggerStay(Collidable* colider) override;
	virtual void OnTriggerExit(Collidable* colider) override;

protected:
	// 減速
	void Decel();
	// アクセル・加速
	void Accel();
	// バック・減速
	void Back();
	// ハンドル
	void Handle();
	// ジャンプアクション
	void JumpAction();

	// ドリフト
	void Drift();
	// スリップストリーム
	void Slipstream();

	// アイテムの使用
	void OnUseItem();
	// アイテムの保持し続け
	void HoldItem();

	// 法線方向にトップを向ける
	void NormUpdate();

	void DamageUpdate();

private:
	void UpdateEff();
	void UpdateSound();
	void UpdateWheel(int handle);

	void CheckDriftStart();
	void OnDrift();
	void EndDrift();

	void BaseRot(const Quaternion& q);

protected:
	enum class FrontDir : int
	{
		CENTER,
		RIGHT,
		LEFT,
		MAX
	};

protected:
	ItemManager& m_itemMgr;

	/* 操作関係 */
	float m_handle;
	float m_throwPower;
	bool m_isDriftStart;
	bool m_isDriftEnd;
	bool m_isReleaseItem;

	/* ダメージ関係 */
	bool m_isStop;			// 停止
	bool m_isInvincible;	// 無敵

	/* アイテム */
	std::shared_ptr<ItemBase> m_hasItem;	// 所持アイテム
	std::shared_ptr<ItemBase> m_holdItem;	// 所持アイテム

	/* 演出関係 */
	bool m_isDashEff;	// ダッシュエフェクトをしているか
	int m_dashEffTime;	// ダッシュエフェクトの時間

	/* その他 */
	bool m_isGround;	// 接地
	int m_carNo;	// 自身の車番号

private:
	Vec3 m_frontRay[static_cast<int>(FrontDir::MAX)];

	CheckPointManager& m_cpMgr;

	/* 速度関係 */
	Vec3 m_accelDir;		// 加速方向
	float m_accel;			// 加速力
	float m_maxSpeed;		// 最高速度
	float m_minSpeed;		// 最低速度(バック時の最高速度)
	float m_maxSpeedRate;	// 最大速度率
	float m_drag;			// スリップストリーム用
	int m_turbo;			// ドリフトによる離したときの加速力
	float m_friction;		// 摩擦力
	float m_standingSpeedRate;

	/* 操作関係 */
	// ジャンプ
	bool m_isJump;		// ジャンプ
	bool m_isInJP;		// JP可能範囲内
	bool m_isJPDash;	// JPによるダッシュ
	// ドリフト
	bool m_isDrift;		// ドリフト
	bool m_isDriftRightTurn;	// 右回転ドリフト

	/* ダメージ関係 */
	int m_stopFrame;		// 停止フレーム
	int m_invincibleFrame;	// 無敵フレーム

	/* アイテム関係 */
	int m_coinNum;		// 所持コイン数
	bool m_isHoldItem;	// アイテムを保持しているか
	
	/* 演出関係 */
	int m_wheelFRFrameH;	// 右前ホイールのフレームハンドル
	int m_wheelFLFrameH;	// 左前ホイールのフレームハンドル
	Quaternion m_wheelRotation;		// ホイールの回転具合
	Quaternion m_preWheelRotation;	// 一つ前のホイールの回転具合
	std::shared_ptr<FileBase> m_smokeEffFile;	// スモークエフェクトファイル
	std::shared_ptr<FileBase> m_miniDriftEffFile;	// ミニドリフトエフェクトファイル
	std::shared_ptr<FileBase> m_driftEffFile;		// ドリフトエフェクトファイル
	std::shared_ptr<FileBase> m_dashEffFile;	// ダッシュエフェクトのファイル
	int m_smokeEffFrame;				// スモークエフェクトフレーム
	int m_driftEffFrme;		// ドリフトエフェクトフレーム
	int m_dashEffFrame;		// ダッシュエフェクトのフレーム
	int m_rightDriftPlayEffH;	// 右ホイールのドリフトエフェクトの実行ハンドル
	int m_leftDriftPlayEffH;	// 左ホイールのドリフトエフェクトの実行ハンドル
	int m_dashPlayEffHandle;	// ダッシュエフェクトの実行ハンドル
	bool m_isMiniDriftEff;	// ミニドリフトエフェクトをしているか
	bool m_isDrifftEff;		// ドリフトエフェクトをしているか
	std::list<EffData> m_smokeEffList;	// スモークエフェクトリスト

	/* その他 */
	MyEngine::ColliderCapsule* m_col;	// 判定
	Vec3 m_groundNorm;	// 接地している地面の法線方向
	std::shared_ptr<FileBase> m_textImg;	// 車の画像ファイル
	int m_engineSe;	// エンジンのSEハンドル
	int m_driftSe;	// ドリフトのSEハンドル
	std::shared_ptr<FileBase> m_dashSe;	// ダッシュのSEファイル
	bool m_isAllGoal;	// 全てがゴールしているか
};

