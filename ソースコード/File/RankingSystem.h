#pragma once
#include <vector>

class RankingSystem
{
private:
	RankingSystem();

	RankingSystem(const RankingSystem& mgr) = delete;
	void operator= (const RankingSystem& mgr) = delete;

public:
	~RankingSystem();

	static RankingSystem& GetInstance();

	void Init();
	void End();

	bool Update(int totalTime, int rapTime);

	int GetTime(int rank) const;

private:
	void Load(const char* const path, std::vector<int>& data);
	void Save(const char* const path, const std::vector<int>& data) const;

	bool Update(int time, std::vector<int>& rnk);

private:
	std::vector<int> m_totalRanking;
	std::vector<int> m_rapRanking;
};

