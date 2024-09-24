#include "ColliderSphere.h"

using namespace MyEngine;

ColliderSphere::ColliderSphere() :
	ColliderBase(ColliderBase::Kind::Sphere),
	radius(0.0f)
{
}

void MyEngine::ColliderSphere::SetMinMax(const Vec3& center)
{
	// ’·‚³
	float len = std::cbrtf(radius * radius * 3);
	// Žè‘O¶‰º
	const auto& minDir = Vec3(-1, -1, -1).GetNormalized();
	min = this->center + center + minDir * len;
	// ‰œ‰Eã
	const auto& maxDir = Vec3(1, 1, 1).GetNormalized();
	max = this->center + center + maxDir * len;
}
