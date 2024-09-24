#pragma once
#include <vector>
#include <memory>

class SceneManager;
class FileBase;
class Input;

/// <summary>
/// �V�[���̊��N���X
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

	// �V�[���ɓ���Ƃ��̏���������
	virtual void Init() = 0;
	// ���t���[���s���X�V����
	virtual void Update(bool isFade) = 0;
	// ���t���[���s���`�揈��
	virtual void Draw() const = 0;
	// �V�[���𔲂���Ƃ��̏���
	virtual void End() = 0;
	// �V�[���ύX����
	std::shared_ptr<SceneBase> Change();

	Kind GetNowKind() const { return m_nowSceneKind; }

protected:
	SceneManager& m_scnMgr;
	std::vector<std::shared_ptr<FileBase>> m_files;
	Kind m_nextSceneKind;
	
private:
	Kind m_nowSceneKind;
};

