#include "SceneSelect.h"
#include <DxLib.h>

SceneSelect::SceneSelect() :
	SceneBase(Kind::CHAR_SELECT)
{
}

void SceneSelect::Init()
{
}

void SceneSelect::Update(bool isFade)
{
}

void SceneSelect::Draw() const
{
#ifdef _DEBUG
	DrawString(8, 8, L"SceneSelect", 0xffffff);
#endif
}

void SceneSelect::End()
{
}
