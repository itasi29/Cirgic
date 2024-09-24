#pragma once
#ifdef _DEBUG
#include <list>
#include <unordered_map>
#include <string>
#include "Geometry/Vec3.h"
#include "Geometry/Quaternion.h"

namespace MyEngine
{
	class DebugDraw
	{
	public:
		struct CircleInfo
		{
			Vec3 center = Vec3();
			float radius = 0.0f;
			unsigned int color = 0xff00ff;
		};
		struct LineInfo
		{
			Vec3 start = Vec3();
			Vec3 end = Vec3();
			unsigned int color = 0xff00ff;
		};
		struct SphereInfo
		{
			Vec3 center = Vec3();
			float radius = 0.0f;
			unsigned int color = 0xff00ff;
			bool isFill = false;
		};
		struct CapsuleInfo
		{
			Vec3 center = Vec3();
			Vec3 dir = Vec3();
			float radius = 0.0f;
			float size = 0.0f;
			unsigned int color = 0xff00ff;
			bool isFill = false;
		};
		struct BoxInfo
		{
			Vec3 center = Vec3();
			Vec3 size = Vec3();
			Quaternion rot;
			unsigned int color = 0xff00ff;
			bool isFill = false;
		};
		struct TerrainInfo
		{
			Vec3 center = Vec3();
			Vec2 size = Vec2();
			Vec3 norm = Vec3();
			Quaternion rot;
			unsigned int color = 0xff00ff;
			bool isFill = false;
		};
		struct StrInfo
		{
			std::wstring str;
			unsigned int color = 0xffffff;
		};
	private:
		DebugDraw();
		~DebugDraw();

		DebugDraw(const DebugDraw&) = delete;
		void operator= (const DebugDraw&) = delete;

	public:
		static DebugDraw& GetInstance();

		void Gulid() const;
		void Tree(int level, const Vec3& min, const Vec3& max) const;

		void SetStop(bool isStop) { m_isStop = isStop; }

		void Clear();
		void Draw() const;

		void DrawCircle(const Vec3& center, float radius, unsigned int color);
		void DrawLine(const Vec3& start, const Vec3& end, unsigned int color);
		void DrawSphere(const Vec3& center, float radius, unsigned int color, bool isFill);
		void DrawCapsule(const Vec3& center, const Vec3& dir, float radius, float size, unsigned int color, bool isFill);
		void DrawCube(const Vec3& center, const Vec3& size, const Quaternion& rot, unsigned int color, bool isFill);
		void DrawTerrain(const Vec3& center, const Vec3& norm, const Vec2& size, const Quaternion& rot, unsigned int color, bool isFill);
		void DrawString(int x, int startY, unsigned int color, std::wstring str);
	public:
		static constexpr unsigned int COL_BEFOR  = 0x0000ff;
		static constexpr unsigned int COL_NEXT   = 0xff00ff;
		static constexpr unsigned int COL_AFFTER = 0xff0000;
		static constexpr unsigned int COL_HIT    = 0x00ffff;

	private:
		bool m_isStop;

		std::list<CircleInfo> m_circleInfo;
		std::list<LineInfo> m_lineInfo;
		std::list<SphereInfo> m_sphereInfo;
		std::list<CapsuleInfo> m_capsuleInfo;
		std::list<BoxInfo> m_boxInfo;
		std::list<TerrainInfo> m_terrainInfo;
		std::unordered_map<int, std::unordered_map<int, std::list<StrInfo>>> m_strInfo;
	};
}
#endif