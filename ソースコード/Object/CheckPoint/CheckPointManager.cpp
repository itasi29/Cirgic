#include "CheckPointManager.h"
#include <cassert>
#include "CheckPoint.h"
#include "Common/Game.h"
#include "Geometry/Vec3.h"
#include "Geometry/Quaternion.h"
#include "Race/RaceManager.h"

namespace
{
	struct CPData
	{
		Vec3 pos;
		Quaternion rot;
		Vec3 norm;
		float height;
		float width;
		int isMust;
	};
}

CheckPointManager::CheckPointManager()
{
}

void CheckPointManager::Init(const std::weak_ptr<RaceManager>& raceMgr)
{
	m_raceMgr = raceMgr;

	LoadAndCreate(L"Data/Stage/Main/CheckPoint.dat");
}

void CheckPointManager::InitPlayerNum(int num)
{
	m_carCp.resize(num);
	m_carMustPtIndex.resize(num);
	for (int i = 0; i < num; ++i)
	{
		m_carCp[i] = 0;
		m_carMustPtIndex[i] = 0;
	}
}

void CheckPointManager::End()
{
	for (auto& cp : m_cps)
	{
		cp->End();
	}
}

void CheckPointManager::UpdateCheckPoint(int carNo, int cpNo)
{
	auto& index = m_carMustPtIndex[carNo];

	// 最後の必ず通らなければならないCPを通った後
	if (index >= m_mustCp.size())
	{
		// もしゴール(てか0番)ならラップ増加、CP情報初期化
		if (cpNo == 0)
		{
			m_carCp[carNo] = 0;
			m_carMustPtIndex[carNo] = 0;

			m_raceMgr.lock()->UpdateRap(carNo);

			return;
		}
		// 現在のCP番号より大きいなら更新
		else if (m_carCp[carNo] < cpNo)
		{
			m_carCp[carNo] = cpNo;
		}
	}
	// 前
	else
	{
		// 現在のCP番号より小さいなら更新しない
		if (m_carCp[carNo] > cpNo) return;

		// 通ったCPがマストのCPより小さければ通る
		if (cpNo <= m_mustCp[index])
		{
			// CP更新
			m_carCp[carNo] = cpNo;

			// 通ったCPとマストのCPが同じならインデックス番号を増やす
			if (cpNo == m_mustCp[index])
			{
				++index;
			}

			return;
		}
	}
}

int CheckPointManager::GetNowCpNo(int carNo) const
{
	return m_carCp.at(carNo);
}

int CheckPointManager::GetCpNum() const
{
	return static_cast<int>(m_cps.size());
}

const Vec3& CheckPointManager::GetNowCpPos(int carNo) const
{
	return m_cps.at(GetNowCpNo(carNo))->GetPos();
}

const Vec3& CheckPointManager::GetNextCpPos(int carNo) const
{
	int nextCp = (GetNowCpNo(carNo) + 1) % m_cps.size();
	return m_cps.at(nextCp)->GetPos();
}

void CheckPointManager::DebugDraw() const
{
	for (auto& cp : m_cps)
	{
		cp->DebugDraw();
	}
	
}

void CheckPointManager::LoadAndCreate(const wchar_t* path)
{
	SetUseASyncLoadFlag(false);

	int handle = FileRead_open(path);

	if (handle != 0)
	{
		int size;
		FileRead_read(&size, sizeof(int), handle);

		float f = 0.5f;
		float maxRate = 1.0f;

		for (int i = 0; i < size; ++i)
		{
			CPData data;
			FileRead_read(&data, sizeof(data), handle);

			CheckPoint::Kind kind;
			if (i == 0)
			{
				kind = CheckPoint::Kind::GOAL;
			}
			else
			{
				if (data.isMust)
				{
					kind = CheckPoint::Kind::MUST;
					m_mustCp.push_back(i);
				}
				else
				{
					kind = CheckPoint::Kind::NORMAL;
				}
			}

			auto cpCol = std::make_shared<CheckPoint>();
			cpCol->Init(data.pos, data.rot, data.norm, data.height, data.width, kind, i);
			m_cps.emplace_back(cpCol);
		}

		FileRead_close(handle);
	}
	else
	{
		assert(false && "道データのファイルパスが違います");
	}

	SetUseASyncLoadFlag(true);
}
