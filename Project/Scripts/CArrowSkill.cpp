// CArrowSkill.cpp
#include "pch.h"
#include "CArrowSkill.h"
#include "CMonsterScript.h"

CArrowSkill::CArrowSkill(UINT _Type)
    : CSkillScript(_Type)
    , m_Speed(600.f)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Speed", &m_Speed });
}

CArrowSkill::CArrowSkill(const CArrowSkill& _Other)
    : CSkillScript(_Other)
    , m_Speed(_Other.m_Speed)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Speed", &m_Speed });
}

CArrowSkill::~CArrowSkill()
{
}

void CArrowSkill::Init()
{
    GetOwner()->FlipbookPlayer()->Play(0, 10, true);
}

void CArrowSkill::Tick()
{
    // 부모 클래스의 Tick 호출 (수명 관리)
    CSkillScript::Tick();

    // 화살의 현재 방향으로 계속 이동
    Vector3 vCurDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
    Vector3 vCurPos = Transform()->GetRelativePos();

    // 속도에 따른 이동
    vCurPos += -vCurDir * m_Speed * DT;
    Transform()->SetRelativePos(vCurPos);
}

void CArrowSkill::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (L"MonsterBody" == _OtherObject->GetName())
    {
        CGameObject* pMonObj = _OtherObject->GetParent();
        CMonsterScript* pMonster = pMonObj->GetScript<CMonsterScript>();
        if (pMonster)
        {
            // 데미지 처리
            pMonster->Damage(5);

            // 화살에 맞은 방향으로 넉백
            Vector3 vHitDir = pMonObj->Transform()->GetWorldPos() - Transform()->GetWorldPos();
            vHitDir.Normalize();

            pMonObj->RigidBody2D()->AddVelocity(Vector3(vHitDir.x * 200.f, 50.f, 0.f));

            // 화살 오브젝트 제거
            DestroyObject(GetOwner());
        }
    }
}


void CArrowSkill::SaveComponent(FILE* _File)
{
    CSkillScript::SaveComponent(_File);
    fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CArrowSkill::LoadComponent(FILE* _File)
{
    CSkillScript::LoadComponent(_File);
    fread(&m_Speed, sizeof(float), 1, _File);
}