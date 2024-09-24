#pragma once
#include <memory>

namespace MyEngine
{
	class Collidable;
	class CCell;

	class ColliderTreeObject
	{
	public:
		ColliderTreeObject();
		~ColliderTreeObject();

		// ���X�g����̏��O
		bool Remove();
		// ��Ԃ�o�^
		void Regist(CCell* cell);
		ColliderTreeObject* GetNextObj() { return m_next; }

	public:
		// �o�^���
		CCell* m_cell;
		// ����ΏۃI�u�W�F
		Collidable* m_object;
		// �O�̍\����
		ColliderTreeObject* m_pre;
		// ���̍\����
		ColliderTreeObject* m_next;
	};
}
