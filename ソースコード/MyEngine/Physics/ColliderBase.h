#pragma once
#include <unordered_map>
#include "Geometry/Vec3.h"

namespace MyEngine
{
	class ColliderBase abstract
	{
	public:
		// 当たり判定種別
		enum class Kind
		{
			Sphere,
			Capsule,
			Terrain,
			Box,
			Line,
			Wall,
		};

		// コンストラクタ
		ColliderBase(Kind kind);
		virtual ~ColliderBase() {}

		virtual void SetMinMax(const Vec3& center) abstract;

		// 当たり判定種別取得
		Kind GetKind() const { return m_kind; }

	public:
		// MEMO: 色々なところで使うためColliderBase関係のpublic変数は
		// メンバ変数のコーディング規約無視している
		bool isTrigger;
		Vec3 center;
		Vec3 min;
		Vec3 max;

	private:
		Kind m_kind;
	};
}
