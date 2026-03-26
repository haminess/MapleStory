#include "pch.h"
#include "CMonsterScript.h"
#include "CParticleSystemScript.h"
#include "CIdleState.h"
#include "CAttackState.h"
#include "CTraceState.h"
#include "CPatrolState.h"
#include "CHitState.h"
#include "CDieState.h"
#include "CSpawnState.h"
#include "CPlatformerScript.h"
#include <Engine/CFontMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CTaskMgr.h>

CMonsterScript::CMonsterScript()
	: CLifeScript(SCRIPT_TYPE::MONSTERSCRIPT)
{
}

CMonsterScript::CMonsterScript(UINT _Type)
	: CLifeScript(_Type)
{
	for (int i = 0; i < (UINT)MON_ANIM::END; ++i)
	{
		//AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLIPBOOK, " ", &m_Flipbook[i] });
	}
}

CMonsterScript::~CMonsterScript()
{
}


void CMonsterScript::Hit(int _Power, int _Count, Ptr<CFlipbook> _HitEff, Vector2 _Dir)
{
	m_HitPower = _Power;
	m_HitAnim = _HitEff;

	for (int i = 0; i < _Count; ++i)
	{
		CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CMonsterScript::HitTimer, 0.2f * i);
	}

	RigidBody2D()->AddImpulse(_Dir);
}

void CMonsterScript::HitTimer()
{
	Damage(m_HitPower);

	if (m_HitAnim != nullptr)
		PlayHitAnim(m_HitAnim, Transform()->GetWorldPos(), 1);
}

void CMonsterScript::Damage(int _Power)
{
	CLifeScript::Damage(_Power);

	// 여기서 데미지스킨 출력***

	StateMachine()->ChangeState(L"HitState");
}

void CMonsterScript::Dead()
{
	CLifeScript::Dead();

	// 사망 애니메이션이 있는 경우 재생
	CState* pCurState = GetOwner()->StateMachine()->GetCurrentState();
	if (nullptr != pCurState)
	{
		GetOwner()->StateMachine()->ChangeState(L"DieState");
	}
}

void CMonsterScript::Init()
{
	GetOwner()->SetName(L"Monster");
	GetOwner()->SetLayerIdx((int)LAYER_INDEX::MONSTER);

	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	GetOwner()->Collider2D()->SetIndependentScale(true);
	GetOwner()->Collider2D()->SetScale(Vector2(5.f, 5.f));
	GetOwner()->StateMachine()->AddState(L"IdleState", new CIdleState);
	GetOwner()->StateMachine()->AddState(L"AttackState", new CAttackState);
	GetOwner()->StateMachine()->AddState(L"TraceState", new CTraceState);
	GetOwner()->StateMachine()->AddState(L"MoveState", new CPatrolState);
	GetOwner()->StateMachine()->AddState(L"HitState", new CHitState);
	GetOwner()->StateMachine()->AddState(L"SpawnState", new CSpawnState);
	GetOwner()->StateMachine()->AddState(L"DieState", new CDieState);
}

void CMonsterScript::Begin()
{
	for (int i = 0; i < (UINT)MON_ANIM::END; ++i)
	{
		if (nullptr != m_Flipbook[i])
			FlipbookPlayer()->AddFlipbook(i, m_Flipbook[i]);
	}

	m_Platformer = GetOwner()->GetScript<CPlatformerScript>();

	StateMachine()->ChangeState(L"IdleState");
}

void CMonsterScript::Tick()
{
	if (StateMachine()->GetCurrentState()->GetStateKey() != L"HitState")
	{
		RandomState();
		RandomDirection();

		if (m_Platformer && 
			m_Platformer->GetCurPlatform() && 
			m_Platformer->GetCurPlatform()->IsBlock())
		{
			float PlatX = m_Platformer->GetCurPlatform()->GetOwner()->Transform()->GetWorldPos().x;
			float MonsterX = GetOwner()->Transform()->GetWorldPos().x;
			float PlatformHalfWidth = m_Platformer->GetCurPlatform()->GetOwner()->Transform()->GetRelativeScale().x / 2.f - 50.f;

			// 플랫폼 왼쪽 끝에 도달
			if (MonsterX < PlatX - PlatformHalfWidth)
			{
				CPatrolState* pPatrol = (CPatrolState*)StateMachine()->FindState(L"MoveState");
				pPatrol->SetDirection(1);
				StateMachine()->ChangeState(L"MoveState");
			}
			// 플랫폼 오른쪽 끝에 도달
			else if (MonsterX > PlatX + PlatformHalfWidth)
			{
				CPatrolState* pPatrol = (CPatrolState*)StateMachine()->FindState(L"MoveState");
				pPatrol->SetDirection(-1);
				StateMachine()->ChangeState(L"MoveState");
			}
		}
	}

	// HP 띄우기
	Vector2 vPos = GetOwner()->Transform()->GetWorldPos();
	vPos.y += 100.f;
	vPos = CKeyMgr::GetWorldToScreen(vPos);
	wstring hp = std::to_wstring(GetHP()) + L" / " + std::to_wstring(GetMaxHP());
	CRenderMgr::GetInst()->AddText(hp.c_str(), vPos.x, vPos.y, 16, FONT_RGBA(255, 100, 100, 255));

}

void CMonsterScript::RandomState()
{
	// 시간 누적
	m_fAccTime += DT;

	// 일정 시간마다 상태 랜덤 변경
	if (m_fAccTime >= m_fStateChangeTime)
	{
		m_fAccTime = 0.f;

		m_fStateChangeTime = 1.0f + (rand() % 40) / 10.f;

		// 랜덤 상태 선택 (None과 END 제외)
		int randomState = rand() % ((UINT)MON_ANIM::END - 1);

		// 상태 변경
		switch ((MON_ANIM)randomState)
		{
		case MON_ANIM::IDLE:
			StateMachine()->ChangeState(L"IdleState");
			break;

		case MON_ANIM::ATTACK:
			//    StateMachine()->ChangeState(L"AttackState");
			//    break;

		case MON_ANIM::TRACE:
			//    StateMachine()->ChangeState(L"TraceState");
			//    break;
		case MON_ANIM::MOVE:
			StateMachine()->ChangeState(L"MoveState");
			CPatrolState* pPatrol = (CPatrolState*)StateMachine()->FindState(L"MoveState");
			pPatrol->SetDirection(RandomDirection());
			break;
		}
	}
}

int CMonsterScript::RandomDirection()
{
	if (rand() % 2)  // 50% 확률로
	{
		return GetOwner()->Transform()->GetRelativeRotation().y > 0.f ? 1 : -1;
	}
	else
	{
		return GetOwner()->Transform()->GetRelativeRotation().y > 0.f ? -1 : 1;
	}
}

