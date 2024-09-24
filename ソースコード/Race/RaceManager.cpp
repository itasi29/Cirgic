#include "RaceManager.h"
#include <cassert>
#include "Common/Game.h"
#include "Common/SoundSystem.h"
#include "File/FileBase.h"
#include "File/FileManager.h"
#include "File/FileID.h"
#include "Object/CheckPoint/CheckPointManager.h"
#include "Object/Car/CarManager.h"
#include "Object/Car/CarBase.h"

namespace
{
	constexpr int RAP_NUM = 3;
}

RaceManager::RaceManager()
{
}

RaceManager::~RaceManager()
{
}

void RaceManager::Init(const std::weak_ptr<CheckPointManager>& cpMgr, const std::weak_ptr<CarManager>& carMgr, int playerNum)
{
	m_cpMgr = cpMgr;
	m_carMgr = carMgr;
	m_carInfos.resize(playerNum);

	for (int i = 0; i < playerNum; ++i)
	{
		m_carInfos[i].rap = 1;
		m_carInfos[i].stading = i + 1;
		m_carInfos[i].isGoal = false;
	}

	auto& fileMgr = FileManager::GetInstance();
	m_rapUpdateSe = fileMgr.Load(FileID::Snd::SE::RAP_UPDATE);
}

#include "MyEngine/DebugDraw.h"
void RaceManager::Update()
{
	UpdateStanding();
	for (auto& item : m_carInfos)
	{
		item.isRapUpdate = false;
		if (item.isGoal) continue;

		++item.time[item.rap - 1];
	}

#if _DEBUG
	auto& debug = MyEngine::DebugDraw::GetInstance();
	wchar_t str[256];
	for (int i = 0; i < m_carInfos.size(); ++i)
	{
		swprintf_s(str, L"carNo: %d, standing : %d", i, m_carInfos[i].stading);
		debug.DrawString(160, 32, 0xffffff, str);
	}
#endif
}

void RaceManager::UpdateRap(int carNo)
{
	auto& info = m_carInfos[carNo];
	if (info.isGoal) return;

	auto& rap = info.rap;
	++rap;
	info.isRapUpdate = true;
	if (rap > RAP_NUM)
	{
		rap = RAP_NUM;
		info.isGoal = true;
	}

	auto& sndSys = SoundSystem::GetInstance();
	sndSys.PlaySe(m_rapUpdateSe->GetHandle());
}

void RaceManager::AddCar(const std::shared_ptr<CarBase>& car)
{
	m_cars.emplace_back(car);
}

bool RaceManager::IsGoal(int carNo) const
{
	return m_carInfos[carNo].isGoal;
}

bool RaceManager::HasItem(int carNo) const
{
	return m_cars[carNo]->HasItem();
}

bool RaceManager::IsRapUpdate(int carNo) const
{
	return m_carInfos[carNo].isRapUpdate;
}

int RaceManager::GetTime(int carNo) const
{
	int ans = 0;
	for (auto t : m_carInfos[carNo].time)
	{
		ans += t;
	}
	return ans;
}

int RaceManager::GetStanding(int carNo) const
{
	return m_carInfos[carNo].stading;
}

int RaceManager::GetRap(int carNo) const
{
	return m_carInfos[carNo].rap;
}

int RaceManager::Get1StandingCarNo() const
{
	for (int i = 0; i < m_carInfos.size(); ++i)
	{
		if (m_carInfos[i].stading == 1)
		{
			return i;
		}
	}
	
	assert(false);
	return 0;
}

std::shared_ptr<CarBase> RaceManager::GetCarToStanding(int standing) const
{
	
	for (int i = 0; i < m_carInfos.size(); ++i)
	{
		if (m_carInfos[i].stading == standing)
		{
			return m_cars[i];
		}
	}

	assert(false && "�͈͊O�ɃA�N�Z�X���悤�Ƃ��Ă��܂�");
	return m_cars.back();
}

std::shared_ptr<ItemBase> RaceManager::GetItem(int carNo) const
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return m_cars[carNo]->GetItem();
}

Vec3 RaceManager::GetCarPos(int carNo) const
{
	return m_cars[carNo]->GetPos();
}

