#pragma once
#include "CSkillScript.h"
class CHeroWarLeapSkill :
    public CSkillScript
{
private:
    int m_Count;

    Ptr<CFlipbook> m_BasicEff;
    Ptr<CFlipbook> m_UpEff;
    Ptr<CFlipbook> m_TailEff;

public:
    virtual void Use() override;
    virtual void SkillKeyTap() override;

    virtual void Init() override;
    virtual void Tick() override;

public:
    CLONE(CHeroWarLeapSkill);
    CHeroWarLeapSkill(UINT _Type = (UINT)SCRIPT_TYPE::HEROWARLEAPSKILL);
    CHeroWarLeapSkill(const CHeroWarLeapSkill& _Other);
    ~CHeroWarLeapSkill();
};
