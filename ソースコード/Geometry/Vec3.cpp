#include "Vec3.h"
#include <cmath>
#include <cassert>

Vec3::Vec3() :
    x(0.0f),
    y(0.0f),
    z(0.0f)
{
}

Vec3::Vec3(float inX, float inY, float inZ) :
    x(inX),
    y(inY),
    z(inZ)
{
}

Vec3::Vec3(const Vec2& vec2) :
	x(vec2.x),
	y(vec2.y),
	z(0.0f)
{
}

Vec3 Vec3::Right()
{
	return Vec3(1.0f, 0.0f, 0.0f);
}

Vec3 Vec3::Up()
{
	return Vec3(0.0f, 1.0f, 0.0f);
}

Vec3 Vec3::Front()
{
	return Vec3(0.0f, 0.0f, 1.0f);
}

Vec3 Vec3::Down()
{
	return Vec3(0.0f, -1.0f, 0.0f);
}

Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator+(const Vec3& val) const
{
	return Vec3(x + val.x, y + val.y, z + val.z);
}

void Vec3::operator+=(const Vec3& val)
{
	x += val.x;
	y += val.y;
	z += val.z;
}

Vec3 Vec3::operator-(const Vec3& val) const
{
	return Vec3(x - val.x, y - val.y, z - val.z);
}

void Vec3::operator-=(const Vec3& val)
{
	x -= val.x;
	y -= val.y;
	z -= val.z;
}

Vec3 Vec3::operator*(float scale) const
{
	return Vec3(x * scale, y * scale, z * scale);
}

void Vec3::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

Vec3 Vec3::operator/(float scale) const
{
	if (scale == 0.0f) 
	{
		assert(false);
		return Vec3();
	}

	return Vec3(x / scale, y / scale, z / scale);
}

void Vec3::operator/=(float scale)
{
	if (SqLength() > 0.0f) 
	{
		x /= scale;
		y /= scale;
		z /= scale;
	}
	else
	{
		assert(false);
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
}

bool Vec3::operator==(const Vec3& val) const
{
	bool isX = x == val.x;
	bool isY = y == val.y;
	bool isZ = z == val.z;
	return isX && isY && isZ;
}

bool Vec3::operator!=(const Vec3& val) const
{
	bool isX = x != val.x;
	bool isY = y != val.y;
	bool isZ = z != val.z;
	return isX || isY || isZ;
}

Vec2 Vec3::GetXZDir() const
{
	return Vec2(x, z);
}

float Vec3::Length() const
{
	return std::hypot(x, y, z);
}

float Vec3::SqLength() const
{
	return x * x + y * y + z * z;
}

Vec3 Vec3::GetNormalized() const
{
	Vec3 ref(0.0f, 0.0f, 0.0f);
	float len = Length();

	if (len > 0.0f)
	{
		ref = *this / len;
	}

    return ref;
}

void Vec3::Normalize()
{
	float len = Length();

	if (len > 0.0f)
	{
		*this /= len;
	}
}

void Vec3::Abs()
{
	x = std::abs(x);
	y = std::abs(y);
	z = std::abs(z);
}

DxLib::VECTOR Vec3::VGet() const
{
    return DxLib::VGet(x, y, z);
}

float Dot(const Vec3& item1, const Vec3& item2)
{
	return item1.x * item2.x + item1.y * item2.y + item1.z * item2.z;
}

Vec3 Cross(const Vec3& item1, const Vec3& item2)
{
	Vec3 result;

	result.x = item1.y * item2.z - item1.z * item2.y;
	result.y = item1.z * item2.x - item1.x * item2.z;
	result.z = item1.x * item2.y - item1.y * item2.x;

	return result;
}

Vec3 Projection(const Vec3& projection, const Vec3& dir)
{
	auto dirN = dir.GetNormalized();
	auto d = Dot(dirN, projection);
	return dirN * d;
}

Vec3 Reflection(const Vec3& vec, const Vec3& norm)
{
	auto n = Projection(vec, norm);
	return vec - n * 2;
}

bool IsSameDirection(const Vec3& dir1, const Vec3& dir2)
{
	auto dirN1 = dir1.GetNormalized();
	auto dirN2 = dir2.GetNormalized();

	// è¨êîëÊ3à Ç‹Ç≈ÇÃî‰ärÇ∆Ç∑ÇÈ
	const int digit = 100;
	int x1 = static_cast<int>(dirN1.x * digit);
	int y1 = static_cast<int>(dirN1.y * digit);
	int z1 = static_cast<int>(dirN1.z * digit);
	int x2 = static_cast<int>(dirN2.x * digit);
	int y2 = static_cast<int>(dirN2.y * digit);
	int z2 = static_cast<int>(dirN2.z * digit);

	bool isX = x1 == x2;
	bool isY = y1 == y2;
	bool isZ = z1 == z2;

	return isX && isY && isZ;
}

bool IsUniformDirection(const Vec3& dir1, const Vec3& dir2)
{
	auto dirN1 = dir1.GetNormalized();
	auto dirN2 = dir2.GetNormalized();

	float dot = Dot(dirN1, dirN2);

	return dot > 0.0f;
}
