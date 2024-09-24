#include "Matrix4x4.h"
#include <cmath>
#include "Quaternion.h"

/// <summary>
/// テスト2
/// </summary>
Matrix4x4::Matrix4x4() :
    m{}
{
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& mat) const
{
    Matrix4x4 result;

    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            result.m[row][line] = m[row][line] + mat.m[row][line];
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& mat) const
{
    Matrix4x4 result;

    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            result.m[row][line] = m[row][line] - mat.m[row][line];
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mat) const
{
    Matrix4x4 result;

    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            result.m[row][line] = Dot(mat, line, row);
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::operator*(const Quaternion& q) const
{
    return *this * q.GetMat();
}

Matrix4x4 Matrix4x4::operator/(float div) const
{
    Matrix4x4 result;

    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            result.m[row][line] = m[row][line] / div;
        }
    }

    return result;
}

void Matrix4x4::Zero()
{
    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            m[row][line] = 0.0f;
        }
    }
}

void Matrix4x4::Identity()
{
    Zero();
    m[0][0] = 1.0f;
    m[1][1] = 1.0f;
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;
}

Matrix4x4 Matrix4x4::Transpose()
{
    Matrix4x4 result;

    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            result.m[row][line] = m[line][row];
        }
    }

    return result;
}

Matrix4x4 Matrix4x4::Inverse(bool isCorrect)
{
    Matrix4x4 result;

    // 簡略なやり方
    if (!isCorrect)
    {
        // 回転部分の転置
        for (int row = 0; row < 3; ++row)
        {
            for (int line = 0; line < 3; ++line)
            {
                result.m[row][line] = m[line][row];
            }
        }
        // 平行移動部分の値反転
        for (int i = 0; i < 3; ++i)
        {
            result.m[3][i] = -m[3][i];
        }
    }
    // 正確なやり方
    else
    {
        float det;
        // REVIEW: 間違ってたら公式見直して
        det = m[0][0] * m[1][1] * m[2][2] * m[3][3] +
              m[0][0] * m[1][2] * m[2][3] * m[3][1] +
              m[0][0] * m[1][3] * m[2][1] * m[3][2] +
              m[0][1] * m[1][0] * m[2][3] * m[3][2] +
              m[0][1] * m[1][2] * m[2][0] * m[3][3] +
              m[0][1] * m[1][3] * m[2][2] * m[3][0] +
              m[0][2] * m[1][0] * m[2][1] * m[3][3] +
              m[0][2] * m[1][1] * m[2][3] * m[3][0] +
              m[0][2] * m[1][3] * m[2][0] * m[3][1] +
              m[0][3] * m[1][0] * m[2][2] * m[3][1] +
              m[0][3] * m[1][1] * m[2][0] * m[3][2] +
              m[0][3] * m[1][2] * m[2][1] * m[3][0] -
              m[0][0] * m[1][1] * m[2][3] * m[3][2] -
              m[0][0] * m[1][2] * m[2][1] * m[3][3] -
              m[0][0] * m[1][3] * m[2][2] * m[3][1] -
              m[0][1] * m[1][0] * m[2][2] * m[3][3] -
              m[0][1] * m[1][1] * m[2][3] * m[3][0] -
              m[0][1] * m[1][3] * m[2][0] * m[3][2] -
              m[0][2] * m[1][0] * m[2][3] * m[3][1] -
              m[0][2] * m[1][1] * m[2][0] * m[3][3] -
              m[0][2] * m[1][3] * m[2][1] * m[3][0] -
              m[0][3] * m[1][0] * m[2][1] * m[3][2] -
              m[0][3] * m[1][1] * m[2][2] * m[3][0] -
              m[0][3] * m[1][2] * m[2][0] * m[3][1];

        if (det != 0.0f)
        {
            // REVIEW: 間違ってたら公式見直して
            result.m[0][0]  = m[1][1] * m[2][2] * m[3][3] + m[1][2] * m[2][3] * m[3][1] + m[1][3] * m[2][1] * m[3][2] - m[1][1] * m[2][3] * m[3][2] - m[1][2] * m[2][1] * m[3][3] - m[1][3] * m[2][2] * m[3][1];
            result.m[0][1]  = m[0][1] * m[2][3] * m[3][2] + m[0][2] * m[2][1] * m[3][3] + m[0][3] * m[2][2] * m[3][1] - m[0][1] * m[2][2] * m[3][3] - m[0][2] * m[2][3] * m[3][1] - m[0][3] * m[2][1] * m[3][2];
            result.m[0][2]  = m[0][1] * m[1][2] * m[3][3] + m[0][2] * m[1][3] * m[3][1] + m[0][3] * m[1][1] * m[3][2] - m[0][1] * m[1][3] * m[3][2] - m[0][2] * m[1][1] * m[3][3] - m[0][3] * m[1][2] * m[3][1];
            result.m[0][3]  = m[0][1] * m[1][3] * m[2][2] + m[0][2] * m[1][1] * m[2][3] + m[0][3] * m[1][2] * m[2][1] - m[0][1] * m[1][2] * m[2][3] - m[0][2] * m[1][3] * m[2][1] - m[0][3] * m[1][1] * m[2][2];

            result.m[1][0]  = m[1][0] * m[2][3] * m[3][2] + m[1][2] * m[2][0] * m[3][3] + m[1][3] * m[2][2] * m[3][0] - m[1][0] * m[2][2] * m[3][3] - m[1][2] * m[2][3] * m[3][0] - m[1][3] * m[2][0] * m[3][2];
            result.m[1][1]  = m[0][0] * m[2][2] * m[3][3] + m[0][2] * m[2][3] * m[3][0] + m[0][3] * m[2][0] * m[3][2] - m[0][0] * m[2][3] * m[3][2] - m[0][2] * m[2][0] * m[3][3] - m[0][3] * m[2][2] * m[3][0];
            result.m[1][2]  = m[0][0] * m[1][3] * m[3][2] + m[0][2] * m[1][0] * m[3][3] + m[0][3] * m[1][2] * m[3][0] - m[0][0] * m[1][2] * m[3][3] - m[0][2] * m[1][3] * m[3][0] - m[0][3] * m[1][0] * m[3][2];
            result.m[1][3]  = m[0][0] * m[1][2] * m[2][3] + m[0][2] * m[1][3] * m[2][0] + m[0][3] * m[1][0] * m[2][2] - m[0][0] * m[1][3] * m[2][2] - m[0][2] * m[1][0] * m[2][3] - m[0][3] * m[1][2] * m[2][0];

            result.m[2][0]  = m[1][0] * m[2][1] * m[3][3] + m[1][1] * m[2][3] * m[3][0] + m[1][3] * m[2][0] * m[3][1] - m[1][0] * m[2][3] * m[3][1] - m[1][1] * m[2][0] * m[3][3] - m[1][3] * m[2][1] * m[3][0];
            result.m[2][1]  = m[0][0] * m[2][3] * m[3][1] + m[0][1] * m[2][0] * m[3][3] + m[0][3] * m[2][1] * m[3][0] - m[0][0] * m[2][1] * m[3][3] - m[0][1] * m[2][3] * m[3][0] - m[0][3] * m[2][0] * m[3][1];
            result.m[2][2]  = m[0][0] * m[1][1] * m[3][3] + m[0][1] * m[1][3] * m[3][0] + m[0][3] * m[1][0] * m[3][1] - m[0][0] * m[1][3] * m[3][1] - m[0][1] * m[1][0] * m[3][3] - m[0][3] * m[1][1] * m[3][0];
            result.m[2][3]  = m[0][0] * m[1][3] * m[2][1] + m[0][1] * m[1][0] * m[2][3] + m[0][3] * m[1][1] * m[2][0] - m[0][0] * m[1][1] * m[2][3] - m[0][1] * m[1][3] * m[2][0] - m[0][3] * m[1][0] * m[2][1];

            result.m[3][0] = m[1][0] * m[2][2] * m[3][1] + m[1][1] * m[2][0] * m[3][2] + m[1][2] * m[2][1] * m[3][0] - m[1][0] * m[2][1] * m[3][2] - m[1][1] * m[2][2] * m[3][0] - m[1][2] * m[2][0] * m[3][1];
            result.m[3][1] = m[0][0] * m[2][1] * m[3][2] + m[0][1] * m[2][2] * m[3][0] + m[0][2] * m[2][0] * m[3][1] - m[0][0] * m[2][2] * m[3][1] - m[0][1] * m[2][0] * m[3][2] - m[0][2] * m[2][1] * m[3][0];
            result.m[3][2] = m[0][0] * m[1][2] * m[3][1] + m[0][1] * m[1][0] * m[3][2] + m[0][2] * m[1][1] * m[3][0] - m[0][0] * m[1][1] * m[3][2] - m[0][1] * m[1][2] * m[3][0] - m[0][2] * m[1][0] * m[3][1];
            result.m[3][3] = m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1] - m[0][0] * m[1][2] * m[2][1] - m[0][1] * m[1][0] * m[2][2] - m[0][2] * m[1][1] * m[2][0];

            result = result / det;
        }
    }
    

    return result;
}



