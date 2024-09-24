#include "ColliderTerrain.h"
#include "Common/Game.h"

MyEngine::ColliderTerrain::ColliderTerrain() : 
	ColliderBase(Kind::Terrain)
{
}

void MyEngine::ColliderTerrain::Init(const Vec3& center, const Quaternion& rot, const Vec3& norm, float width, float length, bool isTrigger)
{
	this->norm = norm;
	this->right = this->rotation * Vec3::Right();
	this->front = this->rotation * Vec3::Front();
	this->rotation = rot;
	this->size = Vec2(width, length);
	this->isTrigger = isTrigger;
}

void MyEngine::ColliderTerrain::Init(const Vec3& center, const Vec3& norm, float width, float length, bool isTrigger)
{
	auto N = norm.GetNormalized();
	this->norm = N;
	float angle = std::acosf(Dot(N, Vec3::Up()));
	auto d = angle * Game::RAD_2_DEG;
	auto axis = Cross(Vec3::Up(), N);
	this->rotation = AngleAxis(d, axis);
	this->size = Vec2(width, length);
	this->isTrigger = isTrigger;
}

void MyEngine::ColliderTerrain::SetMinMax(const Vec3& center)
{
	auto vec = right * size.x + front * size.y;
	min = this->center + center - vec;
	max = this->center + center + vec;
}
