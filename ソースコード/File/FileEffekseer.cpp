#include "FileEffekseer.h"
#include <EffekseerForDXLib.h>

FileEffekseer::FileEffekseer(FileManager& mgr) :
	FileBase(mgr)
{
}

FileEffekseer::~FileEffekseer()
{
}

void FileEffekseer::Delete()
{
	DeleteEffekseerEffect(m_handle);
}
