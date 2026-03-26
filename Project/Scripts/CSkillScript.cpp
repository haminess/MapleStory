#include "pch.h"
#include "CSkillScript.h"

CSkillScript::CSkillScript(UINT Type)
	: CScript(Type)
	, m_SkillUser(nullptr)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "MP", &m_ManaCost });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Cool Time", &m_CoolTime });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FUNCTION, "Use##"+to_string(GetID()), this, (SCRIPT_DELEGATE)&CSkillScript::Use});
}

CSkillScript::CSkillScript(const CSkillScript& _Other)
	: CScript(_Other)
	, m_SkillUser(_Other.m_SkillUser)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "MP", &m_ManaCost });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Cool Time", &m_CoolTime });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FUNCTION, "Use##"+ to_string(GetID()), this, (SCRIPT_DELEGATE)&CSkillScript::Use});
}

CSkillScript::~CSkillScript()
{
}

void CSkillScript::SaveComponent(FILE* _File)
{
}

void CSkillScript::LoadComponent(FILE* _File)
{
}

void CSkillScript::Tick()
{
	if (m_DelInst && m_DelTick)
		(m_DelInst->*m_DelTick)();
}
