#include "pch.h"
#include "CLucidDragonSkill.h"
#include "CProjectileScript.h"
#include "CLucidDragonScript.h"
#include "CPlayerScript.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CTaskMgr.h>
#include <Engine/CSprite.h>

CLucidDragonSkill::CLucidDragonSkill(UINT _Type)
	: CSkillScript(_Type)
	, m_DragonSpawnLeft(Vector3(-275.f, 688.f, 0.f))
	, m_DragonSpawnRight(Vector3(2308.f, 688.f, 0.f))
	, m_BreathLB(Vector3(-3539.f, 269.f, 0.f))
	, m_BreathRT(Vector3(-549.f, 152.f, 0.f))
	, m_BreathCount(10)

{
	AddScriptParam({ SCRIPT_PARAM::VEC3, "DragonSpawn L", &m_DragonSpawnLeft });
	AddScriptParam({ SCRIPT_PARAM::VEC3, "DragonSpawn R", &m_DragonSpawnRight });
	AddScriptParam({ SCRIPT_PARAM::VEC3, "Breath LB", &m_BreathLB });
	AddScriptParam({ SCRIPT_PARAM::VEC3, "Breath RT", &m_BreathRT });
	AddScriptParam({ SCRIPT_PARAM::INT, "Breath Cnt", &m_BreathCount });
}

CLucidDragonSkill::CLucidDragonSkill(const CLucidDragonSkill& _Other)
	: CSkillScript(_Other)
{
}

CLucidDragonSkill::~CLucidDragonSkill()
{
}

void CLucidDragonSkill::Init()
{
	m_Dragon = FIND_PREFAB(L"Prefab\\Lucid Dragon.pref");

	m_AlertObj = CreateAnimObject();
	m_AlertObj->SetName(L"AreaWarning");
	m_AlertObj->FlipbookPlayer()->AddFlipbook(0, FIND_ANIM(L"Flipbook\\LucidDragon_AlertPre.flip"));
	m_AlertObj->FlipbookPlayer()->AddFlipbook(1, FIND_ANIM(L"Flipbook\\LucidDragon_AlertStart.flip"));
	m_AlertObj->FlipbookPlayer()->AddFlipbook(2, FIND_ANIM(L"Flipbook\\LucidDragon_AlertLoop.flip"));
	m_AlertObj->FlipbookPlayer()->AddFlipbook(3, FIND_ANIM(L"Flipbook\\LucidDragon_AlertEnd.flip"));

	CreateObject(m_AlertObj, (int)LAYER_INDEX::MONSTER_OBJECT_1, false);
}

void CLucidDragonSkill::Begin()
{
	// 공격 범위 오브젝트
	Vector3 vBreathScale = m_BreathRT - m_BreathLB;
	Vector3 vBreathPos = m_BreathLB + Vector3(vBreathScale.x / 2.f, vBreathScale.y / 2.f, 0.f);

	m_DragonObj = m_Dragon->Instantiate();
	m_DragonObj->SetActive(false);
	CreateObject(m_DragonObj, (int)LAYER_INDEX::MONSTER, false);
}

void CLucidDragonSkill::Use()
{
	CSkillScript::Use();
	
	// 루시드 애니메이션
	FlipbookPlayer()->Play(2, 8.6f, false);

	// 드래곤 소환
	CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CLucidDragonSkill::AlertDragonAttackPre, 2.f, false);

	// 드래곤 소환
	CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CLucidDragonSkill::AlertDragonAttack, 3.f, false);
	CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CLucidDragonSkill::SpawnDragon, 4.f, false);
	CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CLucidDragonSkill::AlertClear, 11.f, false);
	CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CLucidDragonSkill::End, 20.f, false);
}

void CLucidDragonSkill::End()
{
	CSkillScript::End();

	m_DragonObj->SetActive(false);
	m_AlertObj->SetActive(false);
}

void CLucidDragonSkill::Tick()
{
	CSkillScript::Tick();

	if (!IsStarted())
		return;
}

void CLucidDragonSkill::SpawnDragon()
{
	m_AlertObj->FlipbookPlayer()->Play(2, 10.f, true);

	m_DragonObj->SetActive(true);
	m_DragonObj->GetScript<CLucidDragonScript>()->Spawn();
}

void CLucidDragonSkill::AlertDragonAttackPre()
{
	m_AlertObj->SetActive(true);
	m_AlertObj->FlipbookPlayer()->Play(0, 7.f, false);
}

void CLucidDragonSkill::AlertDragonAttack()
{
	// 10 1초에 10개
	// 문구 + 안개
	m_AlertObj->FlipbookPlayer()->Play(1, 7.f, false);
}

void CLucidDragonSkill::AlertClear()
{
	m_AlertObj->FlipbookPlayer()->Play(3, 10.f, false);
}

void CLucidDragonSkill::AttackBeginOvelap(DWORD_PTR _Collider, DWORD_PTR _OtherCollider)
{
	CGameObject* pPlayer = ((CCollider2D*)_OtherCollider)->GetOwner();

	if (pPlayer->GetLayerIdx() == (int)LAYER_INDEX::PLAYER)
	{
		PlayerDamageByPercent(1.f);
	}
}

