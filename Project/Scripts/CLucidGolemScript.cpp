#include "pch.h"
#include "CLucidGolemScript.h"
#include "CPlayerScript.h"
#include "CPlatformerScript.h"

#include "CSpawnState.h"
#include "CIdleState.h"
#include "CPatrolState.h"
#include "CHitState.h"
#include "CDieState.h"

#include <Engine/CLevelMgr.h>

CLucidGolemScript::CLucidGolemScript()
	: CMonsterScript((UINT)SCRIPT_TYPE::LUCIDGOLEMSCRIPT)
{
}

CLucidGolemScript::~CLucidGolemScript()
{
}

void CLucidGolemScript::Init()
{
	GetOwner()->SetName(L"Golem");
	GetOwner()->SetLayerIdx((int)LAYER_INDEX::MONSTER);

	GetOwner()->AddComponent(new CMeshRender);
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	GetOwner()->AddComponent(new CCollider2D);
	GetOwner()->Collider2D()->SetIndependentScale(true);
	GetOwner()->Collider2D()->SetScale(Vector2(100.f, 222.f));

	GetOwner()->AddComponent(new CRigidBody2D);
	GetOwner()->RigidBody2D()->UseGravity(true);

	GetOwner()->AddComponent(new CFlipbookPlayer);
	GetOwner()->FlipbookPlayer()->AddFlipbook((int)MON_ANIM::SPAWN, FIND_ANIM(L"Flipbook\\NightmareGolem_Regen.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook((int)MON_ANIM::IDLE, FIND_ANIM(L"Flipbook\\NightmareGolem_Stand.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook((int)MON_ANIM::ATTACK, FIND_ANIM(L"Flipbook\\NightmareGolem_Attack.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook((int)MON_ANIM::HIT, FIND_ANIM(L"Flipbook\\NightmareGolem_Hit.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook((int)MON_ANIM::DIE, FIND_ANIM(L"Flipbook\\NightmareGolem_Die.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(1, FIND_ANIM(L"Flipbook\\NightmareGolem_Attack1.flip"));
	m_HitAnim = FIND_ANIM(L"Flipbook\\NightmareGolem_AttackHit.flip").Get();

	GetOwner()->AddComponent(new CStateMachine);
	GetOwner()->StateMachine()->AddState(L"RegenState", new CLucidGolemState_Spawn);
	GetOwner()->StateMachine()->AddState(L"DropState", new CLucidGolemState_Drop);
	GetOwner()->StateMachine()->AddState(L"IdleState", new CIdleState);
	GetOwner()->StateMachine()->AddState(L"HitState", new CHitState);
	GetOwner()->StateMachine()->AddState(L"DieState", new CDieState);
	GetOwner()->StateMachine()->ChangeState(L"RegenState");


	GetOwner()->AddComponent(new CPlatformerScript);
}

void CLucidGolemScript::Begin()
{
	StateMachine()->ChangeState(L"RegenState");
}

void CLucidGolemScript::Tick()
{
}

void CLucidGolemScript::BeginOverlap(CCollider2D* _Col, CGameObject* _OtherObj, CCollider2D* _OtherCol)
{
	if (_OtherObj->GetLayerIdx() == (int)LAYER_INDEX::PLAYER)
	{
		PlayerDamageByPercent(0.5f);
		PlayHitAnim(m_HitAnim, GetRandomPosAround(_OtherObj->Transform()->GetWorldPos()), 1);

		m_PrevPos = _OtherObj->Transform()->GetWorldPos().x;
	}
}

void CLucidGolemScript::Overlap(CCollider2D* _Col, CGameObject* _OtherObj, CCollider2D* _OtherCol)
{
	if (_OtherObj->GetLayerIdx() == (int)LAYER_INDEX::PLAYER)
	{
		float CurPos = CLevelMgr::GetInst()->GetCurrentPlayer()->Transform()->GetWorldPos().x;
		float MyPos = Transform()->GetWorldPos().x;

		// 플레이어가 옆으로 지나감
		if ((MyPos - m_PrevPos) * (MyPos - CurPos) < 0)
		{
			int a = 0;
			PlayerDamageByPercent(0.5f);
		}

		m_PrevPos = CurPos;
	}
}
