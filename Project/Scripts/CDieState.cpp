#include "pch.h"
#include "CDieState.h"
#include "CMonsterScript.h"

CDieState::CDieState()
	: CState(SCRIPT_TYPE::DIESTATE)
{
}

CDieState::~CDieState()
{
}

void CDieState::Enter()
{
	GetStateMachine()->FlipbookPlayer()->Play((int)MON_ANIM::DIE, 5, false);
}

void CDieState::Tick()
{
	if (!GetStateMachine()->FlipbookPlayer()->IsPlaying())
	{
		Exit();
	}
}

void CDieState::Exit()
{
	DestroyObject(GetStateMachine()->GetOwner());
}

