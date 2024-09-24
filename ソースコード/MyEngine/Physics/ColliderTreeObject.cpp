#include "ColliderTreeObject.h"
#include "CCell.h"

using namespace MyEngine;

ColliderTreeObject::ColliderTreeObject() :
	m_cell(nullptr),
	m_object(nullptr),
	m_pre(nullptr),
	m_next(nullptr)
{
}

ColliderTreeObject::~ColliderTreeObject()
{
}

bool ColliderTreeObject::Remove()
{
	if (!m_cell) return false;
	if (!m_cell->OnRemove(this)) return false;

	if (m_pre)
	{
		m_pre->m_next = m_next;
	}
	if (m_next)
	{
		m_next->m_pre = m_pre;
	}
	m_cell = nullptr;
	return true;
}

void ColliderTreeObject::Regist(CCell* cell)
{
	m_cell = cell;
}
