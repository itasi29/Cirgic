#include "FileImage.h"

#include <DxLib.h>
#include "FileManager.h"

FileImage::FileImage(FileManager& mgr) :
	FileBase(mgr)
{
}

FileImage::~FileImage()
{
}

void FileImage::Delete()
{
	DeleteGraph(m_handle);
}
