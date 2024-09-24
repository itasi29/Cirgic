#include "ColliderCapsule.h"

using namespace MyEngine;

ColliderCapsule::ColliderCapsule() :
	ColliderBase(ColliderBase::Kind::Capsule),
	dir(Vec3(0, 0, 1)),
	radius(0.0f),
	size(0.0f)
{
}

void MyEngine::ColliderCapsule::Init(const Vec3& center, const Vec3& dir, float radius, float size, bool isTrigger)
{
	this->center = center;
	this->dir = dir;
	this->radius = radius;
	this->size = size;
	this->isTrigger = isTrigger;
}

void MyEngine::ColliderCapsule::SetMinMax(const Vec3& center)
{
	// ’·‚³
	auto s = radius + size;
	auto t = s * s * 3;
	float len = std::sqrtf(t);
	// Žè‘O¶‰º
	const auto& minDir = Vec3(-1, -1, -1).GetNormalized();
	min = this->center + center + minDir * len;
	// ‰œ‰Eã
	const auto& maxDir = Vec3(1, 1, 1).GetNormalized();
	max = this->center + center + maxDir * len;
}
