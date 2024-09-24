#pragma once
#include "ItemCarapaceBase.h"

class ItemCarapaceGreen : public ItemCarapaceBase
{
public:
	ItemCarapaceGreen(ItemManager& itemMgr);
	~ItemCarapaceGreen();

	void OnCollideEnter(Collidable* colider) override;

private:
	int m_boundCount;
};

