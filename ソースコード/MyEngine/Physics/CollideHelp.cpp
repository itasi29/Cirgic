#include "CollideHelp.h"
#include <cassert>
#include "Geometry/MathHelp.h"
#include "Rigidbody.h"
#include "Colliders.h"


bool IsSphereCheckNormAndSurface(const Vec3& point, float radius, const Vec3& center, const Vec3& norm, const Quaternion& rot, const Vec2& size);
Vec3 FixSurfaceToSphere(const Vec3& surfaceP, const Vec3& norm, const Vec3& sphereP, float radius, const Vec3& sphereShift);

using namespace MyEngine;

bool MyEngine::IsCollideSphereAndSphere(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB)
{
    auto sphereColA = dynamic_cast<ColliderSphere*>(colliderA);
    auto sphereColB = dynamic_cast<ColliderSphere*>(colliderB);

    auto sphereAPos = rigidA.GetNextPos() + sphereColA->center;
    auto sphereBPos = rigidB.GetNextPos() + sphereColB->center;

    return IsCollideSphereAndSphere(sphereAPos, sphereBPos, sphereColA->radius, sphereColB->radius);
}

bool MyEngine::IsCollideSphereAndCapsule(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB)
{
    ColliderSphere* sphereCol;
    ColliderCapsule* capsuleCol;
    Rigidbody sphereRigid;
    Rigidbody capsuleRigid;
    if (colliderA->GetKind() == ColliderBase::Kind::Sphere)
    {
        sphereCol = dynamic_cast<ColliderSphere*>(colliderA);
        capsuleCol = dynamic_cast<ColliderCapsule*>(colliderB);
        sphereRigid = rigidA;
        capsuleRigid = rigidB;
    }
    else
    {
        sphereCol = dynamic_cast<ColliderSphere*>(colliderB);
        capsuleCol = dynamic_cast<ColliderCapsule*>(colliderA);
        sphereRigid = rigidB;
        capsuleRigid = rigidA;
    }

    auto spherePos = sphereRigid.GetNextPos() + sphereCol->center;
    auto capsulePos = capsuleRigid.GetNextPos() + capsuleCol->center;

    auto halfDir = capsuleCol->dir * capsuleCol->size * 0.5f;

    return IsCollideSphereAndCapsule(spherePos, capsulePos, halfDir, capsuleCol->radius, sphereCol->radius);
}

bool MyEngine::IsCollideSphereAndTerrain(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB)
{
    ColliderSphere* sphereCol;
    ColliderTerrain* terrainCol;
    Rigidbody sphereRigid;
    Rigidbody terrainRigid;
    if (colliderA->GetKind() == ColliderBase::Kind::Sphere)
    {
        sphereCol = dynamic_cast<ColliderSphere*>(colliderA);
        terrainCol = dynamic_cast<ColliderTerrain*>(colliderB);
        sphereRigid = rigidA;
        terrainRigid = rigidB;
    }
    else
    {
        sphereCol = dynamic_cast<ColliderSphere*>(colliderB);
        terrainCol = dynamic_cast<ColliderTerrain*>(colliderA);
        sphereRigid = rigidB;
        terrainRigid = rigidA;
    }

    auto spherePos  = sphereRigid.GetNextPos() + sphereCol->center;
    auto terrainPos = terrainRigid.GetNextPos() + terrainCol->center;

    return IsSphereHitSurface(spherePos, sphereCol->radius, terrainPos, terrainCol->norm, terrainCol->rotation, terrainCol->size);
}