void RaceManager::CarAllGaol() const
{
	for (auto& info : m_cars)
	{
		return info->AllGoal();
	}
}


void RaceManager::UpdateStanding()
{
	// 
	// �ŏ��Ɋ��ɃS�[�����Ă�����̂̏��ʂ͖�������
	// ���b�v���Ŕ���
	// �`�F�b�N�|�C���g�Ŕ���
	// ���̃`�F�b�N�|�C���g�܂ł̋����Ŕ���
	// 
	

	// ���ɃS�[�����Ă�����̂͏��ʂ��m�肳��Ă���̂Ŗ���
	int startStanding = 1;
	for (auto& data : m_carInfos)
	{
		if (data.isGoal)
		{
			++startStanding;

			// �X�^�[�g���̏��ʂ��Ԃ̍��v���𒴂��Ă�����I��
			if (startStanding > m_carInfos.size())
			{
				return;
			}
		}
	}

	// ���b�v��S�Ă܂킷
	for (int i = 3; i > 0; --i)
	{
		std::vector<int> raps;

		for (int j = 0; j < m_carInfos.size(); ++j)
		{
			auto& data = m_carInfos[j];
			if (data.isGoal) continue;

			// ���b�v���������Ȃ画��Ƃ��Ēǉ�
			if (data.rap == i)
			{
				raps.push_back(j);
			}
		}

		auto count = raps.size();
		// ���̃��b�v�ɎԂ����݂��Ȃ��Ȃ炱�̃��b�v�̔���I��
		if (count == 0) continue;

		// ���̃��b�v�ł̑��݂���̂�1�Ԃ݂̂Ȃ̂Ŋm�肷��
		if (count == 1)
		{
			auto no = raps[0];
			m_carInfos[no].stading = startStanding;
			++startStanding;
		}
		// ���̃��b�v�ő��݂���̂�2�Ԉȏ�Ȃ̂ŁA�`�F�b�N�|�C���g�Ŕ��肷��
		else
		{
			CheckStandingCP(raps, startStanding);
		}
	}
}

void RaceManager::CheckStandingCP(std::vector<int> datas, int& nowStanding)
{
	// HACK: �������@������͂�

	// �Ƃ肠�������ʂ�������
	for (auto& carNo : datas)
	{
		m_carInfos[carNo].stading = nowStanding;
		++nowStanding;
	}

	auto size = datas.size();
	for (int i = 0; i < size; ++i)
	{
		for (int j = i + 1; j < size; ++j)
		{
			int carNoA = datas[i];
			int carNoB = datas[j];
			
			int cpNoA = m_cpMgr.lock()->GetNowCpNo(carNoA);
			int cpNoB = m_cpMgr.lock()->GetNowCpNo(carNoB);

			bool isChange = false;

			auto& standingA = m_carInfos[carNoA].stading;
			auto& standingB = m_carInfos[carNoB].stading;

			// A�̎���Ă�CP���AB�̎���Ă�CP����(���)�̏ꍇ
			if (cpNoA > cpNoB)
			{
				// ���ʂ�A�̕������Ȃ�ΕύX������
				isChange = standingA > standingB;
			}
			// A�̎���Ă�CP���AB�̎���Ă�CP����(�O)�̏ꍇ
			else if (cpNoA < cpNoB)
			{
				// ���ʂ�A�̕������Ȃ�ΕύX������
				isChange = standingA < standingB;
			}
			// CP�̈ʒu�������ꍇ
			else
			{
				// ����CP�܂ł̋������r����
				auto sqLenA = GetNextSqDistance(carNoA);
				auto sqLenB = GetNextSqDistance(carNoB);
				// �߂��ق������ʂ����Ȃ�ΕύX
				if (sqLenA < sqLenB)
				{
					isChange = standingA > standingB;
				}
				else
				{
					isChange = standingA < standingB;
				}
			}

			if (isChange)
			{
				int temp = standingA;
				standingA = standingB;
				standingB = temp;
				
			}
		}
	}
}

float RaceManager::GetNextSqDistance(int carNo) const
{
	auto carPos = m_carMgr.lock()->GetCarPos(carNo);
	auto nextCpPos = m_cpMgr.lock()->GetNextCpPos(carNo);

	return (carPos - nextCpPos).SqLength();
}
