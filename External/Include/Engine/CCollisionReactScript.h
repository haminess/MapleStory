#pragma once
#include "CScript.h"
class CCollisionReactScript :
    public CScript
{
private:
    virtual void Tick() override {};

public:
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

public:
    CCollisionReactScript();
    ~CCollisionReactScript();
};

