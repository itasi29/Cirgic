#pragma once
#include "SceneBase.h"

class SceneOption : public SceneBase
{
private:
	struct SelectFrame
	{
		int rightX = 0;
		int leftX = 0;
		int y = 0;
		float cycleCount = 0.0f;
		int preY = 0;
		int nextY = 0;
		int preRightX = 0;
		int nextRightX = 0;
		int preLeftX = 0;
		int nextLeftX = 0;
		float rate = 0.0f;
		bool isMove = false;
	};

public:
	SceneOption();
	~SceneOption() {}

	// �V�[���ɓ���Ƃ��̏���������
	void Init() override;
	// ���t���[���s���X�V����
	void Update(bool isFade) override;
	// ���t���[���s���`�揈��
	void Draw() const override;
	// �V�[���𔲂���Ƃ��̏���
	void End() override;

	void SetReturnTitle(bool isReturn) { m_isReturnTitle = isReturn; }

private:
	void UpdateInput();
	void UpdateFrame();

	void UpdateInputNormal();
	void UpdateInputReturn();

	void DrawLevel(int level, int index) const;

	void StartSelectFrameMove(int lX, int rX, int y);

private:
	using UpdateInputFunc_t = void(SceneOption::*)();
	UpdateInputFunc_t m_updateInputFunc;

	bool m_isNowOpen;
	bool m_isReturnTitle;
	bool m_isOpenReturnCheck;
	int m_indexNormal;
	int m_indexReturn;
	SelectFrame m_selectFrameInfo;
};

