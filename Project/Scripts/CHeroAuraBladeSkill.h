#pragma once
#include "CSkillScript.h"
class CHeroAuraBladeSkill :
    public CSkillScript
{
public:
    virtual void SkillKeyTap() override;
    virtual void Tick() override;

public:
    CLONE(CHeroAuraBladeSkill)
    CHeroAuraBladeSkill(UINT _Type = (UINT)SCRIPT_TYPE::HEROAURABLADESKILL);
    CHeroAuraBladeSkill(const CHeroAuraBladeSkill& _Other);
    ~CHeroAuraBladeSkill();
};

