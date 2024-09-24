#pragma once
#include <memory>

namespace MyEngine
{
	class ColliderTreeObject;

	/// <summary>
	/// 空間クラス
	/// </summary>
	class CCell
	{
	public:
		CCell();
		~CCell();

		// リセット
		void ResetLink(ColliderTreeObject* oft);
		// プッシュ
		bool Push(ColliderTreeObject* oft);
		// 削除
		bool OnRemove(ColliderTreeObject* removeObj);

		ColliderTreeObject* GetFirstObj() { return m_latest; }

	protected:
		ColliderTreeObject* m_latest;
	};
}
