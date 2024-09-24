#pragma once
#include <vector>
#include "Geometry/Vec3.h"

struct CameraInfo
{
	// near, far
	float n, f;
	float fov;
	Vec3 cameraPos;
	Vec3 targetPos;
	Vec3 dir;

	int fovFarTime;
	int fovNearTime;
	bool isFovFar;
	bool isFovNear;
	int fovFrame;
	float fovC;
};


class Camera
{
private:
	Camera();

	Camera(const Camera&) = delete;
	void operator= (const Camera&) = delete;

public:
	static Camera& GetInstance();

	void Update(int no);

	const Vec3& NowAppPos() const;

	void SetNearFar(int no, float n, float f);
	void SetFov(int no, float angle, int time);
	void SetInitPos(int no, const Vec3& targetPos, const Vec3& dir);
	void SetPos(int no, const Vec3& targetPos, const Vec3& dir);

	void AppInfo(int no);

	const CameraInfo& GetInfo(int no) const;

#ifdef _DEBUG
	void DebugUpdate(bool isDebug);
#endif

private:
	std::vector<CameraInfo> m_info;
	int m_nowAppNo;
#ifdef _DEBUG
	bool m_isDebug;
	CameraInfo m_debugInfo;
	Vec3 m_dFront;
	Vec3 m_dRright;
#endif
};

