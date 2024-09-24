#pragma once
#include <string>
#include <vector>

class StringUtility
{
public:
	/// <summary>
	/// マルチバイト文字列からワイド文字列への変換
	/// </summary>
	/// <param name="str">変換元のマルチバイト文字列</param>
	/// <returns>変換後のワイド文字列</returns>
	static std::wstring  StringToWString(const std::string& str);

	/// <summary>
	/// 分割関数
	/// </summary>
	/// <param name="str">元の文字列</param>
	/// <param name="del">分割する文字</param>
	/// <returns>分割した文字群</returns>
	static std::vector<std::string> Split(std::string& str, char del);
	/// <summary>
	/// 分割関数
	/// </summary>
	/// <param name="str">元の文字列</param>
	/// <param name="del">分割する文字</param>
	/// <returns>分割した文字群</returns>
	static std::vector<std::wstring> Split(std::wstring& str, char del);
};

