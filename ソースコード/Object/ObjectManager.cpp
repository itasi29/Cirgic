#include "ObjectManager.h"
#include "Stage/StageManager.h"
#include "Item/ItemManager.h"
#include "CheckPoint/CheckPointManager.h"
#include "Car/CarManager.h"
#include "Common/Camera.h"

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::Init(const std::shared_ptr<RaceManager>& raceMgr, int playerNum)
{

	m_stageMgr = std::make_shared<StageManager>();
	m_itemMgr = std::make_shared<ItemManager>();
	m_cpMgr = std::make_shared<CheckPointManager>();
	m_carMgr = std::make_shared<CarManager>();

	m_carMgr->Init(*m_itemMgr, *m_cpMgr, raceMgr, playerNum);
	m_stageMgr->Init();
	m_itemMgr->Init(raceMgr, playerNum);
	m_cpMgr->Init(raceMgr);

	m_cpMgr->InitPlayerNum(playerNum);
}

void ObjectManager::End()
{
	m_stageMgr->End();
	m_itemMgr->End();
	m_cpMgr->End();
	m_carMgr->End();
}

void ObjectManager::Update()
{
	m_stageMgr->Update();
	m_itemMgr->Update();
	m_carMgr->Update();
}

void ObjectManager::Draw(int no) const
{
	auto& camera = Camera::GetInstance(); 
	camera.AppInfo(no);

	m_stageMgr->Draw();
	m_itemMgr->Draw();
#ifdef _DEBUG
	m_cpMgr->DebugDraw();
#endif
	m_carMgr->Draw();
}
