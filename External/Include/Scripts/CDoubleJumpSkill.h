#pragma once
#include "CSkillScript.h"
class CDoubleJumpSkill :
    public CSkillScript
{
private:
    bool            m_IsDone;
    float           m_JumpPower;

public:
    virtual void Use() override {}

    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    CLONE(CDoubleJumpSkill);
    CDoubleJumpSkill(UINT _Type = (UINT)SCRIPT_TYPE::DOUBLEJUMPSKILL);
    CDoubleJumpSkill(const CDoubleJumpSkill& _Other);
    ~CDoubleJumpSkill();
};

