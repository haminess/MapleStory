#include "pch.h"
#include "CSpawnState.h"
#include "CMonsterScript.h"

CSpawnState::CSpawnState()
	: CState((UINT)SCRIPT_TYPE::SPAWNSTATE)
{
}

CSpawnState::~CSpawnState()
{
}

void CSpawnState::Enter()
{
	GetStateMachine()->FlipbookPlayer()->Play((UINT)MON_ANIM::SPAWN, 10, false);
}

void CSpawnState::Tick()
{
	if (GetStateMachine()->FlipbookPlayer()->IsFinishedOnce())
		GetStateMachine()->ChangeState(L"IdleState");
}

void CSpawnState::Exit()
{

}
