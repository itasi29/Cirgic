#pragma once
#include <memory>
#include <vector>

struct Vec3;
class CheckPoint;
class RaceManager;

class CheckPointManager final
{
public:
	CheckPointManager();

	void Init(const std::weak_ptr<RaceManager>& raceMgr);
	void InitPlayerNum(int num);
	void End();

	void UpdateCheckPoint(int carNo, int cpNo);

	int GetNowCpNo(int carNo) const;
	int GetCpNum() const;
	const Vec3& GetNowCpPos(int carNo) const;
	const Vec3& GetNextCpPos(int carNo) const;

	void DebugDraw() const;

private:
	void LoadAndCreate(const wchar_t* path);

private:
	std::weak_ptr<RaceManager> m_raceMgr;
	// CPの位置情報
	std::vector<std::shared_ptr<CheckPoint>> m_cps;
	// 必ず通らなければならないCP番号
	std::vector<int> m_mustCp;

	// 車別の通ったCPの場所
	std::vector<int> m_carCp;
	// 車別の絶対に通るべきCPのインデックス番号
	std::vector<int> m_carMustPtIndex;
};