bool MyEngine::IsCollideSphereAndBox(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB)
{
    ColliderSphere* sphereCol;
    ColliderBox* boxCol;
    Rigidbody sphereRigid;
    Rigidbody boxRigid;
    if (colliderA->GetKind() == ColliderBase::Kind::Sphere)
    {
        sphereCol = dynamic_cast<ColliderSphere*>(colliderA);
        boxCol = dynamic_cast<ColliderBox*>(colliderB);
        sphereRigid = rigidA;
        boxRigid = rigidB;
    }
    else
    {
        sphereCol = dynamic_cast<ColliderSphere*>(colliderB);
        boxCol = dynamic_cast<ColliderBox*>(colliderA);
        sphereRigid = rigidB;
        boxRigid = rigidA;
    }

    auto spherePos = sphereRigid.GetNextPos() + sphereCol->center;
    auto boxPos = boxRigid.GetNextPos() + boxCol->center;

    // �{�b�N�X�̒��S����~�̒��S�܂ł̃x�N�g�����쐬
    auto boxToSphere = spherePos - boxPos;

    // ���ꂼ��̕������m�F���Ă��̒����ȓ��Ȃ瓖�����Ă���
    auto right = boxCol->rotation * Vec3::Right();
    auto up = boxCol->rotation * Vec3::Up();
    auto front = boxCol->rotation * Vec3::Front();
    auto radius = sphereCol->radius;
    auto size = boxCol->size * 0.5f;
    bool isRight = Projection(boxToSphere, right).SqLength() < (size.x + radius) * (size.x + radius);
    bool isUp    = Projection(boxToSphere, up   ).SqLength() < (size.y + radius) * (size.y + radius);
    bool isFront = Projection(boxToSphere, front).SqLength() < (size.z + radius) * (size.z + radius);

    return isRight && isUp && isFront;
}

bool MyEngine::IsCollideCapsuleAndCapsule(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB)
{
    auto capsuleColA = dynamic_cast<ColliderCapsule*>(colliderA);
    auto capsuleColB = dynamic_cast<ColliderCapsule*>(colliderB);

    auto capsuleAPos = rigidA.GetNextPos() + capsuleColA->center;
    auto capsuleBPos = rigidB.GetNextPos() + capsuleColB->center;

    // ���g�̌����Ă�������ɐL�тĂ���x�N�g�����쐬
    Vec3 dirA = capsuleColA->dir.GetNormalized() * capsuleColA->size * 0.5f;
    // �Ώۂ̌����Ă�������ɐL�тĂ���x�N�g�����쐬
    Vec3 dirB = capsuleColB->dir.GetNormalized() * capsuleColB->size * 0.5f;
    // ���ʕێ�
    Vec3 minPosA, minPosB;
    GetNearestPtOnLine(capsuleAPos, dirA, capsuleBPos, dirB, minPosA, minPosB);

    // �傫��(2��)
    float sqLen = (minPosA - minPosB).SqLength();
    // ���ꂼ��̔��a�̍��v��2��
    float ar = capsuleColA->radius + capsuleColB->radius;
    // �傫�������a�ȉ���
    return sqLen < ar* ar;
}

bool MyEngine::IsCollideCapsuleAndTerrain(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB)
{
    ColliderCapsule* capsuleCol;
    ColliderTerrain* terrainCol;
    Rigidbody capusleRigid;
    Rigidbody terrainRigid;
    if (colliderA->GetKind() == ColliderBase::Kind::Capsule)
    {
        capsuleCol = dynamic_cast<ColliderCapsule*>(colliderA);
        terrainCol = dynamic_cast<ColliderTerrain*>(colliderB);
        capusleRigid = rigidA;
        terrainRigid = rigidB;
    }
    else
    {
        capsuleCol = dynamic_cast<ColliderCapsule*>(colliderB);
        terrainCol = dynamic_cast<ColliderTerrain*>(colliderA);
        capusleRigid = rigidB;
        terrainRigid = rigidA;
    }

    auto capsulePos = capusleRigid.GetNextPos() + capsuleCol->center;
    auto terrainPos = terrainRigid.GetNextPos() + terrainCol->center;

    auto halfSize = capsuleCol->size * 0.5f;
    auto capsuleStart = capsulePos + capsuleCol->dir * halfSize;
    auto capsuleEnd = capsulePos - capsuleCol->dir * halfSize;

    auto centerToStart = capsuleStart - terrainPos;
    auto centerToEnd = capsuleEnd - terrainPos;

    if (Dot(centerToEnd, terrainCol->norm) * Dot(centerToStart, terrainCol->norm) <= 0)
    {
        // ���̃X�^�[�g�E�S�[���Ɩʂ܂ł̒������擾
        auto startSurfaceLen = Projection(centerToStart, terrainCol->norm).Length();
        auto endSurfaceLen = Projection(centerToEnd, terrainCol->norm).Length();
        // ��_��������̉��������肩���v�Z
        float rate = startSurfaceLen / (startSurfaceLen + endSurfaceLen);

        // ��_���擾
        auto crossPt = terrainPos + centerToStart * (1 - rate) + centerToEnd * rate;

        if (IsSphereHitSurface(crossPt, capsuleCol->radius, terrainPos, terrainCol->norm, terrainCol->rotation, terrainCol->size))
        {
            if (rate < 0.5f)
            {
                capsuleCol->SetFixS(true);
                capsuleCol->SetFixE(false);
            }
            else
            {
                capsuleCol->SetFixS(false);
                capsuleCol->SetFixE(true);
            }
            return true;
        }
        else
        {
            return false;
        }
        
        return false;
    }
    else
    {
        bool isFixS = IsSphereHitSurface(capsuleStart, capsuleCol->radius, terrainPos, terrainCol->norm, terrainCol->rotation, terrainCol->size);
        bool isFixE = IsSphereHitSurface(capsuleEnd, capsuleCol->radius, terrainPos, terrainCol->norm, terrainCol->rotation, terrainCol->size);
        capsuleCol->SetFixS(isFixS);
        capsuleCol->SetFixE(isFixE);
        return isFixS || isFixE;
    }
}

