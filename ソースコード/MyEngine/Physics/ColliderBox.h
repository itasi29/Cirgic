#pragma once
#include "ColliderBase.h"
#include "Geometry/Vec3.h"
#include "Geometry/Quaternion.h"

namespace MyEngine
{
	class ColliderBox : public ColliderBase
	{
	public:
		ColliderBox();

		void SetMinMax(const Vec3& center) override;

	public:
		Vec3 size;
		Quaternion rotation;
	};
}

