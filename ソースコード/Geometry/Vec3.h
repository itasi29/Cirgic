#pragma once
#include <DxLib.h>
#include "Vec2.h"

/// <summary>
/// �x�N�^�[3
/// </summary>
struct Vec3
{
public:
	float x;
	float y;
	float z;

	Vec3();
	Vec3(float inX, float inY, float inZ);
	Vec3(const Vec2& vec2);

	static Vec3 Right();
	static Vec3 Up();
	static Vec3 Front();
	static Vec3 Down();

	/* ���Z */
	Vec3 operator-() const;
	Vec3 operator+(const Vec3& val) const;
	void operator+=(const Vec3& val);
	Vec3 operator-(const Vec3& val) const;
	void operator-=(const Vec3& val);
	Vec3 operator*(float scale) const;
	void operator*=(float scale);
	Vec3 operator/(float scale) const;
	void operator/=(float scale);
	bool operator==(const Vec3& val) const;
	bool operator!=(const Vec3& val) const;

	// XZ�����݂̂��擾
	Vec2 GetXZDir() const;

	// �����擾
	float Length() const;
	// ������2����擾
	float SqLength() const;
	// ���K�������x�N�g�����擾
	Vec3 GetNormalized() const;
	// ���g�̐��K��
	void Normalize();
	// ���g�̐�Βl��
	void Abs();

	// DxLib�p��VECTOR���擾
	DxLib::VECTOR VGet() const;
};

// ����
float Dot(const Vec3& item1, const Vec3& item2);
// �O��
Vec3 Cross(const Vec3& item1, const Vec3& item2);
// �ˉe
Vec3 Projection(const Vec3& projection, const Vec3& base);
// ����
Vec3 Reflection(const Vec3& vec, const Vec3& norm);
// ���������������Ă��邩(�덷���e)
bool IsSameDirection(const Vec3& dir1, const Vec3& dir2);
// ������x���������������Ă��邩
bool IsUniformDirection(const Vec3& dir1, const Vec3& dir2);