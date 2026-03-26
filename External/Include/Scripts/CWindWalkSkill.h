#pragma once
#include "CSkillScript.h"
class CWindWalkSkill :
    public CSkillScript
{
private:
    Ptr<CPrefab>    m_Tail;
    float           m_MoveAmount;

public:
    Ptr<CPrefab> GetArrow() { return m_Tail; }
    void SetArrow(Ptr<CPrefab> _Arrow) { m_Tail = _Arrow; }

public:
    virtual void Use() override {}

    virtual void Begin() override;
    virtual void Tick() override {}

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    CLONE(CWindWalkSkill);
    CWindWalkSkill(UINT _Type = (UINT)SCRIPT_TYPE::WINDWALKSKILL);
    CWindWalkSkill(const CWindWalkSkill& _Other);
    ~CWindWalkSkill();
};

