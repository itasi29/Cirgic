#pragma once
#include "FileBase.h"

class FileSound : public FileBase
{
public:
	FileSound(FileManager& mgr);
	virtual ~FileSound();

private:
	void Delete() override;
};

