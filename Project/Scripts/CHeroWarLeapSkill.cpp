#include "pch.h"
#include "CHeroWarLeapSkill.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CRenderMgr.h>

CHeroWarLeapSkill::CHeroWarLeapSkill(UINT _Type)
	: CSkillScript(_Type)
{
}

CHeroWarLeapSkill::CHeroWarLeapSkill(const CHeroWarLeapSkill& _Other)
	: CSkillScript(_Other)
{
}

CHeroWarLeapSkill::~CHeroWarLeapSkill()
{
}

void CHeroWarLeapSkill::Use()
{

	if (KEY_PRESSED(KEY::UP))
	{
		CRenderMgr::GetInst()->PlayAnimEff(m_UpEff, GetOwner(), Vector3(3.f, -90.f, 0.f));
		CRenderMgr::GetInst()->PlayAnimEff(m_TailEff, nullptr, GetOwner()->Transform()->GetWorldPos());

		GetOwner()->RigidBody2D()->AddImpulse(Vector2(0.f, 460.f));
	}
	else
	{
		CRenderMgr::GetInst()->PlayAnimEff(m_BasicEff, GetOwner(), Vector3(40.f, 29.f, 0.f));
		CRenderMgr::GetInst()->PlayAnimEff(m_TailEff, nullptr, GetOwner()->Transform()->GetWorldPos(), Vector3(GetOwner()->Transform()->GetRelativeRotation().y, 0.f, Radian(90.f)));

		if(GetOwner()->Transform()->GetRelativeRotation().y > 0 || KEY_PRESSED(KEY::RIGHT))
			GetOwner()->RigidBody2D()->AddImpulse(Vector2(1200.f, 0.f));
		else	
			GetOwner()->RigidBody2D()->AddImpulse(Vector2(-1200.f, 0.f));
	}
}

void CHeroWarLeapSkill::SkillKeyTap()
{
	if (!GetUser()->RigidBody2D()->IsGround() && m_Count > 0)
	{
		Use();
		m_Count = 0;
	}
	else
	{
		++m_Count;
	}
}

void CHeroWarLeapSkill::Init()
{
	m_BasicEff = FIND_ANIM(L"Flipbook\\WarLeap.flip");
	m_UpEff = FIND_ANIM(L"Flipbook\\WarLeap_Up.flip");
	m_TailEff = FIND_ANIM(L"Flipbook\\WarLeap_Tail.flip");
	//GetOwner()->FlipbookPlayer()->AddFlipbook(0, CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"Flipbook\\WarLeap.flip"));
}

void CHeroWarLeapSkill::Tick()
{
	if (GetOwner()->RigidBody2D()->IsGround())
	{
		m_Count = 0;
	}
}
