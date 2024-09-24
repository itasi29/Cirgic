#include "CLiner8TreeManager.h"
#include <cassert>
#include "CCell.h"
#include "ColliderTreeObject.h"
#include "Collidable.h"
#include "Object/Item/ItemBase.h"

using namespace MyEngine;

CLiner8TreeManager::CLiner8TreeManager() :
	m_uiDim(0),
	m_cellAry(nullptr),
	m_pow{},
	m_width{ 1, 1, 1 },
	m_rangeMin{},
	m_rangeMax{},
	m_dwCellNum(0),
	m_uiLevel(0)
{
}

CLiner8TreeManager::~CLiner8TreeManager()
{
	for (DWORD i = 0; i < m_dwCellNum; ++i)
	{
		if (m_cellAry[i])
		{
			delete m_cellAry[i];
		}
	}
	delete[] m_cellAry;
}

// ���`8����z����\�z
bool CLiner8TreeManager::Init(unsigned int level, const Vec3& min, const Vec3& max)
{
	// �ݒ�ō����x���ȏ�͍��Ȃ�
	if (level >= CLINER_8_TREE_MANAGER_MAXLEVEL)
	{
		assert(false);
		return false;
	}

	// �e���x���ł̋�Ԑ����Z�o
	m_pow[0] = 1;
	for (int i = 1; i < CLINER_8_TREE_MANAGER_MAXLEVEL + 1; ++i)
	{
		m_pow[i] = m_pow[i - 1] * 8;
	}

	// ���x��(0��_)�̔z����쐬
	m_dwCellNum = (m_pow[level + 1] - 1) / 7;
	m_cellAry = new CCell * [m_dwCellNum];
	ZeroMemory(m_cellAry, sizeof(CCell*) * m_dwCellNum);

	RegistRegion(min, max);

	m_uiLevel = level;

	return true;
}

void MyEngine::CLiner8TreeManager::RegistRegion(const Vec3& min, const Vec3& max)
{
	// �̈��o�^
	m_rangeMin = min;
	m_rangeMax = max;
	m_width = max - min;
	m_unit = m_width / (static_cast<float>(1 << m_uiLevel));
}

// �I�u�W�F�N�g�̓o�^
bool CLiner8TreeManager::Regist(Vec3* min, Vec3* max, ColliderTreeObject* oft)
{
	// �I�u�W�F�N�g�̋���͈͂���o�^���[�g���ԍ����Z�o
	DWORD elem = GetMortonNumber(min, max);
	if (elem < m_dwCellNum)
	{
		// ��Ԃ��Ȃ��ꍇ�͐V�K�쐬
		if (!m_cellAry[elem])
		{
			CreateNewCell(elem);
		}
		return m_cellAry[elem]->Push(oft);
	}
	return false;
}

/// <summary>
/// �Փ˔��胊�X�g�̍쐬
/// </summary>
/// <param name="colVec"></param>
/// <returns></returns>
DWORD CLiner8TreeManager::GetAllCollisionList(std::vector<Collidable*>& colVec)
{
	colVec.clear();

	// ���[�g��Ԃ̑��݂��`�F�b�N
	if (!m_cellAry[0]) return 0;

	// ���[�g��Ԃ̏���
	std::list<Collidable*> colStac;
	GetCollisionList(0, colVec, colStac);

	return static_cast<DWORD>(colVec.size());
}

/// <summary>
/// ��ԓ��ŏՓ˃��X�g�̍쐬
/// </summary>
/// <param name="elem"></param>
/// <param name="colVec"></param>
/// <param name="colStac"></param>
/// <returns></returns>
bool CLiner8TreeManager::GetCollisionList(DWORD elem, std::vector<Collidable*>& colVec, std::list<Collidable*> colStac)
{
	// ��ԓ��̃I�u�W�F�N�g���m�̏Փ˃��X�g�쐬
	ColliderTreeObject* oft1 = m_cellAry[elem]->GetFirstObj();
	while (oft1)
	{
		ColliderTreeObject* oft2 = oft1->m_next;
		auto p1 = oft1->m_object;
		while (oft2)
		{
			if (IsAddTree(p1, oft2->m_object))
			{
				// �Փ˃��X�g�쐬
				colVec.push_back(oft1->m_object);
				colVec.push_back(oft2->m_object);
			}
			oft2 = oft2->m_next;
		}
		// �Փ˃X�^�b�N�Ƃ̏Փ˃��X�g�쐬
		for (auto& it : colStac)
		{
			if (IsAddTree(p1, it))
			{
				colVec.push_back(oft1->m_object);
				colVec.push_back(it);
			}
		}
		oft1 = oft1->m_next;
	}

	bool isChild = false;
	// �q��ԂɈړ�
	DWORD objNum = 0;
	DWORD nextElem;
	for (DWORD i = 0; i < 8; ++i)
	{
		nextElem = elem * 8 + 1 + i;
		if (nextElem < m_dwCellNum && m_cellAry[nextElem])
		{
			if (!isChild)
			{
				// �o�^�I�u�W�F�N�g���X�^�b�N�ɒǉ�
				oft1 = m_cellAry[elem]->GetFirstObj();
				while (oft1)
				{
					colStac.push_back(oft1->m_object);
					++objNum;
					oft1 = oft1->m_next;
				}
			}
			isChild = true;
			// �q��Ԃֈړ�
			GetCollisionList(nextElem, colVec, colStac);
		}
	}

	// �X�^�b�N����I�u�W�F�N�g���O��
	if (isChild)
	{
		for (DWORD i = 0; i < objNum; ++i)
		{
			colStac.pop_back();
		}
	}

	return true;
}