Vec3 Matrix4x4::GetPos() const
{
    return Vec3(m[3][0], m[3][1], m[3][2]);
}

Quaternion Matrix4x4::GetRot() const
{
    float px =  m[0][0] - m[1][1] - m[2][2] + 1.0f;
    float py = -m[0][0] + m[1][1] - m[2][2] + 1.0f;
    float pz = -m[0][0] - m[1][1] + m[2][2] + 1.0f;
    float pw =  m[0][0] + m[1][1] + m[2][2] + 1.0f;

    enum Select
    {
        PX,
        PY,
        PZ,
        PW
    };
    Select select = Select::PX;
    float max = px;
    if (max < py)
    {
        select = Select::PY;
        max = py;
    }
    if (max < pz)
    {
        select = Select::PZ;
        max = pz;
    }
    if (max < pw)
    {
        select = Select::PW;
        max = pw;
    }

    Quaternion res;

    if (select == Select::PX)
    {
        float x = std::sqrtf(px) * 0.5f;
        float d = 1.0f / (4 * x);
        res.x = x;
        res.y = (m[1][0] + m[0][1]) * d;
        res.z = (m[0][2] + m[2][0]) * d;
        res.w = (m[2][1] - m[1][2]) * d;
    }
    else if (select == Select::PY)
    {
        float y = std::sqrtf(py) * 0.5f;
        float d = 1.0f / (4 * y);
        res.x = (m[1][0] + m[0][1]) * d;
        res.y = y;
        res.z = (m[2][1] + m[1][2]) * d;
        res.w = (m[0][2] - m[2][0]) * d;
    }
    else if (select == Select::PZ)
    {
        float z = std::sqrtf(pz) * 0.5f;
        float d = 1.0f / (4 * z);
        res.x = (m[0][2] + m[2][0]) * d;
        res.y = (m[2][1] + m[1][2]) * d;
        res.z = z;
        res.w = (m[1][0] - m[0][1]) * d;
    }
    else if (select == Select::PW)
    {
        float w = std::sqrtf(pw) * 0.5f;
        float d = 1.0f / (4 * w);
        res.w = (m[2][1] - m[1][2]) * d;
        res.z = (m[0][2] - m[2][0]) * d;
        res.y = (m[1][0] - m[0][1]) * d;
        res.x = w;
    }

    return res;
}

