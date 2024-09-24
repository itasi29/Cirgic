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

		// リストからの除外
		bool Remove();
		// 空間を登録
		void Regist(CCell* cell);
		ColliderTreeObject* GetNextObj() { return m_next; }

	public:
		// 登録空間
		CCell* m_cell;
		// 判定対象オブジェ
		Collidable* m_object;
		// 前の構造体
		ColliderTreeObject* m_pre;
		// 後ろの構造体
		ColliderTreeObject* m_next;
	};
}
