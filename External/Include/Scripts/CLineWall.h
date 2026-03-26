#pragma once
#include <Engine/CScript.h>

class CLineWall :
    public CScript
{
private:

public:
    virtual void Tick() override {}
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    CLONE(CLineWall);
    CLineWall();
    ~CLineWall();
};

