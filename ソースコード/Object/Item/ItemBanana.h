#pragma once
#include "ItemBase.h"

class ItemBanana : public ItemBase
{
public:
	ItemBanana(ItemManager& itemMgr);
	~ItemBanana();

	void Use(CarBase* car) override;
	void Hit(CarBase* car) override;
	void Release(CarBase* car, float power) override;

	void OnCollideEnter(Collidable* colider) override;
};

