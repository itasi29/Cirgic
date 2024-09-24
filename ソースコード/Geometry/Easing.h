#pragma once
#include <cmath>
#include <algorithm>
#include "Common/Game.h"
#include "Vec3.h"
#include "Quaternion.h"

// “™‘¬
template <typename T>
T Lerp(const T& start, const T& end, float t)
{
	return static_cast<T>(start + (end - start) * t);
}
Vec3 Lerp(const Vec3& start, const Vec3& end, float t);
// ‰Á‘¬
template <typename T>
T EaseIn(const T& start, const T& end, float t, float effect = 2.0f)
{
	float rate = std::powf(t, effect);
	return Lerp(start, end, rate);
}
Vec3 EaseIn(const Vec3& start, const Vec3& end, float t, float effect = 2.0f);
// Œ¸‘¬
template <typename T>
T EaseOut(const T& start, const T& end, float t, float effect = 2.0f)
{
	float rate = 1.0f - std::powf(1.0f - t, effect);
	return Lerp(start, end, rate);
}
Vec3 EaseOut(const Vec3& start, const Vec3& end, float t, float effect = 2.0f);
// ‰Á‘¬‚µ‚ÄŒ¸‘¬
template <typename T>
T EaseInOut(const T& start, const T& end, float t, float effect = 2.0f)
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
Vec3 EaseInOut(const Vec3& start, const Vec3& end, float t, float effect = 2.0f);

// ‹…–ÊüŒ`•âŠÔ
Vec3 Slerp(const Vec3& v1, const Vec3& v2, float t);
Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);