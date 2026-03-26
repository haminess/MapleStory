#pragma once
#include "CScript.h"

class CPlayerScript :
    public CScript
{

private:
    float   m_PlayerSpeed;
    Vector3 m_PlayerPos;
    float   m_PaperBurnIntence;
    CGameObject* m_Platform;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

public:
    CPlayerScript();
    ~CPlayerScript();
};

