#include "pch.h"
#include "CHeroSwordOfBurningSoulSkill.h"
#include "CHeroSwordOfBurningSoulScript.h"

CHeroSwordOfBurningSoulSkill::CHeroSwordOfBurningSoulSkill(UINT _Type)
	: CSkillScript(_Type)
	, m_Sword(nullptr)
{
}

CHeroSwordOfBurningSoulSkill::CHeroSwordOfBurningSoulSkill(const CHeroSwordOfBurningSoulSkill& _Other)
	: CSkillScript(_Other)
	, m_Sword(nullptr)
{
}

void CHeroSwordOfBurningSoulSkill::Tick()
{
	if (!IsStarted())
		return;

	if ((m_Sword->GetOwner()->GetParent() && GetElapsedTime() >= 20.f) ||
		(m_Sword->GetOwner()->GetParent() == nullptr && GetElapsedTime() >= 117.f))
	{
		End();
	}
}


CHeroSwordOfBurningSoulSkill::~CHeroSwordOfBurningSoulSkill()
{
}

void CHeroSwordOfBurningSoulSkill::Use()
{
	CSkillScript::Use();

	m_Sword->GetOwner()->SetActive(true);

	if (KEY_PRESSED(KEY::DOWN))
	{
		if (m_Sword->GetOwner()->GetParent())
			AddChild(nullptr, m_Sword->GetOwner());

		m_Sword->Transform()->SetRelativePos(Transform()->GetRelativePos() + Vector3(0.f, 80.f, 0.f));
		m_Sword->GetOwner()->Collider2D()->SetScale(Vector2(463.f, 230.f));
		m_Sword->GetOwner()->Collider2D()->SetOffset(Vector2(0.f, 30.f));
	}
	else
	{
		if (!m_Sword->GetOwner()->GetParent())
			AddChild(GetOwner(), m_Sword->GetOwner());

		m_Sword->Transform()->SetRelativePos(42.f, 89.f, 0.f);
		m_Sword->GetOwner()->Collider2D()->SetScale(Vector2(350.f, 200.f));
		m_Sword->GetOwner()->Collider2D()->SetOffset(Vector2(-160.f, 35.f));
	}

	m_Sword->Summon();
}

void CHeroSwordOfBurningSoulSkill::End()
{
	m_Sword->StateMachine()->ChangeState(L"DieState");
}

void CHeroSwordOfBurningSoulSkill::SkillKeyTap()
{
	Use();
}

void CHeroSwordOfBurningSoulSkill::Init()
{
}

void CHeroSwordOfBurningSoulSkill::Begin()
{
	CGameObject* pObj = new CGameObject;
	pObj->SetLayerIdx((int)LAYER_INDEX::DEFAULT);
	pObj->SetActive(false);

	m_Sword = new CHeroSwordOfBurningSoulScript;
	pObj->AddComponent(m_Sword);

	CreateObject(pObj, (int)LAYER_INDEX::DEFAULT, false);

}
