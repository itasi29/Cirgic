#include <DxLib.h>
#include <cassert>
#include "FontSystem.h"

namespace
{
	const wchar_t* const kFontPath = L"Font/FGMediumHankaku.ttf";
	const wchar_t* const kFontName = L"FGミディアム半角";
}

FontSystem::FontSystem() :
	m_fontPath()
{}

FontSystem::~FontSystem()
{
	// 作ったフォントデータを全て削除
	for (auto& handle : m_handle)
	{
		DeleteFontToHandle(handle.second);
	}

	if (RemoveFontResourceEx(m_fontPath, FR_PRIVATE, NULL)) {
	}
	else {
		MessageBox(NULL, L"remove failure", L"", MB_OK);
	}
}

FontSystem& FontSystem::GetInstance()
{
	static FontSystem instance;
	return instance;
}

void FontSystem::Init()
{
	// 読み込むフォントファイルのパス
	m_fontPath = kFontPath;
	if (AddFontResourceEx(m_fontPath, FR_PRIVATE, NULL) > 0) 
	{
	}
	else 
	{
		// フォント読込エラー処理
		MessageBox(NULL, L"フォント読込失敗", L"", MB_OK);
	}

	// 使いそうなサイズを作っておく
	m_handle[16] = CreateFontToHandle(kFontName, 16, -1);
	m_handle[24] = CreateFontToHandle(kFontName, 24, -1);
	m_handle[28] = CreateFontToHandle(kFontName, 28, -1);
	m_handle[32] = CreateFontToHandle(kFontName, 32, -1);
	m_handle[48] = CreateFontToHandle(kFontName, 48, -1);
	m_handle[64] = CreateFontToHandle(kFontName, 64, -1);
	m_handle[72] = CreateFontToHandle(kFontName, 72, -1);
}

int FontSystem::GetHandle(int size) const
{
	auto it = m_handle.find(size);
	if (it == m_handle.end())
	{
		assert(false);
		return m_handle.at(16);
	}

	return it->second;
}
