#include "Physics.h"
#include <cassert>
#include "Collidable.h"
#include "CollideHelp.h"
#include "Colliders.h"
#include "CLiner8TreeManager.h"
#include "ColliderTreeObject.h"
#include "MyEngine/DebugDraw.h"
#include "Common/Game.h"

using namespace MyEngine;

namespace
{
    // ”»’èÅ‘å‰ñ”
    constexpr int CHECK_COUNT_MAX = 1000;
}

Physics::Physics()
{
}

Physics& Physics::GetInstance()
{
    static Physics instance;
    return instance;
}

void MyEngine::Physics::Init(unsigned int level, const Vec3& min, const Vec3& max)
{
    m_l8Tree = std::make_shared<CLiner8TreeManager>();
    m_l8Tree->Init(level, min, max);
}

void Physics::Entry(std::shared_ptr<Collidable> collidable)
{
    bool isFound = std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end();
    // –¢“o˜^‚È‚ç’Ç‰Á
    if (!isFound)
    {
        m_collidables.emplace_back(collidable);
        auto add = new ColliderTreeObject();
        add->m_object = collidable.get();
        m_oftArr.push_back(add);
    }
    // “o˜^Ï‚İ‚È‚ç–³‹
    else
    {
        assert(false);
    }
}

void Physics::Exit(std::shared_ptr<Collidable> collidable)
{
    bool isFound = std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end();
    // “o˜^Ï‚İ‚È‚çíœ
    if (isFound)
    {
        m_collidables.remove(collidable);
        m_oftArr.remove_if(
            [&collidable](auto& oft)
            {
                bool isDelete = oft->m_object == collidable.get();
                if (isDelete) delete oft;
                return isDelete;
            });
        m_newCollideInfo.remove_if(
            [&collidable](auto& info)
            {
                return info.t1 == collidable.get() || info.t2 == collidable.get();
            });
        m_newTirrigerInfo.remove_if(
            [&collidable](auto& info)
            {
                return info.t1 == collidable.get() || info.t2 == collidable.get();
            });
    }
    // –¢“o˜^‚È‚ç–³‹
    else
    {
        assert(false);
    }
}

void Physics::Update()
{
    m_onCollideInfo.clear();
    m_preCollideInfo = m_newCollideInfo;
    m_newCollideInfo.clear();
    m_preTirrigerInfo = m_newTirrigerInfo;
    m_newTirrigerInfo.clear();

    MoveNextPos();

    CheckCollide();

    CheckSendOnCollideInfo(m_preCollideInfo, m_newCollideInfo, false);
    CheckSendOnCollideInfo(m_preTirrigerInfo, m_newTirrigerInfo, true);

    FixPos();

    for (const auto& info : m_onCollideInfo)
    {
        OnCollideInfo(info.own, info.send, info.kind); 
    }
}

void MyEngine::Physics::SetTreeMinMax(const Vec3& min, const Vec3& max)
{
    m_l8Tree->RegistRegion(min, max);
}

/// <summary>
/// •¨—‚©‚ç‚ÌˆÚ“®‚ğ–¢—ˆ‚ÌÀ•W‚É“K—p
/// </summary>
void MyEngine::Physics::MoveNextPos() const
{
    int k = 0;
    for (const auto& item : m_collidables)
    {
        auto& rigid = item->m_rigid;

        auto pos = rigid.GetPos();
        auto nextPos = pos + rigid.GetVelocity();

        if (rigid.IsGravity())
        {
            rigid.AddGravity(-Game::GRAVITY);
            nextPos += rigid.GetGravity();
        }

        rigid.SetNextPos(nextPos);
        auto& col = item->m_collider;
        col->SetMinMax(nextPos);

#ifdef _DEBUG
        auto& debug = DebugDraw::GetInstance();

        auto kind = item->m_collider->GetKind();
        if (kind == ColliderBase::Kind::Sphere)
        {
            auto data = dynamic_cast<ColliderSphere*>(item->m_collider.get());
            debug.DrawSphere(pos + data->center, data->radius, DebugDraw::COL_BEFOR, false);
            debug.DrawSphere(nextPos + data->center, data->radius, DebugDraw::COL_NEXT, false);
        }
        else if (kind == ColliderBase::Kind::Capsule)
        {
            auto data = dynamic_cast<ColliderCapsule*>(item->m_collider.get());
            debug.DrawCapsule(pos + data->center, data->dir, data->radius, data->size, DebugDraw::COL_BEFOR, false);
            debug.DrawCapsule(nextPos + data->center, data->dir, data->radius, data->size, DebugDraw::COL_NEXT, false);
        }
#if false
        for (const auto& collider : item->m_colliders)
        {
            auto kind = collider->GetKind();
            if (kind == ColliderBase::Kind::Sphere)
            {
                auto data = dynamic_cast<ColliderSphere*>(collider.get());
                debug.DrawSphere(pos + data->center, data->radius, DebugDraw::COL_BEFOR, false);
                debug.DrawSphere(nextPos + data->center, data->radius, DebugDraw::COL_NEXT, false);
            }
            else if (kind == ColliderBase::Kind::Capsule)
            {
                auto data = dynamic_cast<ColliderCapsule*>(collider.get());
                debug.DrawCapsule(pos + data->center, data->dir, data->radius, data->size, DebugDraw::COL_BEFOR, false);
                debug.DrawCapsule(nextPos + data->center, data->dir, data->radius, data->size, DebugDraw::COL_NEXT, false);
            }
            //else if (kind == ColliderBase::Kind::Terrain)
            //{
            //    auto data = dynamic_cast<ColliderTerrain*>(collider.get());
            //    debug.DrawTerrain(pos + data->center, data->norm, data->size, data->rotation, DebugDraw::COL_BEFOR, false);
            //}
            //else if (kind == ColliderBase::Kind::Box)
            //{
            //    auto data = dynamic_cast<ColliderBox*>(collider.get());
            //    debug.DrawCube(pos + data->center, data->size, data->rotation, DebugDraw::COL_BEFOR, false);
            //}
        }
#endif
#endif
    }
}

