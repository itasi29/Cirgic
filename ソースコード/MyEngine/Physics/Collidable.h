#pragma once
#include <memory>
#include <vector>
#include "Common/ObjectTag.h"
#include "Rigidbody.h"
#include "ColliderBase.h"

namespace MyEngine
{
	class Physics;
//	class ColliderBase;

	/// <summary>
	/// 衝突できるもの
	/// </summary>
	class Collidable abstract : public std::enable_shared_from_this<Collidable>
	{
		friend Physics;
	public:
		// 優先度判別
		enum class Priority : int
		{
			LOW,		// 低
			MIDDLE,		// 中
			HIGH,		// 高
			STATIC,		// 動かない（最高）
		};

	public:
		Collidable(Priority priority, ObjectTag tag);
		virtual ~Collidable();

#if false
		ColliderBase* GetColliderData(int index) const;
#else
		ColliderBase* GetColliderData() const;
#endif

		// 衝突したとき
		virtual void OnCollideEnter(Collidable* colider) {}
		virtual void OnCollideStay(Collidable* colider) {}
		virtual void OnCollideExit(Collidable* colider) {}
		virtual void OnTriggerEnter(Collidable* colider) {}
		virtual void OnTriggerStay(Collidable* colider) {}
		virtual void OnTriggerExit(Collidable* colider) {}

		/* Getter */
		ObjectTag GetTag() const { return m_tag; }
		Priority GetPriority() const { return m_priority; }
		const Vec3& GetPos() const;

		/* Setter */
		void AddForce(const Vec3& dir, float power);

	protected:
#if false
		std::shared_ptr<ColliderBase> AddCollider(const ColliderBase::Kind& kind);
#else
		std::shared_ptr<ColliderBase> CreateCollider(const ColliderBase::Kind& kind);
#endif

	protected:
		// 物理データ
		Rigidbody m_rigid;
		// 当たり判定データ
#if false
		std::list<std::shared_ptr<ColliderBase>> m_colliders;
#else
		std::shared_ptr<ColliderBase> m_collider;
#endif
		// Trigger判定だけを取るオブジェクトタグ
		std::list<ObjectTag> m_throughTag;

	private:
		ObjectTag m_tag;
		Priority m_priority;
	};
}

