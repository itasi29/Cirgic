#include "StageManager.h"
#include <cassert>
#include "Road.h"
#include "JumpPoint.h"
#include "SystemWall.h"
#include "File/FileID.h"

namespace
{
	struct RoadColliderData
	{
		Vec3 pos;
		Quaternion rot;
		Vec3 front;
		Vec3 norm;
		float widht;
		float length;
	};
	struct RoadData
	{
		int kind;
		Vec3 pos;
		Quaternion rot;
		Vec3 size;
	};
	struct WallData
	{
		Vec3 pos;
		Quaternion rot;
		Vec3 norm;
		float height;
		float width;
	};
	struct ObjectData
	{
		Vec3 pos;
		Quaternion rot;
		Vec3 size;
	};

	enum RoadKind : int
	{
		Straight,
		Curve,
		SCurve
	};

}

StageManager::StageManager()
{
}

StageManager::~StageManager()
{
}

void StageManager::Init()
{
	//auto jp = std::make_shared<JumpPoint>();
	//m_objects.emplace_back(jp);

	LoadAndCreateWallCollider(L"Data/Stage/Main/WallInside.col");
	LoadAndCreateWallCollider(L"Data/Stage/Main/WallOutside.col");

	auto loadCol = std::make_shared<Road>();
	loadCol->Init(Vec3(0, -0.1f, 0), Quaternion(0, 0, 0, 1), Vec3::Up(), 20000, 20000, 0.6f, 0.4f, 0xff0000);
	m_objects.emplace_back(loadCol);
	LoadAndCreateRoadCollider(L"Data/Stage/Main/RoadCenter.col");


	for (auto& obj : m_objects)
	{
		obj->Entry();
	}

	LoadAndCreateRoad(L"Data/Stage/Main/Road.loc");
	LoadAndCreateObjects(L"Data/Stage/Main/Ground.loc", FileID::Mdl::GROUND);
	LoadAndCreateObjects(L"Data/Stage/Main/Stand.loc", FileID::Mdl::STAND);
	LoadAndCreateObjects(L"Data/Stage/Main/Fence1.loc", FileID::Mdl::FENCE_1);
	LoadAndCreateObjects(L"Data/Stage/Main/Fence2.loc", FileID::Mdl::FENCE_2);
	LoadAndCreateObjects(L"Data/Stage/Main/Tire.loc", FileID::Mdl::TIRE);
	LoadAndCreateObjects(L"Data/Stage/Main/Arch.loc", FileID::Mdl::ARCH_NORMAL);
	LoadAndCreateObject(FileID::Mdl::ARCH_GOAL, Vec3(0, 0, 0), AngleAxis(180, Vec3::Up()), Vec3(0.01f, 0.01f, 0.01f));
}

void StageManager::End()
{
	for (auto& obj : m_objects)
	{
		obj->End();
	}
}

void StageManager::Update()
{
	for (auto& obj : m_objects)
	{
		obj->Update();
	}
}

void StageManager::Draw() const
{
	for (auto& obj : m_objects)
	{
		obj->Draw();
	}
}

void StageManager::LoadAndCreateRoadCollider(const wchar_t* path)
{
	SetUseASyncLoadFlag(false);
	
	int handle = FileRead_open(path);

	if (handle != 0)
	{
		int size;
		FileRead_read(&size, sizeof(int), handle);

		float f = 0.5f;
		float maxRate = 1.0f;

		for (int i = 0; i < size; ++i)
		{
			RoadColliderData data;
			FileRead_read(&data, sizeof(data), handle);

			auto loadCol = std::make_shared<Road>();
			loadCol->Init(data.pos, data.rot, data.norm, data.widht, data.length, f, maxRate, 0x00ff00);
			m_objects.emplace_back(loadCol);
		}

		FileRead_close(handle);
	}
	else
	{
		assert(false && "道データのファイルパスが違います");
	}

	SetUseASyncLoadFlag(true);
}

void StageManager::LoadAndCreateRoad(const wchar_t* path)
{
	SetUseASyncLoadFlag(false);

	int handle = FileRead_open(path);

	if (handle != 0)
	{
		int size;
		FileRead_read(&size, sizeof(int), handle);

		for (int i = 0; i < size; ++i)
		{
			RoadData data;
			FileRead_read(&data, sizeof(data), handle);

			const wchar_t* path;
			if (data.kind == RoadKind::Straight)
			{
				path = FileID::Mdl::LOAD_STRAIGHT;
			}
			else if (data.kind == RoadKind::Curve)
			{
				path = FileID::Mdl::LOAD_CURVE;
			}
			else if (data.kind == RoadKind::SCurve)
			{
				path = FileID::Mdl::LOAD_SCURVE;
			}
			auto load = std::make_shared<Object3D>(MyEngine::Collidable::Priority::STATIC, ObjectTag::NONE, path);
			load->Init(data.pos, data.rot, data.size);
			m_objects.emplace_back(load);
		}

		FileRead_close(handle);
	}
	else
	{
		assert(false && "道データのファイルパスが違います");
	}

	SetUseASyncLoadFlag(true);
}

void StageManager::LoadAndCreateWallCollider(const wchar_t* path)
{
	SetUseASyncLoadFlag(false);

	int handle = FileRead_open(path);

	if (handle != 0)
	{
		int size;
		FileRead_read(&size, sizeof(int), handle);

		for (int i = 0; i < size; ++i)
		{
			WallData data;
			FileRead_read(&data, sizeof(data), handle);

			auto load = std::make_shared<SystemWall>();
			load->Init(data.pos, data.rot, data.norm, data.width, data.height);
			m_objects.emplace_back(load);
		}

		FileRead_close(handle);
	}
	else
	{
		assert(false && "道データのファイルパスが違います");
	}

	SetUseASyncLoadFlag(true);
}

void StageManager::LoadAndCreateObjects(const wchar_t* path, const wchar_t* name)
{
	SetUseASyncLoadFlag(false);

	int handle = FileRead_open(path);
	
	if (handle != 0)
	{
		int size;
		FileRead_read(&size, sizeof(int), handle);

		for (int i = 0; i < size; ++i)
		{
			ObjectData data;
			FileRead_read(&data, sizeof(data), handle);

			auto load = std::make_shared<Object3D>(MyEngine::Collidable::Priority::STATIC, ObjectTag::NONE, name);
			load->Init(data.pos, data.rot, data.size);
			m_objects.emplace_back(load);
		}

		FileRead_close(handle);
	}
	else
	{
		assert(false && "道データのファイルパスが違います");
	}

	SetUseASyncLoadFlag(true);
}

void StageManager::LoadAndCreateObject(const wchar_t* name, const Vec3& pos, const Quaternion& rot, const Vec3& scale)
{
	auto obj = std::make_shared<Object3D>(MyEngine::Collidable::Priority::STATIC, ObjectTag::NONE, name);
	obj->Init(pos, rot, scale);
	m_objects.emplace_back(obj);
}