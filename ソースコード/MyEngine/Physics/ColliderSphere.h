#pragma once
#include "ColliderBase.h"

namespace MyEngine
{
	class ColliderSphere : public ColliderBase
	{
	public:
		ColliderSphere();

		void SetMinMax(const Vec3& center) override;

	public:
		float radius;
	};
}

