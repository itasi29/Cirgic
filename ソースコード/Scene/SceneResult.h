#pragma once
#include "SceneBase.h"
#include <vector>
#include <list>
#include "Race/RaceManager.h"
#include "Geometry/Vec3.h"
#include "Geometry/Quaternion.h"

class SceneResult final : public SceneBase
{
private:
	struct StandingFrame
	{
		int index = 0;
		int totalTime = 0;
		int rapTime = 0;
		int deltTotalTime = 0;
		int deltRapTime = 0;

		int x = 0;
		int y = 0;
		int fileIndex = 0;
		float rate = 0.0f;
		bool isMove = false;
	};
	struct SelectFrame
	{
		int lX = 0;
		int rX = 0;
		int y = 0;

		int nextY = 0;
		int preY = 0;

		float rate = 0.0f;
		bool isMove = 0.0f;
	};
	struct PodiumCarData
	{
		int handle = -1;
		float rate = 0.0f;
		bool isRateUp = false;
		bool isDelay = false;
		int moveDelayFrame = 0;
		Vec3 pos;
		Vec3 axis;
		Quaternion rot;
		Vec3 startPos;
		Vec3 upPos;
	};
	struct CreatePodiumCarData
	{
		int carNo;
		int standing;
	};
public:
	SceneResult();
	~SceneResult() {}

	// シーンに入るときの初期化処理
	void Init() override;
	// 毎フレーム行う更新処理
	void Update(bool isFade) override;
	// 毎フレーム行う描画処理
	void Draw() const override;
	// シーンを抜けるときの処理
	void End() override;

	void Init(int playerNum, std::vector<RaceData> data);

private:
	void UpdateStanding();
	void UpdateReturn();

	void DrawTime(int time, int x, int y, const wchar_t* const baseStr) const;

	void CreateStandingFrame();
	void CreateStandingFrame(int index, int fileIndex, int y);
	void UpdateStandingFrame();

	bool AddTime(int& deltTime, int time);

	void StartSelectFrameMove();

	void LoadAndCreateObjects(const wchar_t* path, int fileIndex);
	void LoadAndCreateObject(int fileIndex, const Vec3& pos, const Quaternion& rot, const Vec3& scale);
	void LoadAndCreateCar(int carNo, int standing);

private:
	using UpdateFunc_t = void(SceneResult::*)();
	UpdateFunc_t m_updateFunc;

	std::list<int> m_entityModelHnaldeList;
	int m_lightH;
	std::list<CreatePodiumCarData> m_createPodiumCarList;
	std::list<PodiumCarData> m_podiumCarList;

	int m_playerNum;
	int m_createNum;
	int m_createCount;
	bool m_isAllCreate;
	bool m_isAllMoved;
	bool m_isRankUpdate;
	std::vector<StandingFrame> m_standingFrames;
	int m_index;
	float m_shakeFrame;
	SelectFrame m_selectFrame;
	std::vector<RaceData> m_data;
};
