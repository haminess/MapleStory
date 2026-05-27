// CLifeScript.cpp
#include "pch.h"
#include "CLifeScript.h"
#include "CDamageSkinScript.h"

CLifeScript::CLifeScript(UINT _Type)
    : CScript(_Type)
    , m_HP(100)
    , m_MaxHP(100)
    , m_MP(100)
    , m_MaxMP(100)
    , m_Power(10)
    , m_IsDead(false)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Max HP", &m_MaxHP });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "HP", &m_HP });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Max MP", &m_MaxMP });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "MP", &m_MP });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Power", &m_Power });
}

CLifeScript::CLifeScript(const CLifeScript& _Other)
    : CScript(_Other)
    , m_HP(_Other.m_HP)
    , m_MaxHP(_Other.m_MaxHP)
    , m_MP(_Other.m_MP)
    , m_MaxMP(_Other.m_MaxMP)
    , m_Power(_Other.m_Power)
    , m_IsDead(_Other.m_IsDead)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Max HP", &m_MaxHP });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "HP", &m_HP });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Max MP", &m_MaxMP });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "MP", &m_MP });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Power", &m_Power });
}


CLifeScript::~CLifeScript()
{
}

void CLifeScript::Damage(int _Power, int _Count)
{
    m_HP -= _Power;

    // HP가 0 이하면 사망 처리
    if (m_HP <= 0)
    {
        m_HP = 0;

        if(!m_IsDead)
            Dead();
    }

    CDamageSkinScript::PlayDamageEffect(_Power, Transform()->GetWorldPos() + Vector3(0.f, Transform()->GetWorldScale().y/2.f + _Count * 30.f, 0.f));
}

void CLifeScript::DamageByPercent(float _Ratio)
{
    int power = m_MaxHP * _Ratio;
    Damage(power);
}

void CLifeScript::Dead()
{
    m_IsDead = true;

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
    fwrite(&m_MP, sizeof(int), 1, _File);
    fwrite(&m_MaxMP, sizeof(int), 1, _File);
}

void CLifeScript::LoadComponent(FILE* _File)
{
    fread(&m_HP, sizeof(int), 1, _File);
    fread(&m_MaxHP, sizeof(int), 1, _File);
    fread(&m_Power, sizeof(int), 1, _File);
    fread(&m_MP, sizeof(int), 1, _File);
    fread(&m_MaxMP, sizeof(int), 1, _File);
}
