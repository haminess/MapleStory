#include "pch.h"
#include "CHitState.h"
#include "CMonsterScript.h"

CHitState::CHitState()
	: CState((UINT)SCRIPT_TYPE::HITSTATE)
	, m_ExitTime(3.f)
	, m_ElapsedTime(0.f)
{
}

CHitState::~CHitState()
{
}

void CHitState::Enter()
{
	GetStateMachine()->FlipbookPlayer()->Play((UINT)MON_ANIM::HIT, 10, true);
}

void CHitState::Tick()
{
	m_ElapsedTime += DT;
	if (m_ExitTime < m_ElapsedTime)
	{
		m_ElapsedTime = 0.f;
		GetStateMachine()->ChangeState(L"IdleState");
	}
}

void CHitState::Exit()
{
}
