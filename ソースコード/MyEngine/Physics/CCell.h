#pragma once
#include <memory>

namespace MyEngine
{
	class ColliderTreeObject;

	/// <summary>
	/// ��ԃN���X
	/// </summary>
	class CCell
	{
	public:
		CCell();
		~CCell();

		// ���Z�b�g
		void ResetLink(ColliderTreeObject* oft);
		// �v�b�V��
		bool Push(ColliderTreeObject* oft);
		// �폜
		bool OnRemove(ColliderTreeObject* removeObj);

		ColliderTreeObject* GetFirstObj() { return m_latest; }

	protected:
		ColliderTreeObject* m_latest;
	};
}
