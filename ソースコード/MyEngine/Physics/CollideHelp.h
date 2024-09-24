#pragma once
#include "Geometry/Vec3.h"

namespace MyEngine
{
	class Rigidbody;
	class ColliderBase;

	bool IsCollideSphereAndSphere(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB);
	bool IsCollideSphereAndCapsule(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB);
	bool IsCollideSphereAndTerrain(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB);
	bool IsCollideSphereAndBox(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB);
	bool IsCollideCapsuleAndCapsule(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB);
	bool IsCollideCapsuleAndTerrain(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB);
	bool IsCollideCapsuleAndBox(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB);
	bool IsCollideSurfaceAndSurface(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB);

	Vec3 FixSphereToSphere(const Rigidbody& primaryRigid, const Rigidbody& secondaryRigid, ColliderBase* primaryCollider, ColliderBase* secondaryCollider);
	Vec3 FixCapsuleToCapsule(const Rigidbody& primaryRigid, const Rigidbody& secondaryRigid, ColliderBase* primaryCollider, ColliderBase* secondaryCollider);
	Vec3 FixTerrainToSphere(const Rigidbody& primaryRigid, const Rigidbody& secondaryRigid, ColliderBase* primaryCollider, ColliderBase* secondaryCollider);
	Vec3 FixTerrainToCapsule(const Rigidbody& primaryRigid, const Rigidbody& secondaryRigid, ColliderBase* primaryCollider, ColliderBase* secondaryCollider);
}