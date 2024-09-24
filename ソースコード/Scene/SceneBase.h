#pragma once
#include <vector>
#include <memory>

class SceneManager;
class FileBase;
class Input;

/// <summary>
/// シーンの基底クラス
/// </summary>
class SceneBase
{
public:
	enum class Kind
	{
		TITLE,
		MAIN,
		RESULT,
		CHAR_SELECT,
		OPTION,
		DEBUG,
		MAX
	};

public:
	SceneBase(Kind kind);
	virtual ~SceneBase() {}

	// シーンに入るときの初期化処理
	virtual void Init() = 0;
	// 毎フレーム行う更新処理
	virtual void Update(bool isFade) = 0;
	// 毎フレーム行う描画処理
	virtual void Draw() const = 0;
	// シーンを抜けるときの処理
	virtual void End() = 0;
	// シーン変更処理
	std::shared_ptr<SceneBase> Change();

	Kind GetNowKind() const { return m_nowSceneKind; }

protected:
	SceneManager& m_scnMgr;
	std::vector<std::shared_ptr<FileBase>> m_files;
	Kind m_nextSceneKind;
	
private:
	Kind m_nowSceneKind;
};

