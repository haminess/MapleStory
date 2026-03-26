#pragma once
#include "CSkillScript.h"



class CLucidFairyDustSkill :
    public CSkillScript
{
private:
    vector<CGameObject*> m_Projectile;
    Ptr<CFlipbook> m_LucidAnim;
    Ptr<CFlipbook> m_ProjectileAnim;

    float   m_Speed;
    float   m_MaxTime;
    float   m_CurTime;

    bool    m_IsStarted;

    class CLucidScript* m_Lucid;
    
public:
    void SpawnFairyDust();

    void ProjectileBeginOverlap(DWORD_PTR _Collider, DWORD_PTR _OtherCollider);

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Use() override;
    virtual void Tick() override;

public:
    CLONE(CLucidFairyDustSkill);
    CLucidFairyDustSkill(UINT _Type = (UINT)SCRIPT_TYPE::LUCIDFAIRYDUSTSKILL);
    CLucidFairyDustSkill(const CLucidFairyDustSkill& _Other);
    ~CLucidFairyDustSkill();
};
