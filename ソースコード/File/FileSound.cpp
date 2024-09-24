#include "FileSound.h"

#include <DxLib.h>

FileSound::FileSound(FileManager& mgr) :
	FileBase(mgr)
{
}

FileSound::~FileSound()
{
}

void FileSound::Delete()
{
	DeleteSoundMem(m_handle);
}
