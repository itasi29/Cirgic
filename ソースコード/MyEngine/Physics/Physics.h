#pragma once
#include <memory>
#include <list>
#include <functional>
#include "Common/ObjectTag.h"
#include "Utility/Utility.h"

struct Vec3;

namespace MyEngine
{
	class Rigidbody;
	class Collidable;
	class ColliderBase;

	class ColliderTreeObject;
	class CLiner8TreeManager;

	class Physics final
	{
	private:
		enum class OnCollideInfoKind
		{
			CollideEnter,
			CollideStay,
			CollideExit,
			TriggerEnter,
			TriggerStay,
			TriggerExit
		};
		struct OnCollideInfoData
		{
			Collidable* own;
			Collidable* send;
			OnCollideInfoKind kind;
		};
//		using SendCollideInfo = std::unordered_map<Collidable*, std::list<Collidable*>>;
		using SendCollideInfo = std::list<Tuple<Collidable*, Collidable*>>;
		struct Data
		{
			void* t1;
			void* t2;
		};
//		using SendCollideInfo = std::list<OnCollideInfoData>;
	private:
		Physics();

		Physics(const Physics&) = delete;
		void operator= (const Physics&) = delete;
	public:
		static Physics& GetInstance();

		void Init(unsigned int level, const Vec3& min, const Vec3& max);
		void Entry(std::shared_ptr<Collidable> collidable);
		void Exit(std::shared_ptr<Collidable> collidable);

		void Update();

		void SetTreeMinMax(const Vec3& min, const Vec3& max);

	private:
		void MoveNextPos() const;

		void CheckCollide();

		bool IsCollide(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB);
		void FixNextPos(const Rigidbody& primaryRigid, Rigidbody& secondaryRigid, ColliderBase* primaryCollider, ColliderBase* secondaryCollider);
		void AddNewCollideInfo(Collidable* objA, Collidable* objB, SendCollideInfo& info);
		void CheckSendOnCollideInfo(SendCollideInfo& preSendInfo, SendCollideInfo& newSendInfo, bool isTrigger);
		void AddOnCollideInfo(Collidable* own, Collidable* send, OnCollideInfoKind kind);
		void OnCollideInfo(Collidable* own, Collidable* send, OnCollideInfoKind kind);
		void FixPos() const;

	private:
		std::list<std::shared_ptr<Collidable>> m_collidables;
		std::list<OnCollideInfoData> m_onCollideInfo;
		SendCollideInfo m_newCollideInfo;
		SendCollideInfo m_preCollideInfo;
		SendCollideInfo m_newTirrigerInfo;
		SendCollideInfo m_preTirrigerInfo;

		std::list<ColliderTreeObject*> m_oftArr;
		std::shared_ptr<CLiner8TreeManager> m_l8Tree;
	};
}

