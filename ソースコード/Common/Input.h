#pragma once
#include <unordered_map>
#include <string>
#include "Geometry/Vec2.h"

struct TriggerData
{
	float LT = 0.0f;
	float RT = 0.0f;
	Vec2 LStick = {};
	Vec2 RStick = {};
};

using InputTable_t = std::unordered_map<std::string, int>;
using InputData_t = std::unordered_map<int, int>;
using InputTriggerData_t = std::unordered_map<int, TriggerData>;

class Input
{
private :
	Input();

	Input(const Input& input) = delete;
	void operator=(const Input& input) = delete;

public:
	static Input& GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();
	/// <summary>
	/// 入力情報の更新
	/// </summary>
	/// <param name="no">タイプ番号</param>
	void Update(int no);

	/// <summary>
	/// 押されているか
	/// </summary>
	/// <param name="no">タイプ番号</param>
	/// <param name="command">コマンド</param>
	/// <returns>true : 押している / false : 押していない</returns>
	bool IsPress(int no, const char* const command) const;
	/// <summary>
	/// 押した瞬間
	/// </summary>
	/// <param name="no">タイプ番号</param>
	/// <returns>true : 押した瞬間 / false : 押していないor押し続けている</returns>
	bool IsTriggerd(int no, const char* const command) const;
	/// <summary>
	/// 離した瞬間
	/// </summary>
	/// <param name="no">タイプ番号</param>
	/// <returns>true : 離した / false : 押していないor押し続けている</returns>
	bool IsReleased(int no, const char* const command) const;

	float GetLT(int no) const;
	float GetRT(int no) const;
	const Vec2& GetLStick(int no) const;
	const Vec2& GetRStick(int no) const;
#ifdef _DEBUG
	bool IsPress(int key) const;
	bool IsTriggerd(int key) const;
#endif

private:
	int GetInputType(int no);
	float GetTriggerRate(unsigned char trigger);
	float GetStickRate(short stick);
	float ApplyDeadZone(float rate);

private:
	// コマンドと入力のペア
	InputTable_t m_commandTable;

	// 入力の記憶
	InputData_t m_padstate;		// 現在の入力
	InputData_t m_prePadstate;	// 直前の入力
	InputTriggerData_t m_trigger;
#ifdef _DEBUG
	char m_keystate[256];
	char m_preKeystate[256];
#endif
};

