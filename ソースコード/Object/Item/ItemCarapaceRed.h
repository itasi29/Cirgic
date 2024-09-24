#pragma once
#include "ItemCarapaceBase.h"

class RaceManager;

class ItemCarapaceRed : public ItemCarapaceBase
{
public:
	ItemCarapaceRed(ItemManager& itemMgr, const std::weak_ptr<RaceManager>& raceMgr);
	~ItemCarapaceRed();

	void Update() override;

	void Release(CarBase* car, float power) override;

	void OnCollideEnter(Collidable* colider) override;

private:
	std::weak_ptr<RaceManager> m_raceMgr;

	bool m_isHoming;
	bool m_isInHomingDis;
	std::shared_ptr<CarBase> m_homingCar;
};

