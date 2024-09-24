#pragma once
#include <DxLib.h>

/// <summary>
/// �x�N�^�[2
/// </summary>
struct Vec2
{
public:
	float x;
	float y;
	
	Vec2();
	Vec2(float inX, float inY);

	/* ���Z */
	Vec2 operator-() const;
	Vec2 operator+(const Vec2& val) const;
	void operator+=(const Vec2& val);
	Vec2 operator-(const Vec2& val) const;
	void operator-=(const Vec2& val);
	Vec2 operator*(float scale) const;
	void operator*=(float scale);
	Vec2 operator/(float scale) const;
	void operator/=(float scale);
	bool operator==(const Vec2& val) const;
	bool operator!=(const Vec2& val) const;

	// �����擾
	float Length() const;
	// ������2����擾
	float SqLength() const;
	// ���K�������x�N�g�����擾
	Vec2 GetNormalized() const;
	// ���g�̐��K��
	void Normalize();

	// DxLib�p��VECTOR���擾
	DxLib::VECTOR VGet() const;
};

// ����
float Dot(const Vec2& item1, const Vec2& item2);
// �O��
float Cross(const Vec2& item1, const Vec2& item2);
// �ˉe
Vec2 Projection(const Vec2& projection, const Vec2& base);
// ���������������Ă��邩(�덷���e)
bool IsSameDirection(const Vec2& dir1, const Vec2& dir2);
