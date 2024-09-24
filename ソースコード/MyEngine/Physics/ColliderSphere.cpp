#include "ColliderSphere.h"

using namespace MyEngine;

ColliderSphere::ColliderSphere() :
	ColliderBase(ColliderBase::Kind::Sphere),
	radius(0.0f)
{
}

void MyEngine::ColliderSphere::SetMinMax(const Vec3& center)
{
	// 長さ
	float len = std::cbrtf(radius * radius * 3);
	// 手前左下
	const auto& minDir = Vec3(-1, -1, -1).GetNormalized();
	min = this->center + center + minDir * len;
	// 奥右上
	const auto& maxDir = Vec3(1, 1, 1).GetNormalized();
	max = this->center + center + maxDir * len;
}
