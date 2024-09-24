#pragma once
#include "Object/Object3D.h"

class ItemManager;
struct Vec3;
struct Quaternion;

class ItemBox final : public Object3D
{
public:
	ItemBox(ItemManager& itemMgr);

	void Init(const Vec3& pos, const Quaternion& rot);
	void End();

	void Update() override;
	void Draw() const override;

	bool IsEnd() const { return m_isEnd; }

	void OnTriggerEnter(Collidable* colider) override;

private:
	ItemManager& m_itemMgr;
	std::shared_ptr<FileBase> m_model;

	bool m_isEnd;
};

