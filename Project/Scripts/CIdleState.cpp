#include "pch.h"
#include "CIdleState.h"
#include "CMonsterScript.h"

CIdleState::CIdleState()
	: CState((UINT)SCRIPT_TYPE::IDLESTATE)
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::Enter()
{
	GetStateMachine()->FlipbookPlayer()->Play((UINT)MON_ANIM::IDLE, 10, true);
}

void CIdleState::Tick()
{
}

void CIdleState::Exit()
{
}
