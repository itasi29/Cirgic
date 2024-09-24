#ifdef _DEBUG
#include "DebugDraw.h"
#include <DxLib.h>
#include "Common/Game.h"
#include "Common/Camera.h"

namespace
{
    // ƒOƒŠƒbƒh
    constexpr int GULID_NUM = 500;
    constexpr int GULID_SIZE = 10000;
    constexpr int GULID_SPACE = GULID_SIZE * 2 / GULID_NUM;
}
using namespace MyEngine;

DebugDraw::DebugDraw() :
    m_isStop(false)
{
}

DebugDraw::~DebugDraw()
{
}

DebugDraw& DebugDraw::GetInstance()
{
    static DebugDraw instance;
    return instance;
}

void DebugDraw::Gulid() const
{
    for (int x = -GULID_SIZE; x <= GULID_SIZE; x += GULID_SPACE)
    {
        DrawLine3D(VGet(static_cast<float>(x), 0, -GULID_SIZE), VGet(static_cast<float>(x), 0, GULID_SIZE), 0xffff00);
    }
    for (int z = -GULID_SIZE; z <= GULID_SIZE; z += GULID_SPACE)
    {
        DrawLine3D(VGet(-GULID_SIZE, 0, static_cast<float>(z)), VGet(GULID_SIZE, 0, static_cast<float>(z)), 0xff0000);
    }

    // X+-,Z+-‚Ì•ûŒü‚ª•ª‚©‚è‚â‚·‚¢‚æ‚¤‚É•\Ž¦‚ð’Ç‰Á‚·‚é
    VECTOR dispPos = ConvWorldPosToScreenPos(VGet(200, 0, 0));
    if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
    {
        DrawStringF(dispPos.x, dispPos.y, L"X+", 0xffffff);
    }

    dispPos = ConvWorldPosToScreenPos(VGet(-200, 0, 0));
    if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
    {
        DrawStringF(dispPos.x, dispPos.y, L"X-", 0xffffff);
    }

    dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 200));
    if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
    {
        DrawStringF(dispPos.x, dispPos.y, L"Z+", 0xffffff);
    }

    dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -200));
    if (dispPos.z >= 0.0f && dispPos.z <= 1.0f)
    {
        DrawStringF(dispPos.x, dispPos.y, L"Z-", 0xffffff);
    }

}

void MyEngine::DebugDraw::Tree(int level, const Vec3& min, const Vec3& max) const
{
    auto vec = max - min;
    vec = vec / static_cast<float>(level);

    Vec3 start = min;
    Vec3 end = min;
    end.x += vec.x;
    for (int i = 0; i < level; ++i)
    {
        DrawCube3D(start.VGet(), end.VGet(), 0xff0000, 0, false);
    }
}

void DebugDraw::Clear()
{
    m_circleInfo.clear();
    m_lineInfo.clear();
    m_sphereInfo.clear();
    m_capsuleInfo.clear();
    m_boxInfo.clear();
    m_terrainInfo.clear();
    m_strInfo.clear();
}

