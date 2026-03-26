#pragma once
#include "CSkillScript.h"
class CLucidDragonSkill :
    public CSkillScript
{
private:
    CGameObject*   m_DragonObj;
    CGameObject*   m_AlertObj;

    Ptr<CPrefab>   m_Dragon;
    

    Vector3 m_DragonSpawnLeft;
    Vector3 m_DragonSpawnRight;

    Vector3 m_BreathLB;
    Vector3 m_BreathRT;
    int     m_BreathCount;


public:
    void SpawnDragon();
    void AlertDragonAttackPre();
    void AlertDragonAttack();
    void AlertClear();
    void AttackBeginOvelap(DWORD_PTR _Collider, DWORD_PTR _OtherCollider);

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Use() override;
    virtual void End() override;

public:
    CLONE(CLucidDragonSkill);
    CLucidDragonSkill(UINT _Type = (UINT)SCRIPT_TYPE::LUCIDDRAGONSKILL);
    CLucidDragonSkill(const CLucidDragonSkill& _Other);
    ~CLucidDragonSkill();
};

