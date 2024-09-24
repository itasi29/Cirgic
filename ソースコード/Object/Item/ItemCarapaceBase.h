#pragma once
#include "ItemBase.h"
#include <memory>

class FileBase;

class ItemCarapaceBase abstract : public ItemBase
{
public:
	ItemCarapaceBase(Kind kind, ItemManager& itemMgr, const wchar_t* const path);
	virtual ~ItemCarapaceBase();

	virtual void Update() override;
	void Draw() const override;

	void Use(CarBase* car) override;
	void Hit(CarBase* car) override;
	virtual void Release(CarBase* car, float power) override;

	virtual void OnCollideEnter(Collidable* colider) override;

private:
	std::shared_ptr<FileBase> m_modelFile;
};

