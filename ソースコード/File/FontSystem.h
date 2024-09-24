#pragma once
#include <unordered_map>
#include <windows.h>

/// <summary>
/// フォントを扱うクラス
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

	// フォントのハンドル
	// Key:フォントのサイズ　Value:フォントハンドル
	std::unordered_map<int, int> m_handle;
};

