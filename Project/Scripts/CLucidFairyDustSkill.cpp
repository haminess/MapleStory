#include "pch.h"
#include "CLucidFairyDustSkill.h"
#include "CProjectileScript.h"
#include "CLucidScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CTaskMgr.h>

CLucidFairyDustSkill::CLucidFairyDustSkill(UINT _Type)
	: CSkillScript(_Type)
	, m_MaxTime(12.f)
	, m_Speed(120.f)
{
}

CLucidFairyDustSkill::CLucidFairyDustSkill(const CLucidFairyDustSkill& _Other)
	: CSkillScript(_Other)
	, m_MaxTime(12.f)
	, m_Speed(120.f)
{
}

CLucidFairyDustSkill::~CLucidFairyDustSkill()
{
}

void CLucidFairyDustSkill::Init()
{
	m_LucidAnim = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\Lucid_FairyDust.flip");
	m_ProjectileAnim = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\FairyDust.flip");

	m_Lucid = GetOwner()->GetScript<CLucidScript>();
}

void CLucidFairyDustSkill::Begin()
{
	for (int i = 0; i < 3; ++i)
	{
		CProjectileScript* pScript = CreateProjectile();
		pScript->SetName(L"FairyDust Projectile");
		CreateObject(pScript->GetOwner(), (int)LAYER_INDEX::MONSTER_OBJECT, true);

		pScript->GetOwner()->Collider2D()->SetScale(Vector2(0.2f, 0.6f));
		pScript->GetOwner()->Collider2D()->SetOffset(Vector2(-0.15f, 0.f));

		pScript->GetOwner()->FlipbookPlayer()->AddFlipbook(0, FIND_ANIM(L"Flipbook\\FairyDust" + std::to_wstring(i) + L".flip"));
		pScript->GetOwner()->FlipbookPlayer()->Play(0, 10, true);
		pScript->GetOwner()->SetActive(false);

		pScript->BindDelegate(this, (SCRIPT_DELEGATE_2)&CLucidFairyDustSkill::ProjectileBeginOverlap, nullptr, nullptr);

		m_Projectile.push_back(pScript->GetOwner());
	}
}

void CLucidFairyDustSkill::Use()
{
	CSkillScript::Use();

	// 스킬 시전자
	if (nullptr == GetUser())
		return;

	GetUser()->FlipbookPlayer()->Play(1, 10.f, false);

	CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CLucidFairyDustSkill::SpawnFairyDust, 2.3f);
}

void CLucidFairyDustSkill::Tick()
{
	if (!m_IsStarted)
		return;

	// Fairy Dust 이동
	for (int i = 0; i < 3; ++i)
	{
		float rotZ = m_Projectile[i]->Transform()->GetRelativeRotation().z; // rad
		Vector3 vDir = Vector3(cosf(rotZ), sinf(rotZ), 0.f);

		Vector3 vMovePos = m_Projectile[i]->Transform()->GetRelativePos();
		vMovePos -= vDir * m_Speed * DT;
		m_Projectile[i]->Transform()->SetRelativePos(vMovePos);
	}

	// 타이머
	m_CurTime -= DT;
	if (m_CurTime < 0.f)
	{
		m_CurTime = m_MaxTime;
		m_IsStarted = false;

		m_Projectile[0]->SetActive(false);
		m_Projectile[1]->SetActive(false);
		m_Projectile[2]->SetActive(false);
	}

}

void CLucidFairyDustSkill::SpawnFairyDust()
{
	m_CurTime = m_MaxTime;

	const int RandIdx = RandomRange(0, 4);
	const float rotDeg[15] = {
		25, 12, 20, 12, 20,
		30, 25, 25, 25, 25,
		45, 35, 25, 35, 25
	};

	for (int i = 0; i < 3; ++i)
	{
		m_Projectile[i]->SetActive(true);
		m_Projectile[i]->Transform()->SetWorldPos(GetUser()->Transform()->GetWorldPos());
		m_Projectile[i]->FlipbookPlayer()->Play(0, 10, true);
		m_Projectile[i]->Transform()->SetRelativeRotation(0, 0, rotDeg[i * 5 + RandIdx]);
	}

	m_IsStarted = true;
}

void CLucidFairyDustSkill::ProjectileBeginOverlap(DWORD_PTR _Collider, DWORD_PTR _OtherCollider)
{
	CCollider2D* Other = (CCollider2D*)_OtherCollider;
	if (Other->GetOwner()->GetLayerIdx() == (int)LAYER_INDEX::PLAYER)
	{
		m_Lucid->SpawnLittleButterfly();
	}
}
