// CLifeScript.cpp
#include "pch.h"
#include "CLifeScript.h"

CLifeScript::CLifeScript(UINT _Type)
    : CScript(_Type)
    , m_HP(100)
    , m_MaxHP(100)
    , m_Power(10)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Max HP", &m_MaxHP });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "HP", &m_HP });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Power", &m_Power });
}

CLifeScript::CLifeScript(const CLifeScript& _Other)
    : CScript(_Other)
    , m_HP(_Other.m_HP)
    , m_MaxHP(_Other.m_MaxHP)
    , m_Power(_Other.m_Power)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Max HP", &m_MaxHP });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "HP", &m_HP });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Power", &m_Power });
}


CLifeScript::~CLifeScript()
{
}

void CLifeScript::Damage(int _Power)
{
    m_HP -= _Power;

    // HP가 0 이하면 사망 처리
    if (m_HP <= 0)
    {
        m_HP = 0;
        Dead();
    }
}

void CLifeScript::DamageByPercent(float _Ratio)
{
    int power = m_MaxHP * _Ratio;
    Damage(power);
}

void CLifeScript::Dead()
{
    if (StateMachine() && StateMachine()->FindState(L"DieState"))
    {
        StateMachine()->ChangeState(L"DieState");
    }
}

void CLifeScript::SaveComponent(FILE* _File)
{
    fwrite(&m_HP, sizeof(int), 1, _File);
    fwrite(&m_MaxHP, sizeof(int), 1, _File);
    fwrite(&m_Power, sizeof(int), 1, _File);
}

void CLifeScript::LoadComponent(FILE* _File)
{
    fread(&m_HP, sizeof(int), 1, _File);
    fread(&m_MaxHP, sizeof(int), 1, _File);
    fread(&m_Power, sizeof(int), 1, _File);
}
