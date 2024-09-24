#pragma once
#include "Vec3.h"
#include "Quaternion.h"

// 円同士が当たっているか判定
bool IsCollideSphereAndSphere(const Vec3& pt1, const Vec3& pt2, float radius1, float radius2);
// 円とカプセルが当たっているか判定
bool IsCollideSphereAndCapsule(const Vec3& pt, const Vec3& center, const Vec3& dir, float radius1, float radius2);

// 線分同士が平行か判定
bool IsParallelOnLine(const Vec3& startA, const Vec3& endA, const Vec3& startB, const Vec3& endB);
// 線分が面をまたいでいるか判定
bool IsCrossSurfaceOnLine(const Vec3& start, const Vec3& end, const Vec3& center, const Vec3& norm);

// 最近接点を取得
Vec3 GetNearestPtOnLine(const Vec3& point, const Vec3& start, const Vec3& end);
// 線分上に最近接点があるかの判定
bool IsNearestPointOnLine(const Vec3& point, const Vec3& start, const Vec3& end);

// 線分同士の最近接点を取得
void GetNearestPtOnLine(const Vec3& centerA, const Vec3& dirA, const Vec3& centerB, const Vec3& dirB, Vec3& outPtA, Vec3& outPtB);

// 線と面の交点を取得
Vec3 GetCrossPtSurfaceAndLine(const Vec3& start, const Vec3& end, const Vec3& center, const Vec3& norm);
// 点と面の交点を取得
Vec3 GetCrossPtSurfaceAndPt(const Vec3& pt, const Vec3& center, const Vec3& norm);

// 球と面のヒット判定
bool IsSphereHitSurface(const Vec3& point, float radius, const Vec3& center, const Vec3& norm, const Quaternion& rot, const Vec2& size);

// 2つの線分の回転クオータオンを取得
Quaternion GetQuaternion(const Vec3& v1, const Vec3& v2);