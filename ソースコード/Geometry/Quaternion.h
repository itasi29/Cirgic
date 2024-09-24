#pragma once
#include "Vec3.h"
#include "Matrix4x4.h"

/// <summary>
/// クオータニオン
/// </summary>
struct Quaternion
{
public:
	float x;
	float y;
	float z;
	float w;

	Quaternion();
	Quaternion(float x, float y, float z, float w);

	Quaternion Conjugated() const;

	Quaternion operator* (const Quaternion& q) const;
	Matrix4x4 operator* (const Matrix4x4& mat) const;
	Vec3 operator* (const Vec3& vec) const;

	Matrix4x4 GetMat() const;
	Vec3 GetRadian() const;
};

Quaternion AngleAxis(float angle, const Vec3& axis);
float Dot(const Quaternion& q1, const Quaternion& q2);

