#pragma once
#include "FileBase.h"

class FileModel : public FileBase
{
public:
	FileModel(FileManager& mgr);
	virtual ~FileModel();

private:
	void Delete() override;
};

