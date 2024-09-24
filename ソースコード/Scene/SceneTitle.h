#pragma once
#include "SceneBase.h"
#include <vector>
#include <list>
#include "Geometry/Vec3.h"
#include "Geometry/Matrix4x4.h"

struct Quaternion;

class SceneTitle : public SceneBase
{
private:
	struct Strs
	{
		const wchar_t* str = L"";
		int initX = 0;
		int x = 0;
		int y = 0;
		float rate = 0.0f;
	};
	struct SelectFrame
	{
		int rightX = 0;
		int leftX = 0;
		int y = 0;
		float cycleCount = 0.0f;
		int preY = 0;
		int nextY = 0;
		float rate = 0.0f;
		bool isMove = false;
	};
	struct SelectNumFrame
	{
		int index = 0;
		int x = 0;
		int preX = 0;
		int nextX = 0;
		float rate = 0.0f;
		bool isMove = false;
	};
	struct RankingInfo
	{
		int time = 0;
		int y = 0;
		float rate = 0.0f;
		bool isMove = false;
	};
	struct CarData
	{
		int handle = -1;
		float speed = 0.0f;
		Vec3 pos = Vec3();
		Matrix4x4 baseMat;
	};

public:
	SceneTitle();
	~SceneTitle() {}

	// シーンに入るときの初期化処理
	void Init() override;
	// 毎フレーム行う更新処理
	void Update(bool isFade) override;
	// 毎フレーム行う描画処理
	void Draw() const override;
	// シーンを抜けるときの処理
	void End() override;

private:
	void InitModelsPos();

	void UpdateNormal();
	void UpdateRanking();

	void UpdateInput();
	void UpdateStr();
	void UpdateSelectFrame();
	void UpdateSelectNumFrame();

	void InitSelectNumFrame();
	void StartSelectFrameMove();
	void StartSelectNumFrameMove();

	void DrawPlayNumFrame(int x, int y, int num) const;
	void DrawRanking() const;
	void DrawModel() const;

	void LoadAndCreateObjects(const wchar_t* path, int fileIndex);
	void LoadAndCreateObject(int fileIndex, const Vec3& pos, const Quaternion& rot, const Vec3& scale);
	void LoadAndCreateCar(int no);

private:
	using UpdateFunc_t = void(SceneTitle::*)();
	UpdateFunc_t m_updateFunc;

	int m_index;
	int m_numIndex;
	std::vector<Strs> m_strDrawPos;
	SelectFrame m_selectFrameInfo;
	SelectNumFrame m_selectNumFrameInfo;

	int m_lightH;
	Vec3 m_cameraPos;
	std::list<int> m_entityModelHnaldeList;
	std::list<CarData> m_modelCarList;

	int m_rankingAlpha;
	bool m_isRankingAlpha;
	std::vector<RankingInfo> m_rankingInfo;
	int m_createFrame;
	int m_createCount;
	bool m_isCreate;
	bool m_isRankingMode;
	bool m_isRankingEnd;
};

