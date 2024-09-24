#include "CarManager.h"
#include "CarPlayer.h"
#include "Race/RaceManager.h"

namespace
{
	const Vec3 INIT_POS[4] = {
		Vec3(-75, 0, -100),
		Vec3(75, 0, -200),
		Vec3(-75, 0, -300),
		Vec3(75, 0, -400)
	};

	constexpr float BOOST_RATE_STANDING = 0.05f;
}

CarManager::CarManager()
{
}

CarManager::~CarManager()
{
}

void CarManager::Init(ItemManager& itemMgr, CheckPointManager& cpMgr, const std::shared_ptr<RaceManager>& raceMgr, int playerNum)
{
	for (int i = 0; i < playerNum; ++i)
	{
		auto car = std::make_shared<CarPlayer>(itemMgr, cpMgr, INIT_POS[i]);
		car->InitCarNo(i);
		car->InitPadNo(i);
		car->Entry();
		m_cars.emplace_back(car);
		raceMgr->AddCar(car);
	}
	m_raceMgr = raceMgr;
}

void CarManager::End()
{
	for (auto& car : m_cars)
	{
		car->End();
	}
}

void CarManager::Update()
{
	int i = 0;
	for (auto& car : m_cars)
	{
		car->SetStandingSpeedRate(m_raceMgr->GetStanding(i) * BOOST_RATE_STANDING + 1.0f);
		car->Update();
		++i;
	}
}

void CarManager::Draw() const
{
	for (auto& car : m_cars)
	{
		car->Draw();
	}
}

int CarManager::GetCoinNum(int carNo) const
{
	return m_cars[carNo]->GetCoinNum();
}

Vec3 CarManager::GetCarPos(int carNo) const
{
	return m_cars[carNo]->GetPos();
}
