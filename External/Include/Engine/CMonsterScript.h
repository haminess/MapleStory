#pragma once
#include "CScript.h"
class CMonsterScript :
    public CScript
{
private:
    Ptr<CTexture> m_StandTex;
    Ptr<CTexture> m_HitTex;
    Ptr<CTexture> m_ButterflyTex[3];
    CParticleSystem* m_Particle;

    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {}
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

public:
    CMonsterScript();
    ~CMonsterScript();
};

