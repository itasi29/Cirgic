#include "Rigidbody.h"
#include <cmath>
#include <algorithm>
#include "Common/Game.h"

using namespace MyEngine;

Rigidbody::Rigidbody()
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Init(const Vec3& pos)
{
	m_pos = pos;
	m_nextPos = m_pos;
	m_gravity = Vec3();
	m_velocity = Vec3();
	m_dir = Vec3(0, 0, 1);
}

void Rigidbody::SetVelocity(const Vec3& velocity)
{
	m_velocity = velocity;
	// �ړ����Ă���ꍇ�̂ݕ�����ύX
	if (velocity.SqLength())
	{
		m_dir = velocity.GetNormalized();
	}
}

void Rigidbody::AddForce(const Vec3& dir, float power, bool isLimit, float limit, bool isMaxUp)
{
	auto newVel = m_velocity + dir * power;

	// ������������ꍇ
	if (isLimit)
	{
		// ���x�������͂̕����Ɠ����ꍇ
		bool isSameDir = IsUniformDirection(newVel, dir);
		// �����𒴂��Ă����ꍇ
		bool isOverVel = newVel.SqLength() > limit;

		if (isSameDir && isOverVel)
		{
			if (isMaxUp)
			{
				m_velocity = newVel.GetNormalized() * std::sqrt(limit);
			}
			return;
		}
	}

	m_velocity = newVel;
}

void Rigidbody::AddGravity(float force)
{
	auto newG = m_velocity.y + m_gravity.y - Game::GRAVITY;
	newG = std::max<float>(newG, -Game::GRAVITY_MAX_SPEED);
	m_gravity.y = newG - m_velocity.y;
}
