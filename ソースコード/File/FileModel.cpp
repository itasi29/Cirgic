#include "FileModel.h"

#include <DxLib.h>

FileModel::FileModel(FileManager& mgr) :
	FileBase(mgr)
{
}

FileModel::~FileModel()
{
}

void FileModel::Delete()
{
	MV1DeleteModel(m_handle);
}