bool MyEngine::IsCollideCapsuleAndBox(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB)
{
    ColliderCapsule* capsuleCol;
    ColliderBox* boxCol;
    Rigidbody capsuleRigid;
    Rigidbody boxRigid;
    if (colliderA->GetKind() == ColliderBase::Kind::Capsule)
    {
        capsuleCol = dynamic_cast<ColliderCapsule*>(colliderA);
        boxCol = dynamic_cast<ColliderBox*>(colliderB);
        capsuleRigid = rigidA;
        boxRigid = rigidB;
    }
    else
    {
        capsuleCol = dynamic_cast<ColliderCapsule*>(colliderB);
        boxCol = dynamic_cast<ColliderBox*>(colliderA);
        capsuleRigid = rigidB;
        boxRigid = rigidA;
    }

    // HACK: ������Ǝ����Ɏ��Ԃ����肻���������΂��܂�
    assert(false && "�������̔���ł�");

    return false;
}

bool MyEngine::IsCollideSurfaceAndSurface(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB)
{
    return false;
}

Vec3 MyEngine::FixSphereToSphere(const Rigidbody& primaryRigid, const Rigidbody& secondaryRigid, ColliderBase* primaryCollider, ColliderBase* secondaryCollider)
{
    auto primarySphere = dynamic_cast<ColliderSphere*>(primaryCollider);
    auto secondarySphere = dynamic_cast<ColliderSphere*>(secondaryCollider);

    auto primaryPos = primaryRigid.GetNextPos() + primaryCollider->center;
    auto secondaryPos = secondaryRigid.GetNextPos() + secondaryCollider->center;

    // primary����secondary�ւ̃x�N�g�����쐬
    auto primaryToSecondary = secondaryPos - primaryPos;
    // ���̂܂܂��Ƃ��傤�Ǔ�����ʒu�ɂȂ�̂ŏ����]���ɗ���
    float  awayDist = primarySphere->radius + secondarySphere->radius + 0.0001f;
    // �����𒲐�
    primaryToSecondary = primaryToSecondary.GetNormalized() * awayDist;

    // primary����x�N�g�������ɉ���
    return primaryPos + primaryToSecondary - secondaryCollider->center;
}

Vec3 MyEngine::FixCapsuleToCapsule(const Rigidbody& primaryRigid, const Rigidbody& secondaryRigid, ColliderBase* primaryCollider, ColliderBase* secondaryCollider)
{
    auto capsulePrimary = dynamic_cast<ColliderCapsule*>(primaryCollider);
    auto capsuleSecondary = dynamic_cast<ColliderCapsule*>(secondaryCollider);

    auto capsulePosPrimary = primaryRigid.GetNextPos() + primaryCollider->center;
    auto capsulePosSecondary = secondaryRigid.GetNextPos() + secondaryCollider->center;

    // ���g�̌����Ă�������ɐL�тĂ���x�N�g�����쐬
    Vec3 dirPrimary = capsulePrimary->dir.GetNormalized() * capsulePrimary->size * 0.5f;
    // �Ώۂ̌����Ă�������ɐL�тĂ���x�N�g�����쐬
    Vec3 dirSecondary = capsuleSecondary->dir.GetNormalized() * capsuleSecondary->size * 0.5f;
    // ���ʕێ�
    Vec3 minPosPrimary, minPosSecondary;
    GetNearestPtOnLine(capsulePosPrimary, dirPrimary, capsulePosSecondary, dirSecondary, minPosPrimary, minPosSecondary);

    // Primary�̍ŋߐړ_����Secondary�̍ŋߐړ_�܂ł̃x�N�g��
    auto pToS = minPosSecondary - minPosPrimary;
    // �������擾
    auto len = pToS.Length();
    // ���ꂼ��̔��a�{�ɏ��l���璷�����������Ԃ񂾂����炷
    float awayDis = capsulePrimary->radius + capsuleSecondary->radius + 0.0001f - len;

    return capsulePosSecondary - secondaryCollider->center + pToS.GetNormalized() * awayDis;
}

