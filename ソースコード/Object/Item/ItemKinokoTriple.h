#pragma once
#include "ItemKinoko.h"

class ItemKinokoTriple : public ItemKinoko
{
public:
	ItemKinokoTriple(ItemManager& itemMgr);
	~ItemKinokoTriple();

	void Use(CarBase* car) override;

	int GetCount() const { return m_count; }

private:
	int m_count;
};

