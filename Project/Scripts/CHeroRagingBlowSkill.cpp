#include "pch.h"
#include "CHeroRagingBlowSkill.h"
#include "CMonsterScript.h"
#include "CProjectileScript.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CTaskMgr.h>

CHeroRagingBlowSkill::CHeroRagingBlowSkill(UINT _Type)
	: CSkillScript(_Type)
	, m_Step(3)
	, m_MaxMob(8)
	, m_AttackCount(4)
{
	AddScriptParam({SCRIPT_PARAM::INT, "Step", &m_Step});
}

CHeroRagingBlowSkill::CHeroRagingBlowSkill(const CHeroRagingBlowSkill& _Other)
	: CSkillScript(_Other)
{
	AddScriptParam({ SCRIPT_PARAM::INT, "Step", &m_Step });
}

CHeroRagingBlowSkill::~CHeroRagingBlowSkill()
{
}

void CHeroRagingBlowSkill::Use()
{
	if (IsStarted())
		return;

	CSkillScript::Use();

	const Vector3 pos[4] = {Vector3(0.f, 67.f, 0.f), Vector3(92.f, 44.f, 0.f), Vector3(-14.f, 86.f, 0.f), Vector3(-40.f, 36.f, 0.f)};
	const Vector2 offset[4] = { Vector2(-144.f, -9.f), Vector2(-116.f, 11.f), Vector2(-67.f, 11.f), Vector2(-84.f, 11.f)};
	const Vector2 scale[4] = { Vector2(330.f, 250.f), Vector2(523.f, 314.f), Vector2(462.f, 314.f), Vector2(567.f, 314.f)};

	GetOwner()->StateMachine()->ChangeState(L"AttackState");
	GetOwner()->FlipbookPlayer()->Play(7, 5.f, false);

	m_RagingBlow->SetActive(true);
	m_RagingBlow->Transform()->SetRelativePos(pos[m_Step]);
	m_RagingBlow->Collider2D()->SetOffset(offset[m_Step]);
	m_RagingBlow->Collider2D()->SetScale(scale[m_Step]);
	m_RagingBlow->FlipbookPlayer()->Play(m_Step, 20.f, false);

	CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CHeroRagingBlowSkill::End, 1.f);
}

void CHeroRagingBlowSkill::End()
{
	CSkillScript::End();

	GetOwner()->StateMachine()->ChangeState(L"IdleState");
	m_RagingBlow->SetActive(false);
}

void CHeroRagingBlowSkill::SkillKeyTap()
{
	Use();
}

void CHeroRagingBlowSkill::SkillKeyPressed()
{
	if (GetOwner()->FlipbookPlayer()->IsFinishedOnce())
		Use();
}

void CHeroRagingBlowSkill::SkillKeyReleased()
{
	GetOwner()->FlipbookPlayer()->SetLoop(false);
}

void CHeroRagingBlowSkill::Init()
{
	m_AttackFlip[0] = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\HeroRagingBlow.flip");
	m_AttackFlip[1] = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\HeroRagingBlow2.flip");
	m_AttackFlip[2] = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\HeroRagingBlow3.flip");
	m_AttackFlip[3] = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\HeroRagingBlow4.flip");
	m_HitFlip[0] = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\HeroRagingBlow_Hit.flip");
	m_HitFlip[1] = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\HeroRagingBlow2_Hit.flip");
	m_HitFlip[2] = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\HeroRagingBlow3_Hit.flip");
	m_HitFlip[3] = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\HeroRagingBlow4_Hit.flip");

}

void CHeroRagingBlowSkill::Begin()
{
	m_RagingBlow = CreateAnimObject();
	m_RagingBlow->SetName(L"RagingBlow");
	m_RagingBlow->SetLayerIdx((int)LAYER_INDEX::PLAYER_OBJECT);
	m_RagingBlow->SetActive(false);

	m_RagingBlow->Transform()->UseIndependentScale(true);
	m_RagingBlow->Transform()->SetRelativePos(0.f, 67.f, 0.f);

	m_RagingBlow->AddComponent(new CCollider2D);
	m_RagingBlow->Collider2D()->SetIndependentScale(true);
	m_RagingBlow->Collider2D()->SetOffset(Vector2(-144.f, -9.f));
	m_RagingBlow->Collider2D()->SetScale(Vector2(330.f, 250.f));

	m_RagingBlow->FlipbookPlayer()->AddFlipbook(0, m_AttackFlip[0]);
	m_RagingBlow->FlipbookPlayer()->AddFlipbook(1, m_AttackFlip[1]);
	m_RagingBlow->FlipbookPlayer()->AddFlipbook(2, m_AttackFlip[2]);
	m_RagingBlow->FlipbookPlayer()->AddFlipbook(3, m_AttackFlip[3]);

	CProjectileScript* pProj = new CProjectileScript;
	pProj->BindDelegate(this, (SCRIPT_DELEGATE_2)&CHeroRagingBlowSkill::HitMonster, nullptr, nullptr);
	m_RagingBlow->AddComponent(pProj);

	GetOwner()->AddChild(m_RagingBlow);
}

void CHeroRagingBlowSkill::Tick()
{
}

void CHeroRagingBlowSkill::HitMonster(DWORD_PTR _Col, DWORD_PTR _Other)
{
	CCollider2D* pMy = (CCollider2D*)_Col;
	CCollider2D* pOther = (CCollider2D*)_Other;
	if (pOther->GetOwner()->GetLayerIdx() == (int)LAYER_INDEX::MONSTER)
	{
		CMonsterScript* pMonster = pOther->GetOwner()->GetScript<CMonsterScript>();
		if (pMonster == nullptr)
			return;

		pMonster->Hit(12345678, 4, m_HitFlip[m_Step], GetKnockbackDir(pOther, pMy, 100.f));
	}
}
