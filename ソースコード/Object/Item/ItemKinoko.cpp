#include "ItemKinoko.h"
#include "Object/Car/CarBase.h"

namespace
{
	constexpr float DASH_POWER = 5.0f;
}

ItemKinoko::ItemKinoko(ItemManager& itemMgr) :
	ItemBase(Kind::KINOKO, itemMgr)
{
}

ItemKinoko::ItemKinoko(Kind kind, ItemManager& itemMgr) :
	ItemBase(kind, itemMgr)
{
}

ItemKinoko::~ItemKinoko()
{
}

void ItemKinoko::Use(CarBase* car)
{
	car->OnDash(DASH_POWER);
}

void ItemKinoko::Hit(CarBase* car)
{
	Use(car);
}
