#pragma once
#include "MyEngine/Physics/Collidable.h"

namespace MyEngine
{
	class ColliderTerrain;
}
struct Vec3;
struct Quaternion;

class CheckPoint : public MyEngine::Collidable
{
public:
	enum class Kind
	{
		NORMAL,
		MUST,
		GOAL
	};

public:
	CheckPoint();
	virtual ~CheckPoint();

	void Init(const Vec3& pos, const Quaternion& rot, const Vec3& norm, float width, float height, Kind kind, int no);
	void End();

	void DebugDraw() const;

	int GetNo() const { return m_no; }
	const Vec3& GetPos() const;

protected:
	int m_no;

private:
	Kind m_kind;
	MyEngine::ColliderTerrain* m_terrain;
};

