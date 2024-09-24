#pragma once
#include "Object/Object3D.h"

namespace MyEngine
{
	class ColliderCapsule;
}

class JumpPoint : public Object3D
{
public:
	JumpPoint();

private:
	MyEngine::ColliderCapsule* m_capsule;
};

