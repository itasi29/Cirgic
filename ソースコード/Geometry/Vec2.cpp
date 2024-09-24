#include "Vec2.h"
#include <cmath>
#include <cassert>

Vec2::Vec2() :
	x(0.0f),
	y(0.0f)
{
}

Vec2::Vec2(float inX, float inY) :
	x(inX),
	y(inY)
{
}

Vec2 Vec2::operator-() const
{
	return Vec2(-x, -y);
}

Vec2 Vec2::operator+(const Vec2& val) const
{
	return Vec2(x + val.x, y + val.y);
}

void Vec2::operator+=(const Vec2& val)
{
	x += val.x;
	y += val.y;
}

Vec2 Vec2::operator-(const Vec2& val) const
{
	return Vec2(x - val.x, y - val.y);
}

void Vec2::operator-=(const Vec2& val)
{
	x -= val.x;
	y -= val.y;
}

Vec2 Vec2::operator*(float scale) const
{
	return Vec2(x * scale, y * scale);
}

void Vec2::operator*=(float scale)
{
	x *= scale;
	y *= scale;
}

Vec2 Vec2::operator/(float scale) const
{
	if (SqLength() == 0.0f)
	{
		assert(false);
		return Vec2();
	}

	return Vec2(x / scale, y / scale);
}

void Vec2::operator/=(float scale)
{
	if (SqLength() > 0.0f)
	{
		x /= scale;
		y /= scale;
	}
	else
	{
		assert(false);
		x = 0.0f;
		y = 0.0f;
	}
}

bool Vec2::operator==(const Vec2& val) const
{
	bool isX = x == val.x;
	bool isY = y == val.y;
	return isX && isY;
}

bool Vec2::operator!=(const Vec2& val) const
{
	bool isX = x != val.x;
	bool isY = y != val.y;
	return isX || isY;
}

float Vec2::Length() const
{
	return std::hypot(x, y);
}

float Vec2::SqLength() const
{
	return x * x + y * y;
}

Vec2 Vec2::GetNormalized() const
{
	Vec2 ref(0.0f, 0.0f);
	float len = Length();

	if (len > 0.0f)
	{
		ref = *this / len;
	}

	return ref;
}

void Vec2::Normalize()
{
	float len = Length();

	if (len > 0.0f)
	{
		*this /= len;
	}
}

DxLib::VECTOR Vec2::VGet() const
{
	return DxLib::VGet(x, y, 0.0f);
}

float Dot(const Vec2& item1, const Vec2& item2)
{
	return item1.x * item2.x + item1.y * item2.y;
}

float Cross(const Vec2& item1, const Vec2& item2)
{
	return item1.x * item2.y - item1.y * item2.x;
}

Vec2 Projection(const Vec2& projection, const Vec2& base)
{
	auto projectionN = projection.GetNormalized();
	return projectionN * Dot(base, projectionN);
}

bool IsSameDirection(const Vec2& dir1, const Vec2& dir2)
{
	auto dirN1 = dir1.GetNormalized();
	auto dirN2 = dir2.GetNormalized();

	// è¨êîëÊ3à Ç‹Ç≈ÇÃî‰ärÇ∆Ç∑ÇÈ
	const int digit = 100;
	int x1 = static_cast<int>(dirN1.x * digit);
	int y1 = static_cast<int>(dirN1.y * digit);
	int x2 = static_cast<int>(dirN2.x * digit);
	int y2 = static_cast<int>(dirN2.y * digit);

	bool isX = x1 == x2;
	bool isY = y1 == y2;

	return isX && isY;
}
