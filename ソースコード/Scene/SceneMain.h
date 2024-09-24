#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include <array>

class ObjectManager;
class RaceManager;

class SceneMain final : public SceneBase
{
private:
	enum class ScreenType
	{
		PLAYER_1,
		PLAYER_2,
		PLAYER_3,
		PLAYER_4,
	};
	struct ItemUIData
	{
		bool isDraw = false;
		bool isBlink = true;
		int count = 0;
	};
	struct RapTime
	{
		int rapTime = 0;
		int frame = 0;
		bool isDraw = false;
		bool isUpdate = false;
	};

public:
	SceneMain();
	~SceneMain() {}

	// シーンに入るときの初期化処理
	void Init() override;
	// 毎フレーム行う更新処理
	void Update(bool isFade) override;
	// 毎フレーム行う描画処理
	void Draw() const override;
	// シーンを抜けるときの処理
	void End() override;

	void InitPlayerNum(int num);

private:
	void CountDownUpdate();
	void NormalUpdate();
	void FinishUpdate();

	void CountDownDraw() const;
	void NormalDraw() const;
	void FinishDraw() const;

	void ScreenDraw() const;

	void LoadFiles();
	void CreateScreen();

	void CountDownDrawUI(bool isCurtainAlpha) const;
	void CarDrawUI() const;

	void DrawCarItemUI(int carNo, int x, int y, double rate) const;
	void DrawMiniMap(int carNo, int centerX, int centerY) const;
	void DrawRapTimeUpdate(int carNo, int x, int y) const;

private:
	int m_playerNum;
	ScreenType m_screenType;

	std::shared_ptr<RaceManager> m_raceMgr;
	std::shared_ptr<ObjectManager> m_objMgr;

	std::vector<int> m_screenH;

	using UpdateFunc_t = void(SceneMain::*)();
	using DrawFunc_t = void(SceneMain::*)() const;
	UpdateFunc_t m_updateFunc;
	DrawFunc_t m_drawFunc;

	int m_bgmVol;
	float m_bgmVolRate;

	bool m_isPlaySeCountDown;
	int m_countDown;
	int m_countIndex;
	bool m_isDrawStartCurtain;
	int m_finishCount;

	int m_carNo;
	int m_changeFrame;

	int m_time;
	std::array<ItemUIData, 4> m_itemUIDatas;

	std::array<RapTime, 3> m_rapTimeDatas;
};