bool MyEngine::CLiner8TreeManager::CreateNewCell(DWORD elem)
{
	while (!m_cellAry[elem])
	{
		m_cellAry[elem] = new CCell;

		elem = (elem - 1) >> 3;
		if (elem >= m_dwCellNum) break;
	}
	return true;
}

DWORD MyEngine::CLiner8TreeManager::GetMortonNumber(Vec3* min, Vec3* max)
{
	DWORD lt = GetPointElem(*min);
	DWORD rb = GetPointElem(*max);

	DWORD def = rb ^ lt;
	unsigned int hiLevel = 1;
	for (unsigned int i = 0; i < m_uiLevel; ++i)
	{
		DWORD check = (def >> (i * 3) & 0x7);
		if (check)
		{
			hiLevel = i + 1;
		}
	}
	DWORD spaceNum = rb >> (hiLevel * 3);
	DWORD addNum = (m_pow[m_uiLevel - hiLevel] - 1) / 7;
	spaceNum += addNum;

	if (spaceNum > m_dwCellNum)
	{
		return 0xffffffff;
	}

	return spaceNum;
}

DWORD CLiner8TreeManager::BitSeparateFor3D(BYTE n)
{
	DWORD s = n;
	s = (s | s << 8) & 0x0000f00f;
	s = (s | s << 4) & 0x000c30c3;
	s = (s | s << 2) & 0x00249249;
	return s;
}

DWORD CLiner8TreeManager::Get3DMortonNumber(BYTE x, BYTE y, BYTE z)
{
	return BitSeparateFor3D(x) | BitSeparateFor3D(y) << 1 | BitSeparateFor3D(z) << 2;
}

DWORD CLiner8TreeManager::GetPointElem(Vec3& p)
{
	auto v = (p - m_rangeMin);
	BYTE x = static_cast<BYTE>(v.x / m_unit.x);
	BYTE y = static_cast<BYTE>(v.y / m_unit.y);
	BYTE z = static_cast<BYTE>(v.z / m_unit.z);
	return Get3DMortonNumber(x, y, z);
}

#include "Object/Item/ItemBase.h"
bool MyEngine::CLiner8TreeManager::IsAddTree(Collidable* objA, Collidable* objB)
{
	auto priorityA = objA->GetPriority();
	auto priorityB = objB->GetPriority();

//	return priorityA == Collidable::Priority::HIGH || priorityB == Collidable::Priority::HIGH;
	if (priorityA == Collidable::Priority::HIGH || priorityB == Collidable::Priority::HIGH)
	{
		return true;
	}
	bool isTagItem = objA->GetTag() == ObjectTag::ITEM || objB->GetTag() == ObjectTag::ITEM;
	bool isTagWall = objA->GetTag() == ObjectTag::SYSTEM_WALL || objB->GetTag() == ObjectTag::SYSTEM_WALL;
	bool isTagCar = objA->GetTag() == ObjectTag::CAR || objB->GetTag() == ObjectTag::CAR;
	if (isTagItem && (isTagWall || isTagCar))
	{
		ItemBase* item;
		
		if (objA->GetTag() == ObjectTag::ITEM)
		{
			item = dynamic_cast<ItemBase*>(objA);
		}
		else
		{
			item = dynamic_cast<ItemBase*>(objB);
		}

		if (item->GetKind() == ItemBase::Kind::CARAPACE_GREEN)
		{
			return true;
		}
	}

	return false;

}
