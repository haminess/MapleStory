#pragma once
#include "CSkillScript.h"
class CStrikeSkill :
    public CSkillScript
{
private:
    int m_Count;
    int m_Power;


public:
    virtual void Use() override {}

    virtual void Init() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    CLONE(CStrikeSkill);
    CStrikeSkill(UINT _Type = (UINT)SCRIPT_TYPE::STRIKESKILL);
    CStrikeSkill(const CStrikeSkill& _Other);
    ~CStrikeSkill();
};

