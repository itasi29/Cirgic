#pragma once
#include "ColliderBase.h"
#include "Geometry/Vec3.h"

namespace MyEngine
{
	class ColliderCapsule : public ColliderBase
	{
	public:
		ColliderCapsule();

		void Init(const Vec3& center, const Vec3& dir, float radius, float size, bool isTrigger);
		void SetMinMax(const Vec3& center) override;

		void SetFixS(bool isFix) { m_isFixS = isFix; }
		void SetFixE(bool isFix) { m_isFixE = isFix; }

		bool IsFixS() const { return m_isFixS; }
		bool IsFixE() const { return m_isFixE; }

	public:
		Vec3 dir;
		float radius;
		float size;

	private:
		bool m_isFixS;
		bool m_isFixE;
	};
}