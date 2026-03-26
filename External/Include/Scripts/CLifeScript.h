#pragma once
#include "CScriptMgr.h"
#include <Engine/CScript.h>

class CLifeScript :
    public CScript
{
private:
    int m_HP;      // 현재 체력
    int m_Power;    // 공격력

    int m_MaxHP;

public:
    // 기존 함수들
    virtual void Damage(int _Power);
    virtual void DamageByPercent(float _Ratio);
    virtual void Dead();

    // 추가된 함수들
    int GetHP() const { return m_HP; }              
    int GetMaxHP() const { return m_MaxHP; }              
    void SetMaxHP(float _MaxHP) { m_MaxHP = _MaxHP; }
    void SetPower(float _Power) { m_Power = _Power; }
    int GetPower() const { return m_Power; }
    void Heal(float _Heal) { m_HP += _Heal; }

public:
    virtual void Tick() override {}
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CLifeScript);
    CLifeScript(UINT _Type = (UINT)SCRIPT_TYPE::LIFESCRIPT);
    CLifeScript(const CLifeScript& _Other);
    ~CLifeScript();
};