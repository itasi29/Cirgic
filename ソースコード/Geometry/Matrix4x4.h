#pragma once
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Vec3.h"

struct Quaternion;

/// <summary>
/// 4x4�s��
/// </summary>
struct Matrix4x4
{
public:
	float m[4][4];
	// �s->��

	Matrix4x4();

	/* ���Z */
	Matrix4x4 operator+ (const Matrix4x4& mat) const;
	Matrix4x4 operator- (const Matrix4x4& mat) const;
	Matrix4x4 operator* (const Matrix4x4& mat) const;
	Matrix4x4 operator* (const Quaternion& q) const;
	Matrix4x4 operator/ (float div) const;

	// �[��
	void Zero();
	// �P�ʍs��
	void Identity();
	// �]�u�s��
	Matrix4x4 Transpose();
	// �t�s��
	Matrix4x4 Inverse(bool isCorrect = false);

	// �|�W�V�����̐ݒ�
	Vec3 GetPos() const;
	Quaternion GetRot() const;

	// DxLib�ϊ��p
	MATRIX GetMATRIX() const;
	Effekseer::Matrix43 GetEffMat() const;

private:
	float Dot(const Matrix4x4& mat, int line, int row) const;
};

Matrix4x4 Scale(const Vec3& size);
Matrix4x4 Pos(const Vec3& pos);
Matrix4x4 Rot(const Quaternion& q);
