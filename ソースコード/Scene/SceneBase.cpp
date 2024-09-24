#include "SceneBase.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneMain.h"
#include "SceneResult.h"
#include "SceneDebug.h"

SceneBase::SceneBase(Kind kind) :
	m_scnMgr(SceneManager::GetInstance()),
	m_nextSceneKind(Kind::TITLE),
	m_nowSceneKind(kind)
{
}

std::shared_ptr<SceneBase> SceneBase::Change()
{
	std::shared_ptr<SceneBase> res;

	if (m_nextSceneKind == Kind::TITLE)
	{
		res = std::make_shared<SceneTitle>();
	}
	else if (m_nextSceneKind == Kind::MAIN)
	{
		res = std::make_shared<SceneMain>();
	}
	else if (m_nextSceneKind == Kind::RESULT)
	{
		res = std::make_shared<SceneResult>();
	}
	else if (m_nextSceneKind == Kind::CHAR_SELECT)
	{
		res = std::make_shared<SceneSelect>();
	}
#ifdef _DEBUG
	else if (m_nextSceneKind == Kind::DEBUG)
	{
		res = std::make_shared<SceneDebug>();
	}
#endif

	return res;
}
