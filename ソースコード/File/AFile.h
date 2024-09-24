#pragma once
#include <vector>

class AFile
{
public:
	AFile(const wchar_t* path);

	bool IsReadble() const;
	bool Read(void* address, int size);
	void Close();

private:
	int m_handle;
	int m_cursor;
	std::vector<uint8_t> m_data;
};
