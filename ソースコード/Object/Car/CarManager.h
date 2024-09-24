#pragma once
#include <vector>
#include <memory>
#include "Geometry/Vec3.h"

class CarBase;
class ItemManager;
class CheckPointManager;
class RaceManager;

class CarManager final
{
public:
	CarManager();
	~CarManager();

	void Init(ItemManager& itemMgr, CheckPointManager& cpMgr, const std::shared_ptr<RaceManager>& raceMgr, int playerNum);
	void End();
	void Update();
	void Draw() const;

	int GetCoinNum(int carNo) const;
	Vec3 GetCarPos(int carNo) const;

private:
	std::vector<std::shared_ptr<CarBase>> m_cars;
	std::shared_ptr<RaceManager> m_raceMgr;
};

