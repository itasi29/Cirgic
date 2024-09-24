#pragma once
#include <memory>
#include <vector>
#include <array>
#include "Geometry/Vec3.h"

class FileBase;
class ItemBase;
class CheckPointManager;
class CarManager;
class CarBase;

struct RaceData
{
	// ラップ数
	int rap;
	// 順位
	int stading;
	// ゴールフラグ
	bool isGoal;
	// 時間
	std::array<int, 3> time;
	// ラップ更新フラグ
	bool isRapUpdate;
};

class RaceManager final
{
public:
	RaceManager();
	~RaceManager();

	void Init(const std::weak_ptr<CheckPointManager>& cpMgr, const std::weak_ptr<CarManager>& carMgr, int playerNum);
	void Update();

	void UpdateRap(int carNo);

	void AddCar(const std::shared_ptr<CarBase>& car);

	bool IsGoal(int carNo) const;
	bool HasItem(int carNo) const;
	bool IsRapUpdate(int carNo) const;
	int GetTime(int carNo) const;
	int GetStanding(int carNo) const;
	int GetRap(int carNo) const;
	int Get1StandingCarNo() const;
	std::vector<RaceData> GetData() const { return m_carInfos; }
	std::shared_ptr<CarBase> GetCarToStanding(int standing) const;
	std::shared_ptr<ItemBase> GetItem(int carNo) const;
	Vec3 GetCarPos(int carNo) const;

	void CarAllGaol() const;

private:
	void UpdateStanding();
	void CheckStandingCP(std::vector<int> datas, int& nowStanding);

	float GetNextSqDistance(int carNo) const;

private:
	std::shared_ptr<FileBase> m_rapUpdateSe;
	std::weak_ptr<CheckPointManager> m_cpMgr;
	std::weak_ptr<CarManager> m_carMgr;
	std::vector<RaceData> m_carInfos;
	std::vector<std::shared_ptr<CarBase>> m_cars;
};

