#pragma once
#include "ColliderBase.h"
#include "Geometry/Vec2.h"
#include "Geometry/Vec3.h"
#include "Geometry/Quaternion.h"

namespace MyEngine
{
	class ColliderTerrain : public ColliderBase
	{
	public:
		ColliderTerrain();
		~ColliderTerrain() {}

		void Init(const Vec3& center, const Quaternion& rot, const Vec3& norm, float width, float length, bool isTrigger);
		void Init(const Vec3& center, const Vec3& norm, float width, float length, bool isTrigger);
		void SetMinMax(const Vec3& center) override;

	public:
		Vec3 norm;
		Vec3 front;
		Vec3 right;
		Quaternion rotation;
		Vec2 size;
	};
}

