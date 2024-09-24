#include "AFile.h"
#include <DxLib.h>
#include <cassert>

AFile::AFile(const wchar_t* path) :
    m_cursor(0)
{
    int size = static_cast<int>(FileRead_size(path));
    m_handle = FileRead_open(path);
    m_data.resize(size);
    FileRead_read(m_data.data(), size, m_handle);
}

bool AFile::IsReadble() const
{
    auto result = CheckHandleASyncLoad(m_handle);
    assert(result != -1);
    return result;
}

bool AFile::Read(void* address, int size)
{
    if (!IsReadble()) return false;

    std::copy_n(&m_data[m_cursor], size, static_cast<uint8_t*>(address));
    m_cursor += size;

    return true;
}

void AFile::Close()
{
    FileRead_close(m_handle);
    m_data.clear();
}
