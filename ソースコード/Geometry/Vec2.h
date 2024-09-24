#pragma once
#include <DxLib.h>

/// <summary>
/// ベクター2
/// </summary>
struct Vec2
{
public:
	float x;
	float y;
	
	Vec2();
	Vec2(float inX, float inY);

	/* 演算 */
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

	// 長さ取得
	float Length() const;
	// 長さの2乗を取得
	float SqLength() const;
	// 正規化したベクトルを取得
	Vec2 GetNormalized() const;
	// 自身の正規化
	void Normalize();

	// DxLib用のVECTORを取得
	DxLib::VECTOR VGet() const;
};

// 内積
float Dot(const Vec2& item1, const Vec2& item2);
// 外積
float Cross(const Vec2& item1, const Vec2& item2);
// 射影
Vec2 Projection(const Vec2& projection, const Vec2& base);
// 同じ方向を向いているか(誤差許容)
bool IsSameDirection(const Vec2& dir1, const Vec2& dir2);
