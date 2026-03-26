// CArrowSkill.h
#pragma once
#include "CSkillScript.h"

class CArrowSkill :
    public CSkillScript
{
private:
    float   m_Speed;    // 화살 이동 속도

public:
    virtual void Use() override {}

    virtual void Init() override;
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    CLONE(CArrowSkill);
    CArrowSkill(UINT _Type = (UINT)SCRIPT_TYPE::ARROWSKILL);
    CArrowSkill(const CArrowSkill& _Other);
    ~CArrowSkill();
};