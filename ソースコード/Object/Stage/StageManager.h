#pragma once
#include <list>
#include <vector>
#include <memory>
#include "Geometry/Vec3.h"
#include "Geometry/Quaternion.h"

class Object3D;
class SystemWall;

class StageManager final
{
public:
	StageManager();
	~StageManager();

	void Init();
	void End();
	void Update();
	void Draw() const;

private:
	void LoadAndCreateRoadCollider(const wchar_t* path);
	void LoadAndCreateRoad(const wchar_t* path);
	void LoadAndCreateWallCollider(const wchar_t* path);
	void LoadAndCreateObjects(const wchar_t* path, const wchar_t* name);
	void LoadAndCreateObject(const wchar_t* name, const Vec3& pos, const Quaternion& rot, const Vec3& scale);

private:
	std::list<std::shared_ptr<Object3D>> m_objects;
};

