#include "pch.h"
#include "CWindWalkSkill.h"

CWindWalkSkill::CWindWalkSkill(UINT _Type)
    : CSkillScript(_Type)
    , m_Tail(nullptr)
    , m_MoveAmount(600.f)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::PREFAB, "Tail Effect", &m_Tail });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Move Amount", &m_MoveAmount });
}

CWindWalkSkill::CWindWalkSkill(const CWindWalkSkill& _Other)
    : CSkillScript(_Other)
    , m_Tail(_Other.m_Tail)
    , m_MoveAmount(_Other.m_MoveAmount)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::PREFAB, "Tail Effect", &m_Tail });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Move Amount", &m_MoveAmount });
}

CWindWalkSkill::~CWindWalkSkill()
{
}

void CWindWalkSkill::Begin()
{
    if (!GetUser()) return;
    CRigidBody2D* pRigid = GetUser()->RigidBody2D();
    if (pRigid == nullptr) return;

    Vector3 vVelo = pRigid->GetVelocity();
    vVelo.Normalize();
    Vector3 vDir = Vector3(/*vVelo.x * m_MoveAmount * DT*/500.f, 0.f, 0.f);
    GetUser()->RigidBody2D()->AddVelocity(vDir);

    if (m_Tail != nullptr)
        CreateObject(m_Tail->Instantiate(), (int)LAYER_INDEX::PLAYER, false);
}

void CWindWalkSkill::SaveComponent(FILE* _File)
{
    SaveAssetRef(m_Tail, _File);
    fwrite(&m_MoveAmount, sizeof(float), 1, _File);
}

void CWindWalkSkill::LoadComponent(FILE* _File)
{
    LoadAssetRef(m_Tail, _File);
    fread(&m_MoveAmount, sizeof(float), 1, _File);
}
