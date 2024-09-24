#ifdef _DEBUG
#include "SceneDebug.h"
#include <string>
#include <DxLib.h>
#include "SceneManager.h"
#include "SceneResult.h"
#include "Common/Input.h"
#include "Common/CommandList.h"

namespace
{
	std::wstring NAME[] =
	{
		L"Title",
		L"Main",
		L"Result",
		L"Select",
		L"Debug",
	};
}

SceneDebug::SceneDebug() :
	SceneBase(Kind::DEBUG),
	m_index(0)
{
	m_nextSceneKind = Kind::TITLE;
}

void SceneDebug::Init()
{
}

void SceneDebug::Update(bool isFade)
{
	auto& input = Input::GetInstance();
	int max = static_cast<int>(Kind::MAX);

	if (input.IsTriggerd(KEY_INPUT_UP) || input.IsTriggerd(0, Command::BTN_UP))
	{
		m_nextSceneKind = static_cast<Kind>((static_cast<int>(m_nextSceneKind) - 1 + max) % max);
	}
	if (input.IsTriggerd(KEY_INPUT_DOWN) || input.IsTriggerd(0, Command::BTN_DOWN))
	{
		m_nextSceneKind = static_cast<Kind>((static_cast<int>(m_nextSceneKind) + 1) % max);
	}
	if (input.IsTriggerd(KEY_INPUT_RETURN) || input.IsTriggerd(0, Command::BTN_OK))
	{
		auto next = Change();
		if (m_nextSceneKind == Kind::RESULT)
		{
			auto t = std::dynamic_pointer_cast<SceneResult>(next);
			RaceData d;
			d.isGoal = true;
			d.rap = 4;
			d.stading = 1;
			d.time[0] = 4000;
			d.time[1] = 5000;
			d.time[2] = 6000;
			std::vector<RaceData> data;
			data.push_back(d);
			t->Init(1, data);
		}
		m_scnMgr.Change(next);
	}
}

void SceneDebug::Draw() const
{
	DrawString(8, 8, L"SceneDebug", 0xffffff);

	int y = 32;
	for (int i = 0; i < static_cast<int>(Kind::MAX); ++i)
	{
		if (i == static_cast<int>(m_nextSceneKind))
		{
			DrawBox(16, y - 4, 96, y + 20, 0xff0000, true);
		}

		DrawFormatString(16, y, 0xffffff, NAME[i].c_str());
		if (i >= static_cast<int>(Kind::CHAR_SELECT))
		{
			DrawFormatString(128, y, 0xffffff, L"Žg‚¦‚Ü‚¹‚ñ");
		}
		

		y += 24;
	}
}

void SceneDebug::End()
{
}
#endif