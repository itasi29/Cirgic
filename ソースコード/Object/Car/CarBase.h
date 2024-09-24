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
	// ����
	void Decel();
	// �A�N�Z���E����
	void Accel();
	// �o�b�N�E����
	void Back();
	// �n���h��
	void Handle();
	// �W�����v�A�N�V����
	void JumpAction();

	// �h���t�g
	void Drift();
	// �X���b�v�X�g���[��
	void Slipstream();

	// �A�C�e���̎g�p
	void OnUseItem();
	// �A�C�e���̕ێ�������
	void HoldItem();

	// �@�������Ƀg�b�v��������
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

	/* ����֌W */
	float m_handle;
	float m_throwPower;
	bool m_isDriftStart;
	bool m_isDriftEnd;
	bool m_isReleaseItem;

	/* �_���[�W�֌W */
	bool m_isStop;			// ��~
	bool m_isInvincible;	// ���G

	/* �A�C�e�� */
	std::shared_ptr<ItemBase> m_hasItem;	// �����A�C�e��
	std::shared_ptr<ItemBase> m_holdItem;	// �����A�C�e��

	/* ���o�֌W */
	bool m_isDashEff;	// �_�b�V���G�t�F�N�g�����Ă��邩
	int m_dashEffTime;	// �_�b�V���G�t�F�N�g�̎���

	/* ���̑� */
	bool m_isGround;	// �ڒn
	int m_carNo;	// ���g�̎Ԕԍ�

private:
	Vec3 m_frontRay[static_cast<int>(FrontDir::MAX)];

	CheckPointManager& m_cpMgr;

	/* ���x�֌W */
	Vec3 m_accelDir;		// ��������
	float m_accel;			// ������
	float m_maxSpeed;		// �ō����x
	float m_minSpeed;		// �Œᑬ�x(�o�b�N���̍ō����x)
	float m_maxSpeedRate;	// �ő呬�x��
	float m_drag;			// �X���b�v�X�g���[���p
	int m_turbo;			// �h���t�g�ɂ�闣�����Ƃ��̉�����
	float m_friction;		// ���C��
	float m_standingSpeedRate;

	/* ����֌W */
	// �W�����v
	bool m_isJump;		// �W�����v
	bool m_isInJP;		// JP�\�͈͓�
	bool m_isJPDash;	// JP�ɂ��_�b�V��
	// �h���t�g
	bool m_isDrift;		// �h���t�g
	bool m_isDriftRightTurn;	// �E��]�h���t�g

	/* �_���[�W�֌W */
	int m_stopFrame;		// ��~�t���[��
	int m_invincibleFrame;	// ���G�t���[��

	/* �A�C�e���֌W */
	int m_coinNum;		// �����R�C����
	bool m_isHoldItem;	// �A�C�e����ێ����Ă��邩
	
	/* ���o�֌W */
	int m_wheelFRFrameH;	// �E�O�z�C�[���̃t���[���n���h��
	int m_wheelFLFrameH;	// ���O�z�C�[���̃t���[���n���h��
	Quaternion m_wheelRotation;		// �z�C�[���̉�]�
	Quaternion m_preWheelRotation;	// ��O�̃z�C�[���̉�]�
	std::shared_ptr<FileBase> m_smokeEffFile;	// �X���[�N�G�t�F�N�g�t�@�C��
	std::shared_ptr<FileBase> m_miniDriftEffFile;	// �~�j�h���t�g�G�t�F�N�g�t�@�C��
	std::shared_ptr<FileBase> m_driftEffFile;		// �h���t�g�G�t�F�N�g�t�@�C��
	std::shared_ptr<FileBase> m_dashEffFile;	// �_�b�V���G�t�F�N�g�̃t�@�C��
	int m_smokeEffFrame;				// �X���[�N�G�t�F�N�g�t���[��
	int m_driftEffFrme;		// �h���t�g�G�t�F�N�g�t���[��
	int m_dashEffFrame;		// �_�b�V���G�t�F�N�g�̃t���[��
	int m_rightDriftPlayEffH;	// �E�z�C�[���̃h���t�g�G�t�F�N�g�̎��s�n���h��
	int m_leftDriftPlayEffH;	// ���z�C�[���̃h���t�g�G�t�F�N�g�̎��s�n���h��
	int m_dashPlayEffHandle;	// �_�b�V���G�t�F�N�g�̎��s�n���h��
	bool m_isMiniDriftEff;	// �~�j�h���t�g�G�t�F�N�g�����Ă��邩
	bool m_isDrifftEff;		// �h���t�g�G�t�F�N�g�����Ă��邩
	std::list<EffData> m_smokeEffList;	// �X���[�N�G�t�F�N�g���X�g

	/* ���̑� */
	MyEngine::ColliderCapsule* m_col;	// ����
	Vec3 m_groundNorm;	// �ڒn���Ă���n�ʂ̖@������
	std::shared_ptr<FileBase> m_textImg;	// �Ԃ̉摜�t�@�C��
	int m_engineSe;	// �G���W����SE�n���h��
	int m_driftSe;	// �h���t�g��SE�n���h��
	std::shared_ptr<FileBase> m_dashSe;	// �_�b�V����SE�t�@�C��
	bool m_isAllGoal;	// �S�Ă��S�[�����Ă��邩
};

