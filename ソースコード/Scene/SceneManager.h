#pragma once
#include <memory>

class FileBase;
class SceneBase;
class Input;

/// <summary>
/// �V�[���Ǘ��N���X
/// </summary>
class SceneManager
{
private:
	SceneManager();

	SceneManager(const SceneManager& mgr) = delete;
	void operator= (const SceneManager& mgr) = delete;

public:
	virtual ~SceneManager();

	static SceneManager& GetInstance();

	void Init();
	void Update();
	void Draw() const;
	void End();

	/// <summary>
	/// �V�[���̕ύX
	/// </summary>
	/// <param name="isFade">true: �t�F�[�h���s��(default) / false: �t�F�[�h���s�킢</param>
	void Change(const std::shared_ptr<SceneBase>& next, bool isFade = true);

	/// <summary>
	/// ����Bgm��ύX����
	/// </summary>
	/// <param name="handle">����Bgm�̃n���h��</param>
	void ChangeBgmH(int handle);

	void OptionOpen();
	void OptionClose();

	void ChangeSkyDomeTexIndex();

	bool IsOpenOption() const { return m_isOpenOption; }
	int GetSkyDomeTexIndex() const;

private:
	using UpdateFunc_t = void(SceneManager::*)();
	void NormalUpdate();
	void FileLoadingUpdate();

	using DrawFunc_t = void(SceneManager::*)() const;
	void NormalDraw() const;
	void FileLoadingDraw() const;

	// �t�F�[�h�A�E�g�Ɏg������
	using FadeFunc_t = void(SceneManager::*)();
	void FadeNone();
	void FadeIn();
	void FadeOut();

	void CheckFileLoadingStart();

private:
	std::shared_ptr<SceneBase> m_optionScnene;
	std::shared_ptr<SceneBase> m_scene;
	std::shared_ptr<SceneBase> m_nextScene;

	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	bool m_isOpenOption;

	// �����Ă���BGM�̃n���h��
	int m_bgmH;

	// �t�F�[�h�A�E�g�Ɏg������
	FadeFunc_t m_fadeFunc;
	float m_fadeRate;
	bool m_isFade;

	std::shared_ptr<FileBase> m_skyDomeTexIndex;
};