void MyEngine::Physics::CheckCollide()
{
    for (auto& item : m_oftArr)
    {
        auto& col = item->m_object->m_collider;
        m_l8Tree->Regist(&col->min, &col->max, item);
    }

    std::vector<Collidable*> colVec;
    auto colNum = m_l8Tree->GetAllCollisionList(colVec);

    colNum /= 2;
    for (DWORD i = 0; i < colNum; ++i)
    {
        auto objA = colVec[i * 2];
        auto objB = colVec[i * 2 + 1];
        const auto& colA = objA->m_collider;
        const auto& colB = objB->m_collider;

        if (!IsCollide(objA->m_rigid, objB->m_rigid, colA.get(), colB.get())) continue;

        auto throughA = objA->m_throughTag;
        auto throughB = objB->m_throughTag;
        bool isThrough = std::find(throughA.begin(), throughA.end(), objB->GetTag()) != throughA.end() 
            || std::find(throughB.begin(), throughB.end(), objA->GetTag()) != throughB.end();

        bool isTrigger = colA->isTrigger || colB->isTrigger || isThrough;
        if (isTrigger)
        {
            AddNewCollideInfo(objA, objB, m_newTirrigerInfo);
        }
        else
        {
            AddNewCollideInfo(objA, objB, m_newCollideInfo);
        }

        // Trigger‚Ìê‡‚ÍˆÊ’u•â³‚Í‚µ‚È‚¢
        if (isTrigger) continue;

        auto primary = objA;
        auto secondary = objB;
        auto primaryCollider = colA;
        auto secondaryCollider = colB;
        if (objA->m_priority < objB->m_priority)
        {
            primary = objB;
            secondary = objA;
            primaryCollider = colB;
            secondaryCollider = colA;
        }
        else if (objA->m_priority == objB->m_priority)
        {
            if (objA->m_rigid.GetVelocity().SqLength() < objB->m_rigid.GetVelocity().SqLength())
            {
                primary = objB;
                secondary = objA;
                primaryCollider = colB;
                secondaryCollider = colA;
            }
        }
            
        FixNextPos(primary->m_rigid, secondary->m_rigid, primaryCollider.get(), secondaryCollider.get());
    }
    
    
    for (auto& item : m_oftArr)
    {
        item->Remove();
    }
}

