// CBowSkill.h
#pragma once
#include "CSkillScript.h"
#include <Engine/CPrefab.h>

class CBowSkill :
    public CSkillScript
{
private:
    Ptr<CPrefab>    m_Arrow;        // 화살 프리팹
    float           m_ShootCoolTime;// 발사 쿨타임
    float           m_AccTime;      // 누적 시간
    bool            m_bKeyPressed;  // 키 눌림 상태

    int m_Step;

private:
    void ShootArrow();    // 화살 발사 함수

public:
    Ptr<CPrefab> GetArrow() { return m_Arrow; }
    void SetArrow(Ptr<CPrefab> _Arrow) { m_Arrow = _Arrow; }

    virtual void Use() override {}

public:
    virtual void Init() override;
    virtual void Tick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    CLONE(CBowSkill);
    CBowSkill(UINT _Type = (UINT)SCRIPT_TYPE::BOWSKILL);
    CBowSkill(const CBowSkill& _Other);
    ~CBowSkill();
};