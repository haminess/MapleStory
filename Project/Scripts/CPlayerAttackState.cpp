#include "pch.h"
#include "CPlayerAttackState.h"

CPlayerAttackState::CPlayerAttackState()
	: CState(SCRIPT_TYPE::PLAYERATTACKSTATE)
{
}

CPlayerAttackState::~CPlayerAttackState()
{
}

void CPlayerAttackState::Enter()
{
	GetStateMachine()->GetOwner()->GetChild()[0]->FlipbookPlayer()->Play(6, 3.f, false);
}

void CPlayerAttackState::Tick()
{
	if (!GetStateMachine()->GetOwner()->GetChild()[0]->FlipbookPlayer()->IsPlaying())
		GetStateMachine()->ChangeState(L"Idle");
}

void CPlayerAttackState::Exit()
{
	GetStateMachine()->GetOwner()->GetChild()[0]->FlipbookPlayer()->Play(1, 3.f, true);
}
