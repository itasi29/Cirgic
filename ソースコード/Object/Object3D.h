#pragma once
#include <memory>
#include "MyEngine/Physics/Collidable.h"
#include "Geometry/Vec3.h"
#include "Geometry/Matrix4x4.h"
#include "Geometry/Quaternion.h"

class FileBase;

class Object3D : public MyEngine::Collidable
{
public:
	Object3D(Collidable::Priority priority, ObjectTag tag);
	Object3D(Collidable::Priority priority, ObjectTag tag, const wchar_t* const path);
	virtual ~Object3D();

	void Entry();
	void Init(const Vec3& pos, const Quaternion& rot, const Vec3& size);
	virtual void End();

	virtual void Update() {}
	virtual void Draw() const;

	// 衝突したとき
	virtual void OnCollideEnter(Collidable* colider) override {}
	virtual void OnCollideStay(Collidable* colider) override {}
	virtual void OnCollideExit(Collidable* colider) override {}
	virtual void OnTriggerEnter(Collidable* colider) override {}
	virtual void OnTriggerStay(Collidable* colider) override {}
	virtual void OnTriggerExit(Collidable* colider) override {}

	const Quaternion& GetRot() const { return m_rotation; }
	const Vec3& GetFront() const { return m_front; }
	const Vec3& GetRight() const { return m_right; }
	const Vec3& GetTop() const { return m_top; }

protected:
	// モデルハンドル
	int m_modelH;
	std::shared_ptr<FileBase> m_model;
	
	// 回転
	Quaternion m_rotation;
	// 拡大
	Vec3 m_scale;

	// 方向
	Vec3 m_front;
	Vec3 m_right;
	Vec3 m_top;
	// カメラ方向
	Vec3 m_cameraFront;

private:
	int m_meshNum;
	bool m_isEntry;
};

