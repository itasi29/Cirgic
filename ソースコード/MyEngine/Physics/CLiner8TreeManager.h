#pragma once
#include <memory>
#include <vector>
#include <list>
#include "Geometry/Vec3.h"

namespace MyEngine
{
	class Collidable;
	class CCell;
	class ColliderTreeObject;

	class CLiner8TreeManager
	{
	public:
		CLiner8TreeManager();
		~CLiner8TreeManager();

		bool Init(unsigned int level, const Vec3& min, const Vec3& max);
		void RegistRegion(const Vec3& min, const Vec3& max);
		bool Regist(Vec3* min, Vec3* max, ColliderTreeObject* psOFT);
		DWORD GetAllCollisionList(std::vector<Collidable*>& colVec);

	protected:
		bool GetCollisionList(DWORD elem, std::vector<Collidable*>& colVec, std::list<Collidable*> colStac);
		bool CreateNewCell(DWORD elem);
		DWORD GetMortonNumber(Vec3* min, Vec3* max);
		DWORD BitSeparateFor3D(BYTE n);
		DWORD Get3DMortonNumber(BYTE x, BYTE y, BYTE z);
		DWORD GetPointElem(Vec3& p);

	private:
		bool IsAddTree(Collidable* objA, Collidable* objB);

	protected:
		static constexpr int CLINER_8_TREE_MANAGER_MAXLEVEL = 7;

	protected:
		unsigned int m_uiDim;
		// ���`���
		CCell** m_cellAry;
		// �ׂ��搔�l�z��
		unsigned int m_pow[CLINER_8_TREE_MANAGER_MAXLEVEL + 1];
		// �̈敝
		Vec3 m_width;
		// �̈�ŏ��l
		Vec3 m_rangeMin;
		// �̈�ő�l
		Vec3 m_rangeMax;
		// �ŏ��̈�̕ӂ̒���
		Vec3 m_unit;
		// ��Ԃ̐�
		DWORD m_dwCellNum;	// DWORD = unsigned long
		// �ŉ��ʃ��x��
		unsigned int m_uiLevel;
	};
}
