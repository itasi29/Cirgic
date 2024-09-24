#include "ItemCarapaceGreen.h"
#include <string>
#include "File/FileID.h"
#include "MyEngine/DebugDraw.h"

namespace
{
	// �o�E���h��
	constexpr int COUNT_BOUND = 3;
}

ItemCarapaceGreen::ItemCarapaceGreen(ItemManager& itemMgr) :
	ItemCarapaceBase(Kind::CARAPACE_GREEN, itemMgr, FileID::Eff::CARAPCE_GREEN),
	m_boundCount(0)
{
}

ItemCarapaceGreen::~ItemCarapaceGreen()
{
}

void ItemCarapaceGreen::OnCollideEnter(Collidable* colider)
{
	ItemCarapaceBase::OnCollideEnter(colider);
#ifdef _DEBUG
	std::wstring message = L"Collide: �΍b����";
#endif

	auto kind = colider->GetTag();
	if (kind == ObjectTag::SYSTEM_WALL)
	{
#ifdef _DEBUG
		message += L"�V�X�e����";
#endif

		++m_boundCount;
		if (m_boundCount > COUNT_BOUND)
		{
			End();
		}
	}

#ifdef _DEBUG
	message += L"�Ɠ�������\n";
	auto& debug = MyEngine::DebugDraw::GetInstance();
	debug.DrawString(900, 16, 0xffffff, message);
#endif
}