bool Physics::IsCollide(const Rigidbody& rigidA, const Rigidbody& rigidB, ColliderBase* colliderA, ColliderBase* colliderB)
{
    bool isCollide = false;

    auto kindA = colliderA->GetKind();
    auto kindB = colliderB->GetKind();

    if (kindA == ColliderBase::Kind::Sphere && kindB == ColliderBase::Kind::Sphere)
    {
        isCollide = IsCollideSphereAndSphere(rigidA, rigidB, colliderA, colliderB);
    }
    else if (kindA == ColliderBase::Kind::Capsule && kindB == ColliderBase::Kind::Capsule)
    {
        isCollide = IsCollideCapsuleAndCapsule(rigidA, rigidB, colliderA, colliderB);
    }
    else if (kindA == ColliderBase::Kind::Terrain && kindB == ColliderBase::Kind::Terrain)
    {
        isCollide = IsCollideSurfaceAndSurface(rigidA, rigidB, colliderA, colliderB);
    }
    else if (kindA == ColliderBase::Kind::Sphere && kindB == ColliderBase::Kind::Capsule
                ||
             kindA == ColliderBase::Kind::Capsule && kindB == ColliderBase::Kind::Sphere)
    {
        isCollide = IsCollideSphereAndCapsule(rigidA, rigidB, colliderA, colliderB);
    }
    else if (kindA == ColliderBase::Kind::Sphere && kindB == ColliderBase::Kind::Terrain
                ||
             kindA == ColliderBase::Kind::Terrain && kindB == ColliderBase::Kind::Sphere)
    {
        isCollide = IsCollideSphereAndTerrain(rigidA, rigidB, colliderA, colliderB);
    }
    else if (kindA == ColliderBase::Kind::Sphere && kindB == ColliderBase::Kind::Box
                ||
             kindA == ColliderBase::Kind::Box && kindB == ColliderBase::Kind::Sphere)
    {
        isCollide = IsCollideSphereAndBox(rigidA, rigidB, colliderA, colliderB);
    }
    else if (kindA == ColliderBase::Kind::Capsule && kindB == ColliderBase::Kind::Terrain
                ||
             kindA == ColliderBase::Kind::Terrain && kindB == ColliderBase::Kind::Capsule)
    {
        isCollide = IsCollideCapsuleAndTerrain(rigidA, rigidB, colliderA, colliderB);
    }

    return isCollide;
}

void MyEngine::Physics::FixNextPos(const Rigidbody& primaryRigid, Rigidbody& secondaryRigid, ColliderBase* primaryCollider, ColliderBase* secondaryCollider)
{
    Vec3 fixedPos = secondaryRigid.GetNextPos();

    auto primaryKind = primaryCollider->GetKind();
    auto secondaryKind = secondaryCollider->GetKind();
    
    if (primaryKind == ColliderBase::Kind::Sphere && secondaryKind == ColliderBase::Kind::Sphere)
    {
        fixedPos = FixSphereToSphere(primaryRigid, secondaryRigid, primaryCollider, secondaryCollider);
    }
    else if (primaryKind == ColliderBase::Kind::Terrain && secondaryKind == ColliderBase::Kind::Sphere)
    {
        fixedPos = FixTerrainToSphere(primaryRigid, secondaryRigid, primaryCollider, secondaryCollider);
    }
    else if (primaryKind == ColliderBase::Kind::Terrain && secondaryKind == ColliderBase::Kind::Capsule)
    {
        fixedPos = FixTerrainToCapsule(primaryRigid, secondaryRigid, primaryCollider, secondaryCollider);
    }
    else if (primaryKind == ColliderBase::Kind::Capsule && secondaryKind == ColliderBase::Kind::Capsule)
    {
        fixedPos = FixCapsuleToCapsule(primaryRigid, secondaryRigid, primaryCollider, secondaryCollider);
    }
    else if (primaryKind == ColliderBase::Kind::Capsule && secondaryKind == ColliderBase::Kind::Sphere)
    {
        fixedPos = secondaryRigid.GetPos();
    }

    secondaryRigid.SetNextPos(fixedPos);
}

void MyEngine::Physics::AddNewCollideInfo(Collidable* objA, Collidable* objB, SendCollideInfo& info)
{
    for (auto& i : info)
    {
        if (i.t1 == objA && i.t2 == objB) return;
        if (i.t1 == objB && i.t2 == objA) return;
    }

    info.emplace_back(Tuple{ objA, objB });
}

