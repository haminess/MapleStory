#include "pch.h"
#include "CSkillState.h"

CSkillState::CSkillState()
	: CState((UINT)SCRIPT_TYPE::SKILLSTATE)
{
}

CSkillState::CSkillState(const CSkillState& _Other)
	: CState(_Other)
{
}

CSkillState::~CSkillState()
{
}

void CSkillState::Enter()
{
}

void CSkillState::Tick()
{
}

void CSkillState::Exit()
{
}
