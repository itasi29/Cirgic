#include "ItemManager.h"
#include <cassert>
#include <fstream>
#include <sstream>
#include "Items.h"
#include "ItemBox.h"
#include "Utility/StringUtility.h"
#include "Common/Application.h"
#include "Common/SoundSystem.h"
#include "Object/Car/CarBase.h"
#include "Race/RaceManager.h"
#include "File/FileBase.h"
#include "File/FileManager.h"
#include "File/FileID.h"

ItemManager::ItemManager()
{
}

ItemManager::~ItemManager()
{
}

void ItemManager::Init(const std::weak_ptr<RaceManager>& raceMgr, int playerNum)
{
	m_raceMgr = raceMgr;

	if (playerNum == 1)
	{
		m_createFunc = &ItemManager::SimpleCreate;
	}
	else
	{
		m_createFunc = &ItemManager::RandCreate;
	}

	LoadItemGetRateData(L"Data/Master/ItemGetRateMaster.csv");
	LoadAndCreateCoin(L"Data/Stage/Main/Coin.loc");
	LoadAndCreateItemBox(L"Data/Stage/Main/ItemBox.loc");

	auto& fileMgr = FileManager::GetInstance();
	m_getSe = fileMgr.Load(FileID::Snd::SE::ITEM_GET);
}

void ItemManager::End()
{
	for (auto& item : m_entityItems)
	{
		item->End();
	}
	for (auto& box : m_itemBoxs)
	{
		box->End();
	}
	m_entityItems.clear();
	m_itemBoxs.clear();
}

void ItemManager::Update()
{
	// 存在するアイテムの更新
	for (auto& item : m_entityItems)
	{
		item->Update();
	}
	m_entityItems.remove_if(
		[](const auto& item)
		{
			return item->IsDestroyEntity();
		});
	// アイテムボックス更新
	for (auto& box : m_itemBoxs)
	{
		box->Update();
	}
	m_itemBoxs.remove_if(
		[](const auto& box)
		{
			return box->IsEnd();
		});

	// 再生成データの更新
	for (auto& item : m_recreateCoinData)
	{
		++item.frame;
		if (item.frame > 180)
		{
			auto coin = std::make_shared<ItemCoin>(*this);
			coin->OnEntiry(15, item.pos, item.scale);
			item.isDestory = true;
		}
	}
	m_recreateCoinData.remove_if(
		[](const auto& item)
		{
			return item.isDestory;
		});
	for (auto& item : m_recreateBoxData)
	{
		++item.frame;
		if (item.frame > 360)
		{
			auto box = std::make_shared<ItemBox>(*this);
			box->Init(item.pos, item.rot);
			m_itemBoxs.emplace_back(box);

			item.isDestory = true;
		}
	}
	m_recreateBoxData.remove_if(
		[](const auto& item)
		{
			return item.isDestory;
		});
}

void ItemManager::Draw() const
{
	for (auto& item : m_entityItems)
	{
		item->Draw();
	}
	for (auto& item : m_itemBoxs)
	{
		item->Draw();
	}
}

void ItemManager::AddEntity(std::shared_ptr<MyEngine::Collidable> item)
{
	auto cast = std::dynamic_pointer_cast<ItemBase>(item);
	m_entityItems.emplace_back(cast);
}

void ItemManager::RecreateCoin(const Vec3& pos, const Vec3& scale)
{
	RecreateCoinData data;

	data.pos = pos;
	data.scale = scale;

	m_recreateCoinData.emplace_back(data);
}

void ItemManager::RecreateBox(const Vec3& pos, const Quaternion& rot)
{
	RecreateBoxData data;
	
	data.pos = pos;
	data.rot = rot;

	m_recreateBoxData.emplace_back(data);
}

std::shared_ptr<ItemBase> ItemManager::GetItem(int carNo)
{
	auto& sndSys = SoundSystem::GetInstance();
	sndSys.PlaySe(m_getSe->GetHandle());
//	return std::make_shared<ItemCarapaceGreen>(*this);
	return (this->*m_createFunc)(carNo);
}

