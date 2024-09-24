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
	/// ����������
	/// </summary>
	void Init();
	/// <summary>
	/// ���͏��̍X�V
	/// </summary>
	/// <param name="no">�^�C�v�ԍ�</param>
	void Update(int no);

	/// <summary>
	/// ������Ă��邩
	/// </summary>
	/// <param name="no">�^�C�v�ԍ�</param>
	/// <param name="command">�R�}���h</param>
	/// <returns>true : �����Ă��� / false : �����Ă��Ȃ�</returns>
	bool IsPress(int no, const char* const command) const;
	/// <summary>
	/// �������u��
	/// </summary>
	/// <param name="no">�^�C�v�ԍ�</param>
	/// <returns>true : �������u�� / false : �����Ă��Ȃ�or���������Ă���</returns>
	bool IsTriggerd(int no, const char* const command) const;
	/// <summary>
	/// �������u��
	/// </summary>
	/// <param name="no">�^�C�v�ԍ�</param>
	/// <returns>true : ������ / false : �����Ă��Ȃ�or���������Ă���</returns>
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
	// �R�}���h�Ɠ��͂̃y�A
	InputTable_t m_commandTable;

	// ���͂̋L��
	InputData_t m_padstate;		// ���݂̓���
	InputData_t m_prePadstate;	// ���O�̓���
	InputTriggerData_t m_trigger;
#ifdef _DEBUG
	char m_keystate[256];
	char m_preKeystate[256];
#endif
};

