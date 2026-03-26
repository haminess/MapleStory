#include "pch.h"
#include "CScriptMgr.h"
#include "CSkillManager.h"
#include "CSkillScript.h"

CSkillManager::CSkillManager()
	: CScript(SCRIPT_TYPE::SKILLMANAGER)
{
}

CSkillManager::CSkillManager(const CSkillManager& _Other)
	: CScript(SCRIPT_TYPE::SKILLMANAGER)
{
}

CSkillManager::~CSkillManager()
{
}

void CSkillManager::Init()
{
	m_vecSkill.resize((UINT)KEY::END);
}

void CSkillManager::Tick()
{
	for (UINT i = 0; i < (UINT)KEY::END; ++i)
	{
		if (m_vecSkill[i] == nullptr)
			continue;

		if (KEY_TAP((KEY)i))
		{
			m_vecSkill[i]->SkillKeyTap();
		}
		else if (KEY_PRESSED((KEY)i))
		{
			m_vecSkill[i]->SkillKeyPressed();
		}
		else if (KEY_RELEASED((KEY)i))
		{
			m_vecSkill[i]->SkillKeyReleased();
		}
	}
}


void CSkillManager::RegisterSkill(KEY _Key, CSkillScript* _Skill)
{
	if (!_Skill)
		return;

	_Skill->SetCaster(GetOwner());
	m_vecSkill[(int)_Key] = _Skill;
}