MATRIX Matrix4x4::GetMATRIX() const
{
    MATRIX result;

    for (int row = 0; row < 4; ++row)
    {
        for (int line = 0; line < 4; ++line)
        {
            result.m[row][line] = m[row][line];
        }
    }

    return result;
}

Effekseer::Matrix43 Matrix4x4::GetEffMat() const
{
    Effekseer::Matrix43 ret;

    for (int y = 0; y < 4; ++y)
    {
        for (int x = 0; x < 3; ++x)
        {
            ret.Value[y][x] = m[y][x];
        }
    }

    return ret;
}

float Matrix4x4::Dot(const Matrix4x4& mat, int line, int row) const
{
    float result = 0.0f;

    for (int i = 0; i < 4; ++i)
    {
        float val = m[row][i] * mat.m[i][line];
        result += val;
    }

    return result;
}

Matrix4x4 Scale(const Vec3& size)
{
    Matrix4x4 result;
    result.Identity();

    result.m[0][0] = size.x;
    result.m[1][1] = size.y;
    result.m[2][2] = size.z;

    return result;
}

Matrix4x4 Pos(const Vec3& pos)
{
    Matrix4x4 mat;
    mat.Identity();

    mat.m[3][0] = pos.x;
    mat.m[3][1] = pos.y;
    mat.m[3][2] = pos.z;

    return mat;
}

Matrix4x4 Rot(const Quaternion& q)
{
    const auto& qMat = q.GetMat();
    Matrix4x4 mat;
    mat.Identity();
    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            mat.m[x][y] = qMat.m[x][y];
        }
    }
    return mat;
}
