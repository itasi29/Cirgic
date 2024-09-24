#include "MathHelp.h"
#include <cmath>
#include <algorithm>
#include "Vec2.h"
#include "Common/Game.h"

bool IsCollideSphereAndSphere(const Vec3& pt1, const Vec3& pt2, float radius1, float radius2)
{
	auto sqLen = (pt1 - pt2).SqLength();
	auto sumRadius = radius1 + radius2;
	return sqLen < sumRadius * sumRadius;
}

bool IsCollideSphereAndCapsule(const Vec3& pt, const Vec3& center, const Vec3& dir, float radius1, float radius2)
{
	auto capsuleStart = center + dir;
	auto capsuleEnd = center - dir;

	auto nearPt = GetNearestPtOnLine(pt, capsuleStart, capsuleEnd);

	return IsCollideSphereAndSphere(pt, nearPt, radius1, radius2);
}

bool IsParallelOnLine(const Vec3& startA, const Vec3& endA, const Vec3& startB, const Vec3& endB)
{
	auto vA = endA - startA;
	auto vB = endB - startB;

	auto cross = Cross(vA, vB);

	return cross.SqLength() < 0.001f;
}

bool IsCrossSurfaceOnLine(const Vec3& start, const Vec3& end, const Vec3& center, const Vec3& norm)
{
	auto centerToStart = start - center;
	auto centerToEnd = end - center;

	// �����Ƃ��s�p�Ȃ�Ζʂ��܂����ł��Ȃ�
	return (Dot(norm, centerToStart) * Dot(norm, centerToEnd) < 0);
}

Vec3 GetNearestPtOnLine(const Vec3& point, const Vec3& start, const Vec3& end)
{
	Vec3 startToEnd = end - start;
	Vec3 startToPoint = point - start;

	// ����̂ǂ̕ӂ�
	float t = Dot(startToEnd, startToPoint) / startToEnd.SqLength();
	// �r������
	t = std::fmax(std::fmin(t, 1.0f), 0.0f);

	return start + startToEnd * t;
}

bool IsNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end)
{
	Vec3 startToEnd = end - start;
	Vec3 startToPoint = point - start;

	// ����̂ǂ̕ӂ�
	float t = Dot(startToEnd, startToPoint) / startToEnd.SqLength();

	// t��������Ȃ�true
	return 0.0f <= t && t <= 1.0f;
}

void GetNearestPtOnLine(const Vec3& centerA, const Vec3& dirA, const Vec3& centerB, const Vec3& dirB, Vec3& outPtA, Vec3& outPtB)
{
	// ���΃x�N�g��
	Vec3 vec = centerB - centerA;

	// �@���x�N�g��
	Vec3 norm = Cross(dirA, dirB);
	// ���s����
//	bool isParallel = norm.SqLength() < 0.001f;

	float s, t;
#if false
	// ���s�łȂ��ꍇ
	if (!isParallel)
	{
		Vec3 v1, v2;
		v1.x = dirA.x;
		v1.y = -dirB.x;
		v1.z = norm.x;
		v2.x = dirA.y;
		v2.y = -dirB.y;
		v2.z = norm.y;

		s = Dot(v1, vec) / v1.SqLength();
		t = Dot(v2, vec) / v2.SqLength();

		s = Dot(dirA, vec) / dirA.SqLength();
		t = Dot(dirB, -vec) / dirB.SqLength();
	}
	// ���s�̏ꍇ
	else
	{
		s = Dot(dirA, vec) / dirA.SqLength();
		t = Dot(dirB, -vec) / dirB.SqLength();
	}
#endif

	s = Dot(dirA, vec) / dirA.SqLength();
	t = Dot(dirB, -vec) / dirB.SqLength();

	// �͈͂̐���
	s = std::min<float>(std::max<float>(s, -1.0f), 1.0f);
	t = std::min<float>(std::max<float>(t, -1.0f), 1.0f);

	outPtA = dirA * s + centerA;
	outPtB = dirB * t + centerB;
}

Vec3 GetCrossPtSurfaceAndLine(const Vec3& start, const Vec3& end, const Vec3& center, const Vec3& norm)
{
	auto centerToStart = start;
	auto centerToEnd = end;

	// ���̃X�^�[�g�E�S�[���Ɩʂ܂ł̒������擾
	auto startSurfaceLen = Projection(centerToStart, norm).Length();
	auto endSurfaceLen = Projection(centerToEnd, norm).Length();
	// ��_��������̉��������肩���v�Z
	auto rate = startSurfaceLen / (startSurfaceLen + endSurfaceLen);
	// ��_���擾
	return centerToStart * (1 - rate) + centerToEnd * rate;
}

Vec3 GetCrossPtSurfaceAndPt(const Vec3& pt, const Vec3& center, const Vec3& norm)
{
	auto ptToCenter = center - pt;
	auto proj = Projection(ptToCenter, norm);
	return pt + proj;
}

bool IsSphereHitSurface(const Vec3& point, float radius, const Vec3& center, const Vec3& norm, const Quaternion& rot, const Vec2& size)
{
	// �~�̒��S����ʂ܂ł̐����쐬
	auto ptToCenter = -norm * radius;
	auto ptToCenterPos = point + ptToCenter;

	// �܂�������
//	if (!IsCrossSurfaceOnLine(point, ptToCenterPos, center, norm)) return false;

	// ���Ɩʂ̌�_�����߂�
	auto crossPt = GetCrossPtSurfaceAndLine(point, ptToCenterPos, center, norm);

	// ���E�����E�O������̒������m�F���đ傫���ȓ��Ȃ瓖�����Ă���
	auto right = rot * Vec3::Right();
	auto front = rot * Vec3::Front();
	auto centerToCross = crossPt - center;
	auto halfSize = size * 0.5f;
	auto rp = Projection(centerToCross, right);
	auto fp = Projection(centerToCross, front);
	bool isWidth = rp.SqLength() < (halfSize.x) * (halfSize.x);
	bool isFront = fp.SqLength() < (halfSize.y) * (halfSize.y);

	if (rp.SqLength() > (halfSize.x) * (halfSize.x))
	{
		rp = rp.GetNormalized() * halfSize.x;
	}
	if (fp.SqLength() > (halfSize.y) * (halfSize.y))
	{
		fp = fp.GetNormalized() * halfSize.y;
	}

	crossPt = rp + fp + center;

	return (crossPt - point).SqLength() < radius * radius;

	return isWidth && isFront;
}

Quaternion GetQuaternion(const Vec3& v1, const Vec3& v2)
{
	float dot = Dot(v1, v2);
	float angle = std::acosf(dot) * Game::RAD_2_DEG;
	auto axis = Cross(v2.GetNormalized(), v1.GetNormalized());

	return AngleAxis(angle, axis);
}