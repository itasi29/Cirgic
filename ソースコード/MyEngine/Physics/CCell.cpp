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
/// �����N�����Z�b�g
/// </summary>
/// <param name="oft">�I�u�W�F�N�g</param>
void CCell::ResetLink(ColliderTreeObject* oft)
{
	if (oft->m_next)
	{
		ResetLink(oft->m_next);
	}
	oft = nullptr;
}

/// <summary>
/// oft���v�b�V��
/// </summary>
/// <param name="oft">�I�u�W�F�N�g</param>
/// <returns>false / ���s, true / ����</returns>
bool CCell::Push(ColliderTreeObject* oft)
{
	// �����I�u�W�F�N�g���O
	if (!oft) return false;
	// 2�d�o�^�`�F�b�N
	if (oft->m_cell == this) return false;

	if (!m_latest)
	{
		// �V�K�o�^
		m_latest = oft;
	}
	else
	{
		// �X�V
		oft->m_next = m_latest;
		m_latest->m_pre = oft;
		m_latest = oft;
	}
	// ��ԓo�^
	oft->Regist(this);
	return true;
}

/// <summary>
/// �폜�I�u�W�F�N�g�̃`�F�b�N
/// </summary>
/// <param name="removeObj">�폜�I�u�W�F</param>
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