Vec3 MyEngine::FixTerrainToSphere(const Rigidbody& primaryRigid, const Rigidbody& secondaryRigid, ColliderBase* primaryCollider, ColliderBase* secondaryCollider)
{
    auto terrain = dynamic_cast<ColliderTerrain*>(primaryCollider);
    auto sphere = dynamic_cast<ColliderSphere*>(secondaryCollider);

    auto terrainPos = primaryRigid.GetNextPos() + primaryCollider->center;
    auto spherePos = secondaryRigid.GetNextPos() + secondaryCollider->center;

    return FixSurfaceToSphere(terrainPos, terrain->norm, spherePos, sphere->radius, secondaryCollider->center);
}

Vec3 MyEngine::FixTerrainToCapsule(const Rigidbody& primaryRigid, const Rigidbody& secondaryRigid, ColliderBase* primaryCollider, ColliderBase* secondaryCollider)
{
    auto terrain = dynamic_cast<ColliderTerrain*>(primaryCollider);
    auto capsule = dynamic_cast<ColliderCapsule*>(secondaryCollider);

    auto terrainPos = primaryRigid.GetNextPos() + primaryCollider->center;
    auto capsulePos = secondaryRigid.GetNextPos() + secondaryCollider->center;

    auto halfSize = capsule->size * 0.5f;
    auto halfSizeDir = capsule->dir * halfSize;
    auto capsuleStart = capsulePos + halfSizeDir;
    auto capsuleEnd = capsulePos - halfSizeDir;

    bool isFixS = capsule->IsFixS();
    bool isFixE = capsule->IsFixE();

    if (isFixS)
    {
        capsuleStart = FixSurfaceToSphere(terrainPos, terrain->norm, capsuleStart, capsule->radius, secondaryCollider->center);
    }
    else
    {
        capsuleStart -= secondaryCollider->center;
    }
    if (isFixE)
    {
        capsuleEnd = FixSurfaceToSphere(terrainPos, terrain->norm, capsuleEnd, capsule->radius, secondaryCollider->center);
    }
    else
    {
        capsuleEnd -= secondaryCollider->center;
    }


    auto preToNext = secondaryRigid.GetNextPos() - secondaryRigid.GetPos();
    auto dir = (capsuleStart - capsuleEnd).GetNormalized();
    dir *= halfSize;

    Vec3 res;
    if (IsUniformDirection(terrain->norm, preToNext))
    {
        res = capsuleEnd + dir;
    }
    else
    {
        res = capsuleStart - dir;
    }
    // �@�������ɏ������炷
    res += terrain->norm * 0.001f;

    
    return res;
}

bool IsSphereCheckNormAndSurface(const Vec3& point, float radius, const Vec3& center, const Vec3& norm, const Quaternion& rot, const Vec2& size)
{
    auto pt = GetCrossPtSurfaceAndPt(point, center, norm);
    auto v = pt - center;
    // ������x��������
    if (IsUniformDirection(norm, v))
    {
        return IsSphereHitSurface(point, radius, center, norm, rot, size);
    }
    else
    {
        return true;
    }
}

Vec3 FixSurfaceToSphere(const Vec3& surfaceP, const Vec3& norm, const Vec3& sphereP, float radius, const Vec3& sphereShift)
{
    // ���̒��S�̖ʂł̈ʒu���擾
    auto crossPt = GetCrossPtSurfaceAndPt(sphereP, surfaceP, norm);
    crossPt -= sphereShift;

    // �n�ʂ̖@�������ɏ����]���ɂ��炷
    float awayDis = radius + 0.001f;
    auto normDisVec = norm.GetNormalized() * awayDis;
    return crossPt + normDisVec;
}