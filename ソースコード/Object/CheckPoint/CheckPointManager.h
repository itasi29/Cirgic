#pragma once
#include <memory>
#include <vector>

struct Vec3;
class CheckPoint;
class RaceManager;

class CheckPointManager final
{
public:
	CheckPointManager();

	void Init(const std::weak_ptr<RaceManager>& raceMgr);
	void InitPlayerNum(int num);
	void End();

	void UpdateCheckPoint(int carNo, int cpNo);

	int GetNowCpNo(int carNo) const;
	int GetCpNum() const;
	const Vec3& GetNowCpPos(int carNo) const;
	const Vec3& GetNextCpPos(int carNo) const;

	void DebugDraw() const;

private:
	void LoadAndCreate(const wchar_t* path);

private:
	std::weak_ptr<RaceManager> m_raceMgr;
	// CP�̈ʒu���
	std::vector<std::shared_ptr<CheckPoint>> m_cps;
	// �K���ʂ�Ȃ���΂Ȃ�Ȃ�CP�ԍ�
	std::vector<int> m_mustCp;

	// �ԕʂ̒ʂ���CP�̏ꏊ
	std::vector<int> m_carCp;
	// �ԕʂ̐�΂ɒʂ�ׂ�CP�̃C���f�b�N�X�ԍ�
	std::vector<int> m_carMustPtIndex;
};

