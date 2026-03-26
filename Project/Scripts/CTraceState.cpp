#include "pch.h"
#include "CTraceState.h"
#include "CMonsterScript.h"

CTraceState::CTraceState()
	: CState((UINT)SCRIPT_TYPE::TRACESTATE)
	, m_Return(false)
{
}

CTraceState::~CTraceState()
{
}

void CTraceState::Enter()
{
	GetStateMachine()->FlipbookPlayer()->Play((UINT)MON_ANIM::TRACE, 10, true);
}

void CTraceState::Tick()
{

	//if (false == m_Return)
	//{
	//	// Trace 상태의 몬스터를 알아낸다.
	//	CMonster* pMonster = GetOwner<CMonster>();
	//	CRigidBody* pRigid = pMonster->GetComponent<CRigidBody>();

	//	// Player 를 추적한다.
	//	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();

	//	Vector2 vDir = pPlayer->GetPos() - pMonster->GetPos();
	//	vDir.Normalize();

	//	//Vector2 vPos = pMonster->GetPos() + vDir * pMonster->GetMonsterInfo().Speed * DT;
	//	//pMonster->SetPos(vPos);
	//	pRigid->AddVelocity(vDir * pMonster->GetMonsterInfo().Speed * DT);

	//	// 만약 Player 가 탐지범위 이상으로 멀어지면 
	//	// or 추적을 시작한 이후로 일정시간이 지난 경우
	//	// or 초기위치에서 일정거리 이상 추격한경우
	//	float Distance = pMonster->GetPos().Distance(pPlayer->GetPos());
	//	if (pMonster->GetMonsterInfo().DetectRange < Distance)
	//	{
	//		m_Return = true;
	//	}
	//}

	//else
	//{
	//	// 원래 위치로 돌아간다.
	//	CMonster* pMonster = GetOwner<CMonster>();
	//	Vector2 vDir = m_InitPos - pMonster->GetPos();
	//	vDir.Normalize();
	//	Vector2 vNewPos = pMonster->GetPos() + vDir * pMonster->GetMonsterInfo().Speed * 2.f * DT;
	//	pMonster->SetPos(vNewPos);

	//	// 초기위치로 완전히 되돌아오면 IdleState 로 변경
	//	if (pMonster->GetPos().Distance(m_InitPos) < 1)
	//	{
	//		pMonster->SetPos(m_InitPos);
	//		GetStateMachine()->ChangeState(L"IdleState");
	//	}
	//}
}

void CTraceState::Exit()
{
	/*LOG(LOG_LEVEL::WARNING, L"Exit TraceState");
	LOG(LOG_LEVEL::WARNING, L"Exit TraceState");
	m_Return = false;*/
}
