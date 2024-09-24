#pragma once
#include "Object/Object3D.h"

namespace MyEngine
{
	class ColliderTerrain;
}

class SystemWall : public Object3D
{
public:
	SystemWall();

	void Init(const Vec3& pos, const Quaternion& rot, const Vec3& norm, float width, float height);

	void Draw() const override;

private:
	MyEngine::ColliderTerrain* m_terrain;
};

