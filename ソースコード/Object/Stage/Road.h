#pragma once
#include "Object/Object3D.h"

namespace MyEngine
{
	class ColliderTerrain;
}

struct Vec3;
struct Quaternion;

class Road : public Object3D
{
public:
	Road();

	void Init(const Vec3& pos, const Quaternion& rot, const Vec3& norm, float width, float length, float friction, float rate, unsigned int color);
	void Draw() const override;

	void SetFriction(float friction) { m_friction = friction; }
	void SetMaxSpeedRate(float rate) { m_maxSpeedRate = rate; }
	float GetFriction() const { return m_friction; }
	float GetMaxSpeedRate() const { return m_maxSpeedRate; }

private:
	MyEngine::ColliderTerrain* m_terrain;
	// –€C—Í
	float m_friction;
	float m_maxSpeedRate;

	// TODO: ˆê•Ï”‚Ì‚½‚ßÁ‚·
	unsigned int m_color;
};

