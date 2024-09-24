#include "Easing.h"
#include <cmath>
#include <algorithm>
#include "Common/Game.h"

Vec3 Lerp(const Vec3& start, const Vec3& end, float t)
{
    return start + (end - start) * t;
}

Vec3 EaseIn(const Vec3& start, const Vec3& end, float t, float effect)
{
    float rate = std::powf(t, effect);
    return Lerp(start, end, rate);
}

Vec3 EaseOut(const Vec3& start, const Vec3& end, float t, float effect)
{
    float rate = 1.0f - std::powf(1.0f - t, effect);
    return Lerp(start, end, rate);
}

Vec3 EaseInOut(const Vec3& start, const Vec3& end, float t, float effect)
{
	float rate;
	if (t < 0.5f)
	{
		rate = 2 * (effect - 1.0f) * std::powf(t, effect);
	}
	else
	{
		rate = 1.0f - std::powf(-2 * t + 2, effect) * 0.5f;
	}
	return Lerp(start, end, rate);
}

Vec3 Slerp(const Vec3& v1, const Vec3& v2, float t)
{
	float dot = Dot(v1.GetNormalized(), v2.GetNormalized());
	float angle = std::acosf(dot) * Game::RAD_2_DEG;
	auto axis = Cross(v1.GetNormalized(), v2.GetNormalized());
	auto q = AngleAxis(angle * t, axis);

	return q * v1;
}

Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float dot = Dot(q1, q2);

	if (dot >= 1.0f)
	{
		return q1;
	}

	float angle = std::acosf(dot);

	angle = std::abs(angle);

	float st = std::sinf(angle);

	float s1 = std::sinf(angle * t) / st;
	float s2 = std::sinf(angle * (1.0f - t)) / st;

	Quaternion res;
	res.x = s1 * q1.x + s2 * q2.x;
	res.y = s1 * q1.y + s2 * q2.y;
	res.z = s1 * q1.z + s2 * q2.z;
	res.w = s1 * q1.w + s2 * q2.w;

	assert(!isnan(res.x));
	assert(!isnan(res.y));
	assert(!isnan(res.z));

	return res;
}
