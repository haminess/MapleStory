// CBowSkill.cpp
#include "pch.h"
#include "CBowSkill.h"
#include "CMonsterScript.h"

#include <Engine/CLevelMgr.h>

CBowSkill::CBowSkill(UINT _Type)
    : CSkillScript(_Type)
    , m_ShootCoolTime(0.2f)
    , m_AccTime(0.f)
    , m_bKeyPressed(false)
    , m_Arrow(nullptr)
    , m_Step(0)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::PREFAB, "Arrow", &m_Arrow });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Shoot Cool Time", &m_ShootCoolTime });
}

CBowSkill::CBowSkill(const CBowSkill& _Other)
    : CSkillScript(_Other)
    , m_ShootCoolTime(_Other.m_ShootCoolTime)
    , m_AccTime(_Other.m_AccTime)
    , m_bKeyPressed(_Other.m_bKeyPressed)
    , m_Arrow(_Other.m_Arrow)
    , m_Step(_Other.m_Step)
{
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::PREFAB, "Arrow", &m_Arrow });
    AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Shoot Cool Time", &m_ShootCoolTime });
}

CBowSkill::~CBowSkill()
{
}

void CBowSkill::Init()
{
    //m_Arrow = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"Prefab\\SongOfHeavenArrow.pref");
    GetOwner()->FlipbookPlayer()->Play(0, 10, false);
    m_AccTime = m_ShootCoolTime;
}

void CBowSkill::Tick()
{
    // 부모 클래스의 Tick 호출
    //CSkill::Tick();
    switch (m_Step)
    {
    case 0:
        if (!GetOwner()->FlipbookPlayer()->IsPlaying())
        {
            ++m_Step;
            GetOwner()->FlipbookPlayer()->Play(1, 10, true);
        }
        break;
    case 1:
    {
        // 키 입력 확인 및 처리
        if (KEY_PRESSED(GetKey()))
        {
            m_bKeyPressed = true;
            m_AccTime += DT;

            if (m_AccTime >= m_ShootCoolTime)
            {
                ShootArrow();
                m_AccTime = 0.f;
            }
        }
        else if (KEY_RELEASED(GetKey()))
        {
            ++m_Step;
            m_bKeyPressed = false;
            m_AccTime = 0.f;
            GetOwner()->FlipbookPlayer()->Play(2, 10, false);
        }
    }
        break;
    case 2:
        if (!GetOwner()->FlipbookPlayer()->IsPlaying())
        {
            DestroyObject(GetOwner());
        }
        break;
    }

    if(m_Step > 1 && !GetOwner()->FlipbookPlayer()->IsPlaying())
        DestroyObject(GetOwner());
}

void CBowSkill::ShootArrow()
{
    if (m_Arrow == nullptr)
        return;

    // Arrow 프리팹 생성
    CGameObject* pArrow = m_Arrow->Instantiate();
    CreateObject(pArrow, (int)LAYER_INDEX::PLAYER, true);

    // Arrow의 시작 위치를 스킬 소유자의 위치로 설정
    Vector3 vPos = Transform()->GetWorldPos();
    Vector3 vRot = CLevelMgr::GetInst()->GetCurrentPlayer()->Transform()->GetRelativeRotation();
    pArrow->Transform()->SetRelativePos(vPos);
    pArrow->Transform()->SetRelativeRotation(vRot.x, -vRot.y, vRot.z);
}



void CBowSkill::SaveComponent(FILE* _File)
{
    CSkillScript::SaveComponent(_File);

    SaveAssetRef(m_Arrow, _File);
    fwrite(&m_ShootCoolTime, sizeof(float), 1, _File);
}

void CBowSkill::LoadComponent(FILE* _File)
{
    CSkillScript::LoadComponent(_File);

    LoadAssetRef(m_Arrow, _File);
    fread(&m_ShootCoolTime, sizeof(float), 1, _File);
}