std::shared_ptr<ItemBase> ItemManager::RandCreate(int carNo)
{
	auto& app = Application::GetInstance();

	std::shared_ptr<ItemBase> res;
	auto rand = app.GetRand(0, 100);

	const auto& mgr = m_raceMgr.lock();
	int standing = mgr->GetStanding(carNo) - 1;
	int checkRate = 0;
	int index = 0;
	for (auto& rate : m_itemRateData[standing].rateData)
	{
		checkRate += rate;
		if (rand <= checkRate) break;
		++index;
	}
	ItemBase::Kind kind = static_cast<ItemBase::Kind>(index);

	if (kind == ItemBase::Kind::COIN)
	{
		res = std::make_shared<ItemCoin>(*this);
	}
	else if (kind == ItemBase::Kind::KINOKO)
	{
		res = std::make_shared<ItemKinoko>(*this);
	}
	else if (kind == ItemBase::Kind::KINOKO_TRIPLE)
	{
		res = std::make_shared<ItemKinokoTriple>(*this);
	}
	else if (kind == ItemBase::Kind::CARAPACE_GREEN)
	{
		res = std::make_shared<ItemCarapaceGreen>(*this);
	}
	else if (kind == ItemBase::Kind::CARAPACE_RED)
	{
		res = std::make_shared<ItemCarapaceRed>(*this, m_raceMgr);
	}
	else if (kind == ItemBase::Kind::BANANA)
	{
		res = std::make_shared<ItemBanana>(*this);
	}
	else
	{
		// 変なものが入ってきたらコインを変えす
		assert(false && "範囲外のアイテムです");
		res = std::make_shared<ItemCoin>(*this);
	}

	return res;
}

std::shared_ptr<ItemBase> ItemManager::SimpleCreate(int carNo)
{
	return std::make_shared<ItemKinokoTriple>(*this);
}

void ItemManager::LoadItemGetRateData(const wchar_t* const path)
{
	// ファイル読み込み
	std::wifstream ifs(path);
	if (!ifs)
	{
		assert(false);
		return;
	}

	std::wstring strBuf;
	std::vector<std::wstring> strConmaBuf;

	// 余分な部分の読み込み
	std::getline(ifs, strBuf);

	// データすべて読み込み
	while (std::getline(ifs, strBuf))
	{
		strConmaBuf = StringUtility::Split(strBuf, ',');

		enum CSVIndex : int 
		{
			LENGHT,
			RATE_DATA
		};


		ItemGetRateData data;
		// 順位取得
		data.standing = std::stoi(strConmaBuf[CSVIndex::LENGHT]);

		// データ数の取得
		int size = static_cast<int>(strConmaBuf.size()) - 1;
		data.rateData.resize(size);

		for (int i = 0; i < size; ++i)
		{
			auto& rate = data.rateData[i];
			rate = std::stoi(strConmaBuf[CSVIndex::RATE_DATA + i]);
		}

		m_itemRateData.push_back(data);
	}
}

void ItemManager::LoadAndCreateCoin(const wchar_t* const path)
{
	SetUseASyncLoadFlag(false);

	int handle = FileRead_open(path);

	if (handle != 0)
	{
		int size;
		FileRead_read(&size, sizeof(int), handle);

		for (int i = 0; i < size; ++i)
		{
			Vec3 pos;
			Quaternion rot;
			Vec3 scale;

			FileRead_read(&pos, sizeof(pos), handle);
			FileRead_read(&rot, sizeof(rot), handle);
			FileRead_read(&scale, sizeof(scale), handle);

			auto coin = std::make_shared<ItemCoin>(*this);
			coin->OnEntiry(15, pos, scale);
		}

		FileRead_close(handle);
	}
	else
	{
		assert(false && "道データのファイルパスが違います");
	}

	SetUseASyncLoadFlag(true);
}

void ItemManager::LoadAndCreateItemBox(const wchar_t* const path)
{
	SetUseASyncLoadFlag(false);

	int handle = FileRead_open(path);

	if (handle != 0)
	{
		int size;
		FileRead_read(&size, sizeof(int), handle);

		for (int i = 0; i < size; ++i)
		{
			Vec3 pos;
			Quaternion rot;

			FileRead_read(&pos, sizeof(pos), handle);
			FileRead_read(&rot, sizeof(rot), handle);

			auto box = std::make_shared<ItemBox>(*this);
			box->Init(pos, rot);
			m_itemBoxs.emplace_back(box);
		}

		FileRead_close(handle);
	}
	else
	{
		assert(false && "道データのファイルパスが違います");
	}

	SetUseASyncLoadFlag(true);
}
