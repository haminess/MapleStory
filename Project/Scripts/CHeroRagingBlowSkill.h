#pragma once
#include "CSkillScript.h"
class CHeroRagingBlowSkill :
    public CSkillScript
{
private:
    Ptr<CFlipbook> m_AttackFlip[4];
    Ptr<CFlipbook> m_HitFlip[4];

    CGameObject* m_RagingBlow;
    int m_Step;
    int m_MaxMob;
    int m_AttackCount;

public:
    virtual void Use() override;
    virtual void End() override;
    virtual void SkillKeyTap() override;
    virtual void SkillKeyPressed() override;
    virtual void SkillKeyReleased() override;

    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

    void HitMonster(DWORD_PTR _Col, DWORD_PTR _Other);

public:
    CLONE(CHeroRagingBlowSkill);
    CHeroRagingBlowSkill(UINT _Type = (UINT)SCRIPT_TYPE::HERORAGINGBLOWSKILL);
    CHeroRagingBlowSkill(const CHeroRagingBlowSkill& _Other);
    ~CHeroRagingBlowSkill();
};
