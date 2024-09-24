#include "FileBase.h"

#include "FileManager.h"

FileBase::FileBase(FileManager& mgr) :
	m_mgr(mgr),
	m_handle(-1),
	m_refCount(0),
	m_isEternal(false),
	m_isEnd(false)
{
}

FileBase::~FileBase()
{
	if (m_isEnd) return;

	m_mgr.Delete(m_name.c_str());
}
