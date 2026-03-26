#pragma once
#include "CScript.h"
class CPlatformScript :
    public CScript
{
private:
    Vector3 m_StartPos;
    Vector3 m_EndPos;
    Vector3 m_vecInclination;
    float m_Height;

    bool m_SideCollidable;

public:
    void SetPlatformPos(Vector3 _StartPos, Vector3 _EndPos, float _Height = 20.f);
    void SetInclination(Vector3 _Dir)
    {
        m_vecInclination = _Dir;
        m_vecInclination.Normalize();
    }

    Vector3 GetStartPos() { return m_StartPos; }
    Vector3 GetEndPos() { return m_EndPos; }
    float GetHeight() { return m_Height; }
    Vector3 GetInclination() { return m_vecInclination; }

private:
    void BlockOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    virtual void Begin();
    virtual void Tick() override {}
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    CPlatformScript();
    ~CPlatformScript();
};

