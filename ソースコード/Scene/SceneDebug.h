#pragma once
#ifdef _DEBUG
#include "SceneBase.h"
class SceneDebug final : public SceneBase
{
public:
	SceneDebug();
	~SceneDebug() {}

	// �V�[���ɓ���Ƃ��̏���������
	void Init() override;
	// ���t���[���s���X�V����
	void Update(bool isFade) override;
	// ���t���[���s���`�揈��
	void Draw() const override;
	// �V�[���𔲂���Ƃ��̏���
	void End() override;

private:
	int m_index;
};
#endif
