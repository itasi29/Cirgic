#pragma once
#include "SceneBase.h"

class SceneSelect : public SceneBase
{
public:
	SceneSelect();
	~SceneSelect() {}

	// �V�[���ɓ���Ƃ��̏���������
	void Init() override;
	// ���t���[���s���X�V����
	void Update(bool isFade) override;
	// ���t���[���s���`�揈��
	void Draw() const override;
	// �V�[���𔲂���Ƃ��̏���
	void End() override;
};

