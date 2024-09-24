#include "ColliderBase.h"

MyEngine::ColliderBase::ColliderBase(Kind kind) :
	isTrigger(false),
	center(Vec3()),
	m_kind(kind)
{
}
