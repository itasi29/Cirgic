#pragma once
#include "FileBase.h"


class FileImage :  public FileBase
{
public:
	FileImage(FileManager& mgr);
	virtual ~FileImage();

private:
	void Delete() override;
};

