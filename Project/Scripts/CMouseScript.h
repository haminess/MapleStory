#pragma once
#include <Engine/CScript.h>
class CMouseScript :
    public CScript
{
private:
    class CGameObject* m_Cam;
    CGameObject* m_Texture;
    CGameObject* m_ParticleChild;
    float m_Time;
    CParticleSystem* m_HitParticle;
    CParticleSystem* m_DSParticle;
    Ptr<CTexture> m_SkillHitTex;
    Ptr<CTexture> m_DamageSkinTex[11];

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)override;

public:
    CLONE(CMouseScript);
    CMouseScript();
    ~CMouseScript();
};

