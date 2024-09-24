#pragma once
#include <memory>
#include <string>
#include <list>
#include <unordered_map>

class FileBase;

enum class LoadType
{
	IMAGE,
	SOUND,
	MODEL,
	EFFEKSEER
};

struct Path
{
	LoadType type = LoadType::IMAGE;
	std::wstring path = L"";
};

// パスデータ
using PathData_t = std::unordered_map<std::wstring, Path>;
// ロード済みデータ
using LoadedData_t = std::unordered_map<std::wstring, std::shared_ptr<FileBase>>;
// モデルにはるデータ群
using TextureData_t = std::unordered_map<std::wstring, std::list<std::wstring>>;

/// <summary>
/// ファイル管理クラス
/// </summary>
class FileManager
{
private:
	FileManager();

	FileManager(const FileManager& mgr) = delete;
	void operator= (const FileManager& mgr) = delete;

public:
	~FileManager();

	static FileManager& GetInstance();

	// 初期化処理
	void Init();
	// 終了処理
	void End();

	/// <summary>
	/// データの読み込み
	/// </summary>
	/// <param name="name">読み込みたいデータ名</param>
	/// <param name="isEternal">true : 永続 / false : 使っているものが消えたら削除</param>
	/// <returns>データ</returns>
	std::shared_ptr<FileBase> Load(const wchar_t* const name, bool isEternal = false);
	/// <summary>
	/// モデルデータにテクスチャデータを貼り付ける
	/// </summary>
	/// <param name="modelH">モデルハンドル</param>
	/// <param name="name">データ名</param>
	void SetTexture(int modelH, const std::wstring& name);
	/// <summary>
	///	データの削除
	/// </summary>
	/// <param name="name">消したいデータ名</param>
	void Delete(const wchar_t* const name);

private:
	// ファイルの生成
	std::shared_ptr<FileBase> MakeFile(const wchar_t* const name);
	// ファイルのコピー
	std::shared_ptr<FileBase> CastCopyFile(std::shared_ptr<FileBase>& data);
	// ハンドルの取得
	int GetHandle(const wchar_t* name) const;

	// パスデータのロード
	void LoadPath();
	// テクスチャ貼り付けデータのロード
	void LoadSetTextureDate();

private:
	bool m_isEnd;

	PathData_t m_path;
	LoadedData_t m_file;
	TextureData_t m_texData;
};

