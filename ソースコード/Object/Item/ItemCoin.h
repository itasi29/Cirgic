#pragma once
#include "ItemBase.h"
#include <memory>

class FileBase;

class ItemCoin : public ItemBase
{
public:
	ItemCoin(ItemManager& itemMgr);
	~ItemCoin();

	void Update() override;
	void End() override;

	void Use(CarBase* car) override;
	void Hit(CarBase* car) override;

	void Draw() const override;

	void OnEntiry(float radius, const Vec3& pos, const Vec3& scale);

private:
	std::shared_ptr<FileBase> m_se;
	std::shared_ptr<FileBase> m_model;

	bool m_isEntiry;
};

