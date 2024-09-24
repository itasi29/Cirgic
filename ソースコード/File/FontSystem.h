#pragma once
#include <unordered_map>
#include <windows.h>

/// <summary>
/// �t�H���g�������N���X
/// </summary>
class FontSystem
{
private:
	FontSystem();

	FontSystem(const FontSystem& font) = delete;
	void operator= (const FontSystem& font) = delete;

public:
	~FontSystem();

	static FontSystem& GetInstance();

	void Init();

	int GetHandle(int size) const;

private:
	LPCWSTR m_fontPath;

	// �t�H���g�̃n���h��
	// Key:�t�H���g�̃T�C�Y�@Value:�t�H���g�n���h��
	std::unordered_map<int, int> m_handle;
};

