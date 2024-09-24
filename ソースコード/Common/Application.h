#pragma once
#include <random>

class Application
{
private:
	Application();
	
	Application(const Application& app) = delete;
	void operator= (const Application& app) = delete;

public:
	static Application& GetInstance();

	bool Init();
	void Run();
	void Terminate();

	// ゲームの終了
	void End() { m_isEnd = true; }

	// 乱数取得
	int GetRand();
	int GetRand(int min, int max);
	float GetRand(float min, float max);

	// 画面の明るさ
	bool AddScreenLight(int val);
	bool SubScreenLight(int val);
	int GetScreenLight() const { return m_screenLight; }

private:
	bool m_isEnd;
	bool m_isWindows;
	int m_screenLight;

	std::mt19937 m_mt;
};

