#pragma once
#include "CarBase.h"

class CheckPointManager;

class CarPlayer : public CarBase
{
public:
	CarPlayer(ItemManager& itemMgr, CheckPointManager& cpMgr, const Vec3& pos);

	void InitPadNo(int no);

	void Update() override;
	void Draw() const override;

	void OnDash(float dash) override;

private:
	int m_padNo;	// é©êgÇÃPadî‘çÜ
};

