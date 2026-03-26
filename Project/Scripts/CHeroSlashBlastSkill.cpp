#include "pch.h"
#include "CHeroSlashBlastSkill.h"

CHeroSlashBlastSkill::CHeroSlashBlastSkill(UINT _Type)
    : CSkillScript(_Type)
    , m_Speed(600.f)
{
    //AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Speed", &m_Speed });
}

CHeroSlashBlastSkill::CHeroSlashBlastSkill(const CHeroSlashBlastSkill& _Other)
    : CSkillScript(_Other)
    , m_Speed(_Other.m_Speed)
{
    //AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Speed", &m_Speed });
}

CHeroSlashBlastSkill::~CHeroSlashBlastSkill()
{
}

void CHeroSlashBlastSkill::Init()
{
    //GetOwner()->FlipbookPlayer()->Play(0, 10, true);
    //GetUser()->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\SlashBlast.flip"));

}

void CHeroSlashBlastSkill::Tick()
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

void CHeroSlashBlastSkill::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
    if (L"MonsterBody" == _OtherObject->GetName())
    {
        //CGameObject* pMonObj = _OtherObject->GetParent();
        //CMonsterScript* pMonster = pMonObj->GetScript<CMonsterScript>();
        //if (pMonster)
        //{
        //    // 데미지 처리
        //    pMonster->Hit(5);

        //    // 화살에 맞은 방향으로 넉백
        //    Vector3 vHitDir = pMonObj->Transform()->GetWorldPos() - Transform()->GetWorldPos();
        //    vHitDir.Normalize();

        //    pMonObj->RigidBody2D()->AddVelocity(Vector3(vHitDir.x * 200.f, 50.f, 0.f));

        //    // 화살 오브젝트 제거
        //    DestroyObject(GetOwner());
        //}
    }
}


void CHeroSlashBlastSkill::SaveComponent(FILE* _File)
{
    CSkillScript::SaveComponent(_File);
    //fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CHeroSlashBlastSkill::LoadComponent(FILE* _File)
{
    CSkillScript::LoadComponent(_File);
    //fread(&m_Speed, sizeof(float), 1, _File);
}