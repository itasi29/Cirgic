#include "ColliderSphere.h"

using namespace MyEngine;

ColliderSphere::ColliderSphere() :
	ColliderBase(ColliderBase::Kind::Sphere),
	radius(0.0f)
{
}

void MyEngine::ColliderSphere::SetMinMax(const Vec3& center)
{
	// ����
	float len = std::cbrtf(radius * radius * 3);
	// ��O����
	const auto& minDir = Vec3(-1, -1, -1).GetNormalized();
	min = this->center + center + minDir * len;
	// ���E��
	const auto& maxDir = Vec3(1, 1, 1).GetNormalized();
	max = this->center + center + maxDir * len;
}
