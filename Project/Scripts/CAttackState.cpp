#include "pch.h"
#include "CAttackState.h"
#include "CMonsterScript.h"

CAttackState::CAttackState()
	: CState((UINT)SCRIPT_TYPE::ATTACKSTATE)
{
}

CAttackState::~CAttackState()
{
}

void CAttackState::Enter()
{
	GetStateMachine()->FlipbookPlayer()->Play((UINT)MON_ANIM::ATTACK, 10, true);
}

void CAttackState::Tick()
{
}

void CAttackState::Exit()
{
}
