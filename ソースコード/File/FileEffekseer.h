#pragma once
#include "FileBase.h"

class FileEffekseer : public FileBase
{
public:
	FileEffekseer(FileManager& mgr);
	virtual ~FileEffekseer();

private:
	void Delete() override;
};

