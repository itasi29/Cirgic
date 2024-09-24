#include "Quaternion.h"
#include <cmath>
#include "Common/Game.h"
#include <algorithm>

// HACK: ‰Ê‚½‚µ‚Ä‰Šú‰»Žq‚±‚ê‚Å‚¢‚¢‚Ì‚©...
Quaternion::Quaternion() :
    x(0.0f),
    y(0.0f),
    z(0.0f),
    w(0.0f)
{
}

Quaternion::Quaternion(float inX, float inY, float inZ, float inW) :
    x(inX),
    y(inY),
    z(inZ),
    w(inW)
{
}

Quaternion Quaternion::Conjugated() const
{
    return Quaternion(-x, -y, -z, w);
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
    Quaternion res;

    res.x = x * q.w + w * q.x - z * q.y + y * q.z;
    res.y = y * q.w + z * q.x + w * q.y - x * q.z;
#if false
    res.z = z * q.w - y * q.x + x * q.y + w * q.z;
#endif
    auto l1 = z * q.w;
    auto l2 = y * q.x;
    auto l3 = x * q.y;
    auto l4 = w * q.z;
    auto aa = l1 - l2;
    auto bb = l3 + l4;
    auto l = aa + bb;
    res.z = l;
    res.w = w * q.w - x * q.x - y * q.y - z * q.z;

    return res;
}

Matrix4x4 Quaternion::operator*(const Matrix4x4& mat) const
{
    return GetMat() * mat;
}

Vec3 Quaternion::operator*(const Vec3& vec) const
{
    Quaternion posQ = Quaternion(vec.x, vec.y, vec.z, 1.0f);

    Quaternion newPos = *this * posQ * this->Conjugated();
    
    return Vec3(newPos.x, newPos.y, newPos.z);
}

Matrix4x4 Quaternion::GetMat() const
{
    Matrix4x4 matQ;

    float x2 = x * x;
    float y2 = y * y;
    float z2 = z * z;
    float w2 = w * w;

    float r00 = x2 - y2 - z2 + w2;
    float r01 = 2.0f * (x * y + z * w);
    float r02 = 2.0f * (x * z - y * w);

    float r10 = 2.0f * (x * y - z * w);
    float r11 = -x2 + y2 - z2 + w2;
    float r12 = 2.0f * (y * z + x * w);

    float r20 = 2.0f * (x * z + y * w);
    float r21 = 2.0f * (y * z - x * w);
    float r22 = -x2 - y2 + z2 + w2;

    matQ.m[0][0] = r00;
    matQ.m[0][1] = r01;
    matQ.m[0][2] = r02;
    matQ.m[1][0] = r10;
    matQ.m[1][1] = r11;
    matQ.m[1][2] = r12;
    matQ.m[2][0] = r20;
    matQ.m[2][1] = r21;
    matQ.m[2][2] = r22;

    matQ.m[3][3] = 1.0f;

    return matQ;
}

Vec3 Quaternion::GetRadian() const
{
    float q00 = w * w;
    float q11 = x * x;
    float q22 = y * y;
    float q33 = z * z;
    float q01 = w * x;
    float q02 = w * y;
    float q03 = w * z;
    float q12 = x * y;
    float q13 = x * z;
    float q23 = y * z;

    Vec3 res;

    res.x = std::atan2f(2.0f * (q23 + q01), (q00 - q11 - q22 + q33));
    res.y = -std::asinf(2.0f * (q13 - q02));
    res.z = std::atan2f(2.0f * (q12 + q03), (q00 + q11 - q22 - q33));

    return res;
}

Quaternion AngleAxis(float angle, const Vec3& axis)
{
    Quaternion result;

    float halfRad = angle * Game::DEG_2_RAD * 0.5f;
    float sin = std::sin(halfRad);
    float cos = std::cos(halfRad);
    auto normAxis = axis.GetNormalized();

    result = Quaternion(normAxis.x * sin, normAxis.y * sin, normAxis.z * sin, cos);

    return result;
}

float Dot(const Quaternion& q1, const Quaternion& q2)
{
    return q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
}
