#pragma once
#include "ColliderBase.h"

namespace MyEngine
{
	class ColliderLine : public ColliderBase
	{
	public:
		ColliderLine();

		void SetMinMax(const Vec3& center) override;
	};
}

