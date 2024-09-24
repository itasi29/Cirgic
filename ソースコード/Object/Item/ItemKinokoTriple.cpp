#include "ItemKinokoTriple.h"
#include "Object/Car/CarBase.h"

namespace
{
	constexpr int USE_NUM = 3;
}

ItemKinokoTriple::ItemKinokoTriple(ItemManager& itemMgr) :
	ItemKinoko(Kind::KINOKO_TRIPLE, itemMgr),
	m_count(USE_NUM)
{
}

ItemKinokoTriple::~ItemKinokoTriple()
{
}

void ItemKinokoTriple::Use(CarBase* car)
{
	ItemKinoko::Use(car);
	--m_count;
}