void MyEngine::Physics::CheckSendOnCollideInfo(SendCollideInfo& preSendInfo, SendCollideInfo& newSendInfo, bool isTrigger)
{
    auto isPreExist = preSendInfo.size() != 0;

    for (auto& info : newSendInfo)
    {
        bool isEntry = false;

        if (isPreExist)
        {
            bool isEnter = true;
            auto it = preSendInfo.begin();
            for (; it != preSendInfo.end(); ++it)
            {
                if (it->t1 == info.t1 && it->t2 == info.t2) isEnter = false;
                if (it->t1 == info.t2 && it->t2 == info.t1) isEnter = false;
                if (!isEnter) break;
            }
            if (isEnter)
            {
                if (isTrigger)
                {
                    AddOnCollideInfo(info.t1, info.t2, OnCollideInfoKind::TriggerEnter);
                    AddOnCollideInfo(info.t2, info.t1, OnCollideInfoKind::TriggerEnter);
                }
                else
                {
                    AddOnCollideInfo(info.t1, info.t2, OnCollideInfoKind::CollideEnter);
                    AddOnCollideInfo(info.t2, info.t1, OnCollideInfoKind::CollideEnter);
                }
            }
            else
            {
                preSendInfo.erase(it);
            }
        }
        else
        {
            if (isTrigger)
            {
                AddOnCollideInfo(info.t1, info.t2, OnCollideInfoKind::TriggerEnter);
                AddOnCollideInfo(info.t2, info.t1, OnCollideInfoKind::TriggerEnter);
            }
            else
            {
                AddOnCollideInfo(info.t1, info.t2, OnCollideInfoKind::CollideEnter);
                AddOnCollideInfo(info.t2, info.t1, OnCollideInfoKind::CollideEnter);
            }
        }

        // Stary‚Í–ˆ“xŒÄ‚Ô
        if (isTrigger)
        {
            AddOnCollideInfo(info.t1, info.t2, OnCollideInfoKind::TriggerStay);
            AddOnCollideInfo(info.t2, info.t1, OnCollideInfoKind::TriggerStay);
        }
        else
        {
            AddOnCollideInfo(info.t1, info.t2, OnCollideInfoKind::CollideStay);
            AddOnCollideInfo(info.t2, info.t1, OnCollideInfoKind::CollideStay);
        }
    }

    for (auto& info : preSendInfo)
    {
        if (isTrigger)
        {
            AddOnCollideInfo(info.t1, info.t2, OnCollideInfoKind::TriggerExit);
            AddOnCollideInfo(info.t2, info.t1, OnCollideInfoKind::TriggerExit);
        }
        else
        {
            AddOnCollideInfo(info.t1, info.t2, OnCollideInfoKind::CollideExit);
            AddOnCollideInfo(info.t2, info.t1, OnCollideInfoKind::CollideExit);
        }
    }
}

void MyEngine::Physics::AddOnCollideInfo(Collidable* own, Collidable* send, OnCollideInfoKind kind)
{
    OnCollideInfoData info;
    info.own = own;
    info.send = send;
    info.kind = kind;
    m_onCollideInfo.emplace_back(info);
}

void MyEngine::Physics::OnCollideInfo(Collidable* own, Collidable* send, OnCollideInfoKind kind)
{
    if (!own || !send) return;

    if (kind == OnCollideInfoKind::CollideEnter)
    {
        own->OnCollideEnter(send);
    }
    else if (kind == OnCollideInfoKind::CollideStay)
    {
        own->OnCollideStay(send);
    }
    else if (kind == OnCollideInfoKind::CollideExit)
    {
        own->OnCollideExit(send);
    }
    else if (kind == OnCollideInfoKind::TriggerEnter)
    {
        own->OnTriggerEnter(send);
    }
    else if (kind == OnCollideInfoKind::TriggerStay)
    {
        own->OnTriggerStay(send);
    }
    else if (kind == OnCollideInfoKind::TriggerExit)
    {
        own->OnTriggerExit(send);
    }
}

/// <summary>
/// ÅI“I‚È–¢—ˆ‚ÌÀ•W‚©‚çŒ»İ‚ÌÀ•W‚É“K—p
/// </summary>
void Physics::FixPos() const
{
    for (const auto& item : m_collidables)
    {
        auto& rigid = item->m_rigid;

        rigid.SetPos(rigid.GetNextPos());

#ifdef _DEBUG
        auto& debug = DebugDraw::GetInstance();

        auto kind = item->m_collider->GetKind();
        if (kind == ColliderBase::Kind::Sphere)
        {
            auto data = dynamic_cast<ColliderSphere*>(item->m_collider.get());
            debug.DrawSphere(rigid.GetPos() + data->center, data->radius, DebugDraw::COL_AFFTER, false);
        }
        else if (kind == ColliderBase::Kind::Capsule)
        {
            auto data = dynamic_cast<ColliderCapsule*>(item->m_collider.get());
            debug.DrawCapsule(rigid.GetPos() + data->center, data->dir, data->radius, data->size, DebugDraw::COL_AFFTER, false);
        }
#if false
        for (const auto& collider : item->m_colliders)
        {
            auto kind = collider->GetKind();
            if (kind == ColliderBase::Kind::Sphere)
            {
                auto data = dynamic_cast<ColliderSphere*>(collider.get());
                debug.DrawSphere(rigid.GetPos() + data->center, data->radius, DebugDraw::COL_AFFTER, false);
            }
            else if (kind == ColliderBase::Kind::Capsule)
            {
                auto data = dynamic_cast<ColliderCapsule*>(collider.get());
                debug.DrawCapsule(rigid.GetPos() + data->center, data->dir, data->radius, data->size, DebugDraw::COL_AFFTER, false);
            }
        }
#endif
#endif
    }

}
