#pragma once
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Vec3.h"

struct Quaternion;

/// <summary>
/// 4x4sρ
/// </summary>
struct Matrix4x4
{
public:
	float m[4][4];
	// s->ρ

	Matrix4x4();

	/* Z */
	Matrix4x4 operator+ (const Matrix4x4& mat) const;
	Matrix4x4 operator- (const Matrix4x4& mat) const;
	Matrix4x4 operator* (const Matrix4x4& mat) const;
	Matrix4x4 operator* (const Quaternion& q) const;
	Matrix4x4 operator/ (float div) const;

	// [
	void Zero();
	// PΚsρ
	void Identity();
	// ]usρ
	Matrix4x4 Transpose();
	// tsρ
	Matrix4x4 Inverse(bool isCorrect = false);

	// |WVΜέθ
	Vec3 GetPos() const;
	Quaternion GetRot() const;

	// DxLibΟ·p
	MATRIX GetMATRIX() const;
	Effekseer::Matrix43 GetEffMat() const;

private:
	float Dot(const Matrix4x4& mat, int line, int row) const;
};

Matrix4x4 Scale(const Vec3& size);
Matrix4x4 Pos(const Vec3& pos);
Matrix4x4 Rot(const Quaternion& q);
