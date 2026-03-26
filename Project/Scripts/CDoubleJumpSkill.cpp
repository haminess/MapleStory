#include "pch.h"
#include "CDoubleJumpSkill.h"

CDoubleJumpSkill::CDoubleJumpSkill(UINT _Type)
    : CSkillScript(_Type)
    , m_JumpPower(5.f)
    , m_IsDone(false)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Jump Power", &m_JumpPower });
}

CDoubleJumpSkill::CDoubleJumpSkill(const CDoubleJumpSkill& _Other)
    : CSkillScript(_Other)
    , m_JumpPower(_Other.m_JumpPower)
    , m_IsDone(false)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Jump Power", &m_JumpPower });
}

CDoubleJumpSkill::~CDoubleJumpSkill()
{
}

void CDoubleJumpSkill::Begin()
{
    
}

void CDoubleJumpSkill::Tick()
{
    CSkillScript::Tick();

    if (KEY_TAP(KEY::ALT) && !m_IsDone)
    {
        if (KEY_PRESSED(KEY::UP))
        {
            GetUser()->RigidBody2D()->Jump(Vector3(0.f, 1.f, 0.f) * m_JumpPower);
            m_IsDone = true;
        }
        else if (KEY_PRESSED(KEY::LEFT))
        {
            GetUser()->RigidBody2D()->Jump(Vector3(-1.f, 1.f, 0.f) * m_JumpPower);
            m_IsDone = true;
        }
        else if (KEY_PRESSED(KEY::RIGHT))
        {
            GetUser()->RigidBody2D()->Jump(Vector3(1.f, 1.f, 0.f) * m_JumpPower);
            m_IsDone = true;
        }
        else
        {
            GetUser()->RigidBody2D()->Jump();
            m_IsDone = true;
        }
    }
}

void CDoubleJumpSkill::SaveComponent(FILE* _File)
{
}

void CDoubleJumpSkill::LoadComponent(FILE* _FILE)
{
}
