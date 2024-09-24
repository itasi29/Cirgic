#include "CCell.h"
#include "ColliderTreeObject.h"

using namespace MyEngine;

CCell::CCell() :
	m_latest(nullptr)
{
}

CCell::~CCell()
{
	if (m_latest)
	{
		ResetLink(m_latest);
	}
}

/// <summary>
/// リンクをリセット
/// </summary>
/// <param name="oft">オブジェクト</param>
void CCell::ResetLink(ColliderTreeObject* oft)
{
	if (oft->m_next)
	{
		ResetLink(oft->m_next);
	}
	oft = nullptr;
}

/// <summary>
/// oftをプッシュ
/// </summary>
/// <param name="oft">オブジェクト</param>
/// <returns>false / 失敗, true / 成功</returns>
bool CCell::Push(ColliderTreeObject* oft)
{
	// 無効オブジェクト除外
	if (!oft) return false;
	// 2重登録チェック
	if (oft->m_cell == this) return false;

	if (!m_latest)
	{
		// 新規登録
		m_latest = oft;
	}
	else
	{
		// 更新
		oft->m_next = m_latest;
		m_latest->m_pre = oft;
		m_latest = oft;
	}
	// 空間登録
	oft->Regist(this);
	return true;
}

/// <summary>
/// 削除オブジェクトのチェック
/// </summary>
/// <param name="removeObj">削除オブジェ</param>
/// <returns></returns>
bool CCell::OnRemove(ColliderTreeObject* removeObj)
{
	if (m_latest == removeObj)
	{
		if (m_latest)
		{
			m_latest = m_latest->m_next;
		}
	}
	return true;
}
