#include "RankingSystem.h"
#include <DxLib.h>
#include "Utility/StringUtility.h"

namespace
{
	const char* const PATH_RAP = "Data/SaveData/RapRanking.dat";
	const char* const PATH_TOTAL = "Data/SaveData/TotalRanking.dat";
}

RankingSystem::RankingSystem()
{
}

RankingSystem::~RankingSystem()
{
}

RankingSystem& RankingSystem::GetInstance()
{
	static RankingSystem sys;
	return sys;
}

void RankingSystem::Init()
{
	Load(PATH_RAP, m_rapRanking);
	Load(PATH_TOTAL, m_totalRanking);
}

void RankingSystem::End()
{
	Save(PATH_RAP, m_rapRanking);
	Save(PATH_TOTAL, m_totalRanking);
}

bool RankingSystem::Update(int totalTime, int rapTime)
{
	bool isTotal = Update(totalTime, m_totalRanking);
	bool isRap = Update(rapTime, m_rapRanking);
	return isTotal || isRap;
}

int RankingSystem::GetTime(int rank) const
{
	return m_totalRanking[rank];
}

void RankingSystem::Load(const char* const path, std::vector<int>& data)
{
	SetUseASyncLoadFlag(false);

	auto wPath = StringUtility::StringToWString(path);
	int handle = FileRead_open(wPath.c_str());

	data.resize(10);
	if (handle != 0)
	{
		for (int i = 0; i < 10; ++i)
		{
			auto& item = data[i];
			FileRead_read(&item, sizeof(item), handle);
		}
	}
	else
	{
		for (int i = 0; i < 10; ++i)
		{
			data[i] = 9999999;
		}
	}

	FileRead_close(handle);

	SetUseASyncLoadFlag(true);
}

void RankingSystem::Save(const char* const path, const std::vector<int>& data) const
{
	FILE* fp = nullptr;
	
	auto err = fopen_s(&fp, path, "wb");

	for (auto& item : data)
	{
		fwrite(&item, sizeof(item), 1, fp);
	}

	fclose(fp);
}

bool RankingSystem::Update(int time, std::vector<int>& rnk)
{
	bool isUpdate = false;

	for (int i = 0; i < 10; ++i)
	{
		if (time < rnk[i])
		{
			for (int j = 9; j > i; --j)
			{
				rnk[j] = rnk[j - 1];
			}
			rnk[i] = time;
			isUpdate = true;
			break;
		}
	}

	return isUpdate;
}
