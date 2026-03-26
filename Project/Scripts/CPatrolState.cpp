#include "pch.h"
#include "CPatrolState.h"
#include "CMonsterScript.h"

CPatrolState::CPatrolState()
	: CState(SCRIPT_TYPE::PATROLSTATE)
	, m_fPatrolTime(0.f)
	, m_fPatrolDuration(0.f)
	, m_fMovementSpeed(50.f)
	, m_iDirection(1)
{
}

CPatrolState::~CPatrolState()
{
}


void CPatrolState::Enter()
{
	GetStateMachine()->FlipbookPlayer()->Play((UINT)MON_ANIM::MOVE, 10, true);
}

void CPatrolState::Tick()
{
    m_fPatrolTime += DT;

    Vector3 velo = GetStateMachine()->GetOwner()->RigidBody2D()->GetVelocity();
    GetStateMachine()->GetOwner()->RigidBody2D()->SetVelocity(Vector3(m_fMovementSpeed * m_iDirection, velo.y, velo.z));

    // 이동 방향에 따라 스프라이트 방향 설정
    GetStateMachine()->GetOwner()->Transform()->SetRelativeScale(Vector3(-abs(GetStateMachine()->GetOwner()->Transform()->GetRelativeScale().x) * m_iDirection,
        GetStateMachine()->GetOwner()->Transform()->GetRelativeScale().y,
        GetStateMachine()->GetOwner()->Transform()->GetRelativeScale().z));
}

void CPatrolState::Exit()
{
}

