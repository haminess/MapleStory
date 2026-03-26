#pragma once
#include <Engine/CScript.h>
class CRope :
    public CScript
{
public:
    virtual void Init() override;
    virtual void Tick() {}

public:
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    CLONE(CRope);
    CRope();
    ~CRope();
};

