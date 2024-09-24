#pragma once
#include "ItemBase.h"

class ItemKinoko : public ItemBase
{
public:
	ItemKinoko(ItemManager& itemMgr);
	ItemKinoko(Kind kind, ItemManager& itemMgr);
	~ItemKinoko();

	virtual void Use(CarBase* car) override;
	void Hit(CarBase* car) override;
};

