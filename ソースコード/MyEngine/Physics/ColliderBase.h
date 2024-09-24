#pragma once
#include <unordered_map>
#include "Geometry/Vec3.h"

namespace MyEngine
{
	class ColliderBase abstract
	{
	public:
		// �����蔻����
		enum class Kind
		{
			Sphere,
			Capsule,
			Terrain,
			Box,
			Line,
			Wall,
		};

		// �R���X�g���N�^
		ColliderBase(Kind kind);
		virtual ~ColliderBase() {}

		virtual void SetMinMax(const Vec3& center) abstract;

		// �����蔻���ʎ擾
		Kind GetKind() const { return m_kind; }

	public:
		// MEMO: �F�X�ȂƂ���Ŏg������ColliderBase�֌W��public�ϐ���
		// �����o�ϐ��̃R�[�f�B���O�K�񖳎����Ă���
		bool isTrigger;
		Vec3 center;
		Vec3 min;
		Vec3 max;

	private:
		Kind m_kind;
	};
}
