#pragma once
#include <DxLib.h>
#include "Vec2.h"

/// <summary>
/// ベクター3
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

	/* 演算 */
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

	// XZ方向のみを取得
	Vec2 GetXZDir() const;

	// 長さ取得
	float Length() const;
	// 長さの2乗を取得
	float SqLength() const;
	// 正規化したベクトルを取得
	Vec3 GetNormalized() const;
	// 自身の正規化
	void Normalize();
	// 自身の絶対値化
	void Abs();

	// DxLib用のVECTORを取得
	DxLib::VECTOR VGet() const;
};

// 内積
float Dot(const Vec3& item1, const Vec3& item2);
// 外積
Vec3 Cross(const Vec3& item1, const Vec3& item2);
// 射影
Vec3 Projection(const Vec3& projection, const Vec3& base);
// 反射
Vec3 Reflection(const Vec3& vec, const Vec3& norm);
// 同じ方向を向いているか(誤差許容)
bool IsSameDirection(const Vec3& dir1, const Vec3& dir2);
// ある程度同じ方向を向いているか
bool IsUniformDirection(const Vec3& dir1, const Vec3& dir2);