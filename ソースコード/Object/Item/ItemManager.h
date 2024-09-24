#pragma once
#include <memory>
#include <list>
#include "Geometry/Vec3.h"
#include "Geometry/Quaternion.h"

namespace MyEngine
{
	class Collidable;
}
class ItemBase;
class ItemBox;
class FileBase;
class RaceManager;

class ItemManager final
{
	struct RecreateCoinData
	{
		Vec3 pos;
		Vec3 scale;
		int frame = 0;
		bool isDestory = false;
	};
	struct RecreateBoxData
	{
		Vec3 pos;
		Quaternion rot;
		int frame = 0;
		bool isDestory = false;
	};
	struct ItemGetRateData
	{
		int standing;
		std::vector<int> rateData;
	};

public:
	ItemManager();
	~ItemManager();

	void Init(const std::weak_ptr<RaceManager>& raceMgr, int playerNum);
	void End();
	void Update();
	void Draw() const;

	void AddEntity(std::shared_ptr<MyEngine::Collidable> item);

	void RecreateCoin(const Vec3& pos, const Vec3& scale);
	void RecreateBox(const Vec3& pos, const Quaternion& rot);

	std::shared_ptr<ItemBase> GetItem(int carNo);

private:
	std::shared_ptr<ItemBase> RandCreate(int carNo);
	std::shared_ptr<ItemBase> SimpleCreate(int carNo);

	void LoadItemGetRateData(const wchar_t* const path);
	void LoadAndCreateCoin(const wchar_t* const path);
	void LoadAndCreateItemBox(const wchar_t* const path);

private:
	std::weak_ptr<RaceManager> m_raceMgr;

	std::vector<ItemGetRateData> m_itemRateData;

	std::shared_ptr<FileBase> m_getSe;
	std::list<std::shared_ptr<ItemBase>> m_entityItems;
	std::list<std::shared_ptr<ItemBox>> m_itemBoxs;

	std::list<RecreateCoinData> m_recreateCoinData;
	std::list<RecreateBoxData> m_recreateBoxData;

	using CreateFunc_t = std::shared_ptr<ItemBase>(ItemManager::*)(int);
	CreateFunc_t m_createFunc;
};

