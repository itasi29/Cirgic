#pragma once
#include "Object/Object3D.h"
#include <memory>

namespace MyEngine
{
	class ColliderSphere;
}

struct Vec3;
class CarBase;
class ItemManager;

class ItemBase abstract : public Object3D
{
public:
	enum class Kind
	{
		COIN,
		KINOKO,
		KINOKO_TRIPLE,
		NOW_MAX,
		CARAPACE_GREEN = NOW_MAX,
		CARAPACE_RED,
		BANANA,
		MAX
	};

public:
	ItemBase(Kind kind, ItemManager& itemMgr);
	virtual ~ItemBase();

	virtual void Update() override;
	virtual void Draw() const override;
	virtual void End();

	virtual void Use(CarBase* car) abstract;
	virtual void Hit(CarBase* car) abstract;
	virtual void Release(CarBase* car, float power);
	void UpdateEntityPos(const Vec3& pos);

	Kind GetKind() const { return m_kind; }
	bool IsDestroyEntity() const { return m_isDestoyEntity; }
	bool IsDraw() const { return m_isDraw; }

	virtual void OnCollideEnter(Collidable* colider) override;

protected:
	void OnEntity(float radius);

protected:
	std::shared_ptr<MyEngine::ColliderSphere> m_sphere;
	ItemManager& m_itemMgr;


private:
	Kind m_kind;

	int m_existCount;
	bool m_isCountExist;

	bool m_isEntryPhysics;
	bool m_isDestoyEntity;

	bool m_isDraw;
};
