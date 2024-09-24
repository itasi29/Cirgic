#pragma once
#include <memory>

class RaceManager;
class StageManager;
class ItemManager;
class CheckPointManager;
class CarManager;

class ObjectManager final
{
public:
	ObjectManager();
	~ObjectManager();

	void Init(const std::shared_ptr<RaceManager>& raceMgr, int playerNum);
	void End();
	void Update();
	void Draw(int no) const;

	const std::shared_ptr<CheckPointManager>& GetCpMgr() const { return m_cpMgr; }
	const std::shared_ptr<CarManager>& GetCarMgr() const { return m_carMgr; }

private:
	std::shared_ptr<StageManager> m_stageMgr;
	std::shared_ptr<ItemManager> m_itemMgr;
	std::shared_ptr<CheckPointManager> m_cpMgr;
	std::shared_ptr<CarManager> m_carMgr;
};

