#include "pch.h"
#include "CHeroComboAttackSkill.h"

#include <Engine/CRenderMgr.h>

CHeroComboAttackSkill::CHeroComboAttackSkill(UINT _Type)
	: CSkillScript(_Type)
{
	AddScriptParam({ SCRIPT_PARAM::INT, "Combo", &m_Combo });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Update", this, (SCRIPT_DELEGATE)&CHeroComboAttackSkill::UpdateCombo });
}

CHeroComboAttackSkill::CHeroComboAttackSkill(const CHeroComboAttackSkill& _Other)
	: CSkillScript(_Other)
{
	AddScriptParam({ SCRIPT_PARAM::INT, "Combo", &m_Combo });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Update", this, (SCRIPT_DELEGATE)&CHeroComboAttackSkill::UpdateCombo });
}

CHeroComboAttackSkill::~CHeroComboAttackSkill()
{
}

void CHeroComboAttackSkill::ClearCombo()
{
	for (int i = 0; i < 5; ++i)
	{
		m_vecCombo[i]->SetActive(false);
	}
}

void CHeroComboAttackSkill::ActivateCombo(int _Combo)
{
	ClearCombo();

	m_Combo = _Combo;
	if (_Combo == 0)
		return;

		
	// combo attack
	int cnt = std::clamp(m_Combo, 0, 5);
	for (int i = 0; i < cnt; ++i)
	{
		m_vecCombo[i]->SetActive(true);
		m_vecCombo[i]->FlipbookPlayer()->Play(0, 10.f, true);
	}

	// advanced combo
	if (5 < m_Combo)
	{
		if (m_Back->FlipbookPlayer()->GetCurFlipbook() != m_Back->FlipbookPlayer()->GetFlipbook(3))
			m_Back->FlipbookPlayer()->Play(3, 10.f, true);

		int cnt2 = std::clamp(m_Combo - 5, 0, 5);
		for (int i = 0; i < cnt2; ++i)
		{
			m_vecCombo[i]->SetActive(true);
			m_vecCombo[i]->FlipbookPlayer()->Play(1, 10.f, true);
		}
	}

	if(m_Combo <= 5)
		CRenderMgr::GetInst()->PlayAnimEff(m_ComboOnAnim[0], m_vecCombo[m_Combo - 1], Vector3(), Vector3(), (int)LAYER_INDEX::EFFECT);
	else if(m_Combo <= 10)
		CRenderMgr::GetInst()->PlayAnimEff(m_ComboOnAnim[1], m_vecCombo[(m_Combo-1)%5], Vector3(), Vector3(), (int)LAYER_INDEX::EFFECT);
}

void CHeroComboAttackSkill::UpdateCombo()
{
	ActivateCombo(m_Combo);
}

void CHeroComboAttackSkill::Init()
{
	m_ComboOnAnim[0] = FIND_ANIM(L"Flipbook\\HeroComboAttack_Start.flip");
	m_ComboOnAnim[1] = FIND_ANIM(L"Flipbook\\HeroAdvancedCombo_Start.flip");
}

void CHeroComboAttackSkill::Begin()
{
	m_Back = CreateAnimObject();
	m_Back->SetName(L"Combo Attack");
	m_Back->SetLayerIdx((int)LAYER_INDEX::OBJECT);
	m_Back->SetActive(false);
	m_Back->Transform()->UseIndependentScale(true);
	m_Back->Transform()->SetRelativePos(7.f, 42.f, 0.f);
	m_Back->FlipbookPlayer()->AddFlipbook(0, FIND_ANIM(L"Flipbook\\HeroComboAttack_Back.flip"));
	m_Back->FlipbookPlayer()->AddFlipbook(1, FIND_ANIM(L"Flipbook\\HeroComboAttack_BackLoop.flip"));
	m_Back->FlipbookPlayer()->AddFlipbook(2, FIND_ANIM(L"Flipbook\\HeroAdvancedCombo_Back.flip"));
	m_Back->FlipbookPlayer()->AddFlipbook(3, FIND_ANIM(L"Flipbook\\HeroAdvancedCombo_BackLoop.flip"));
	m_Back->FlipbookPlayer()->Play(0, 10.f, true);
	GetOwner()->AddChild(m_Back);

	for (int i = 0; i < 5; ++i)
	{
		CGameObject* pCombo = CreateAnimObject();
		pCombo->SetName(L"Combo Item");
		pCombo->SetLayerIdx((int)LAYER_INDEX::PLAYER_OBJECT_1);
		pCombo->SetActive(false);
		pCombo->Transform()->UseIndependentScale(true);
		pCombo->FlipbookPlayer()->AddFlipbook(0, FIND_ANIM(L"Flipbook\\HeroComboAttack_" + std::to_wstring(i + 1) + L".flip"));
		pCombo->FlipbookPlayer()->AddFlipbook(1, FIND_ANIM(L"Flipbook\\HeroAdvancedCombo_" + std::to_wstring(i + 1) + L".flip"));
		pCombo->FlipbookPlayer()->Play(0, 10.f, true);

		m_vecCombo.push_back(pCombo);
		CreateObject(pCombo, (int)LAYER_INDEX::PLAYER_OBJECT_1, false);
	}
}

void CHeroComboAttackSkill::Tick()
{
	// 회전 돌리기
	float r = 50.f;
	m_Rotator += XM_PI * DT;
	for (int i = 0; i < 5; ++i)
	{
		m_vecCombo[i]->Transform()->SetRelativePos(m_Back->Transform()->GetWorldPos() + Vector3(r * cosf(m_Rotator + 2 * XM_PI / 5.f * i), r * sinf(m_Rotator + 2 * XM_PI / 5.f * i), 0.f));
	}

	if (m_Rotator > 2 * XM_PI)
	{
		m_Rotator -= 2 * XM_PI;
	}
}

void CHeroComboAttackSkill::Use()
{
	m_Back->SetActive(true);
	m_Back->FlipbookPlayer()->Play(1, 10.f, true);

	UpdateCombo();
}

void CHeroComboAttackSkill::End()
{
	m_Back->SetActive(false);
	for (int i = 0; i < 5; ++i)
	{
		m_vecCombo[i]->SetActive(false);
	}
}