void DebugDraw::Draw() const
{
    auto& camera = Camera::GetInstance();
    const auto& cInfo = camera.GetInfo(0);

    // ‰~î•ñ•`‰æ
    for (const auto& info : m_circleInfo)
    {
        auto dir = info.center - cInfo.cameraPos;
        if (!IsUniformDirection(dir, cInfo.dir)) continue;

        int x = static_cast<int>(info.center.x);
        int y = static_cast<int>(info.center.y);
        int radius = static_cast<int>(info.radius);
        DxLib::DrawCircle(x, y, radius, info.color);
    }
    // üî•ñ•`‰æ
    for (const auto& info : m_lineInfo)
    {


        DxLib::DrawLine3D(info.start.VGet(), info.end.VGet(), info.color);
    }
    SetUseLighting(false);
    // ‹…•`‰æ
    for (const auto& info : m_sphereInfo)
    {
        auto dir = info.center - cInfo.cameraPos;
        if (!IsUniformDirection(dir, cInfo.dir)) continue;

        DxLib::DrawSphere3D(info.center.VGet(), info.radius, 8, info.color, 0, info.isFill);
    }
    // ƒJƒvƒZƒ‹•`‰æa
    for (const auto& info : m_capsuleInfo)
    {
        auto dir = info.center - cInfo.cameraPos;
        if (!IsUniformDirection(dir, cInfo.dir)) continue;

        auto halfSize = info.size * 0.5f;
        auto start = info.center + info.dir * halfSize;
        auto end = info.center - info.dir * halfSize;
        DxLib::DrawCapsule3D(start.VGet(), end.VGet(), info.radius, 8, info.color, 0, info.isFill);
    }
    // ƒ{ƒbƒNƒX•`‰æ
    for (const auto& info : m_boxInfo)
    {
        auto dir = info.center - cInfo.cameraPos;
        if (!IsUniformDirection(dir, cInfo.dir)) continue;

        auto rot = info.rot * info.size * 0.5f;
        auto min = info.center - rot;
        auto max = info.center + rot;
        DxLib::DrawCube3D(min.VGet(), max.VGet(), info.color, 0, info.isFill);
    }
    // ’n–Ê‚ð•`‰æ
    for (const auto& info : m_terrainInfo)
    {
        if (info.isFill || false)
        {
            VERTEX3D v[4];
            WORD index[6];

            int r = (0xff0000 & info.color) >> 16;
            int g = (0x00ff00 & info.color) >> 8;
            int b = (0x0000ff & info.color);
            for (auto& t : v)
            {
                t.norm = info.norm.VGet();
                t.dif = GetColorU8(r, g, b, 255);
                t.spc = GetColorU8(0, 0, 0, 0);
                t.u = 0.0f;
                t.v = 0.0f;
                t.su = 0.0f;
                t.sv = 0.0f;
            }

            auto rp = info.rot * Vec3::Right() * info.size.x * 0.5f;
            auto fp = info.rot * Vec3::Front() * info.size.y * 0.5f;

            auto rf = info.center + rp + fp;
            auto lf = info.center - rp + fp;
            auto rb = info.center + rp - fp;
            auto lb = info.center - rp - fp;

            v[0].pos = lb.VGet();
            v[1].pos = rb.VGet();
            v[2].pos = lf.VGet();
            v[3].pos = rf.VGet();

            index[0] = 0;
            index[1] = 1;
            index[2] = 2;
            index[3] = 3;
            index[4] = 2;
            index[5] = 1;

            DxLib::DrawPolygonIndexed3D(v, 4, index, 2, DX_NONE_GRAPH, false);
        }
        else
        {
            auto rp = info.rot * Vec3::Right() * info.size.x * 0.5;
            auto fp = info.rot * Vec3::Front() * info.size.y * 0.5;

            auto rf = info.center + rp + fp;
            auto lf = info.center - rp + fp;
            auto rb = info.center + rp - fp;
            auto lb = info.center - rp - fp;

            int size = 100;
            auto shift = rp / (size * 0.5f);
            auto s = rb;
            auto e = rf;
            for (int i = 0; i < size; ++i)
            {
                DrawLine3D(s.VGet(), e.VGet(), info.color);
                s -= shift;
                e -= shift;
            }
            DrawLine3D(lb.VGet(), lf.VGet(), info.color);

            shift = fp / (size * 0.5f);
            s = rb;
            e = lb;
            for (int i = 0; i < size; ++i)
            {
                DrawLine3D(s.VGet(), e.VGet(), info.color);
                s += shift;
                e += shift;
            }
            DrawLine3D(rf.VGet(), lf.VGet(), info.color);
        }
    }
    SetUseLighting(true);
    // •¶Žš•`‰æ
    for (const auto& info : m_strInfo)
    {
        for (const auto& info2 : info.second)
        {
            int y = info.first;
            int x = info2.first;
            for (const auto& data : info2.second)
            {
                DxLib::DrawString(x, y, data.str.c_str(), data.color);
                y += 20;
            }
        }
    }
}

void MyEngine::DebugDraw::DrawCircle(const Vec3& center, float radius, unsigned int color)
{
    if (m_isStop) return;

    m_circleInfo.emplace_back(CircleInfo{ center, radius, color });
}

void MyEngine::DebugDraw::DrawLine(const Vec3& start, const Vec3& end, unsigned int color)
{
    if (m_isStop) return;

    m_lineInfo.emplace_back(LineInfo{ start, end, color });
}

void MyEngine::DebugDraw::DrawSphere(const Vec3& center, float radius, unsigned int color, bool isFill)
{
    if (m_isStop) return;

    m_sphereInfo.emplace_back(SphereInfo{ center, radius, color, isFill});
}

void MyEngine::DebugDraw::DrawCapsule(const Vec3& center, const Vec3& dir, float radius, float size, unsigned int color, bool isFill)
{

    if (m_isStop) return;

    m_capsuleInfo.emplace_back(CapsuleInfo{ center, dir, radius, size, color, isFill });
}

void MyEngine::DebugDraw::DrawCube(const Vec3& center, const Vec3& size, const Quaternion& rot, unsigned int color, bool isFill)
{
    if (m_isStop) return;

    m_boxInfo.emplace_back(BoxInfo{ center, size, rot, color, isFill });
}

void MyEngine::DebugDraw::DrawTerrain(const Vec3& center, const Vec3& norm, const Vec2& size, const Quaternion& rot, unsigned int color, bool isFill)
{
    if (m_isStop) return;

    m_terrainInfo.emplace_back(TerrainInfo{ center, size, norm, rot, color, isFill });
}

void MyEngine::DebugDraw::DrawString(int x, int y, unsigned int color, std::wstring str)
{
    if (m_isStop) return;

    m_strInfo[y][x].emplace_back(StrInfo{str, color});
}
#endif