#include "pch.h"
#include "CLucidToadstoolScript.h"
#include "CProjectileScript.h"
#include "CPlatformerScript.h"
#include "CPlayerScript.h"
#include "CSpawnState.h"
#include "CIdleState.h"
#include "CPatrolState.h"
#include "CHitState.h"
#include "CDieState.h"
#include "CAttackState.h"

CLucidToadstoolScript::CLucidToadstoolScript()
	: CMonsterScript((UINT)SCRIPT_TYPE::LUCIDTOADSTOOLSCRIPT)
{
}

CLucidToadstoolScript::CLucidToadstoolScript(const CLucidToadstoolScript& _Other)
	: CMonsterScript(_Other)
{
}

CLucidToadstoolScript::~CLucidToadstoolScript()
{
}


void CLucidToadstoolScript::Init()
{
	GetOwner()->SetName(L"Toadstool");
	GetOwner()->SetLayerIdx((int)LAYER_INDEX::MONSTER);

	GetOwner()->AddComponent(new CMeshRender);
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	GetOwner()->AddComponent(new CCollider2D);
	GetOwner()->Collider2D()->SetIndependentScale(true);
	GetOwner()->Collider2D()->SetScale(Vector2(90.f, 278.f));

	GetOwner()->AddComponent(new CRigidBody2D);
	GetOwner()->RigidBody2D()->UseGravity(true);

	GetOwner()->AddComponent(new CFlipbookPlayer);
	GetOwner()->FlipbookPlayer()->AddFlipbook((int)MON_ANIM::SPAWN, FIND_ANIM(L"Flipbook\\NightmareToadstool_Regen.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook((int)MON_ANIM::IDLE, FIND_ANIM(L"Flipbook\\NightmareToadstool_Stand.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook((int)MON_ANIM::MOVE, FIND_ANIM(L"Flipbook\\NightmareToadstool_Move.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook((int)MON_ANIM::ATTACK, FIND_ANIM(L"Flipbook\\NightmareToadstool_Attack.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook((int)MON_ANIM::HIT, FIND_ANIM(L"Flipbook\\NightmareToadstool_Hit.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook((int)MON_ANIM::DIE, FIND_ANIM(L"Flipbook\\NightmareToadstool_Die.flip"));

	GetOwner()->AddComponent(new CStateMachine);
	GetOwner()->StateMachine()->AddState(L"RegenState", new CSpawnState);
	GetOwner()->StateMachine()->AddState(L"IdleState", new CIdleState);
	GetOwner()->StateMachine()->AddState(L"MoveState", new CPatrolState);
	GetOwner()->StateMachine()->AddState(L"AttackState", new CAttackState);
	GetOwner()->StateMachine()->AddState(L"HitState", new CHitState);
	GetOwner()->StateMachine()->AddState(L"DieState", new CDieState);
	GetOwner()->StateMachine()->ChangeState(L"RegenState");


	GetOwner()->AddComponent(new CPlatformerScript);
}

void CLucidToadstoolScript::Begin()
{
	GetOwner()->StateMachine()->ChangeState(L"RegenState");
}

void CLucidToadstoolScript::Tick()
{
	CMonsterScript::Tick();
}

void CLucidToadstoolScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == (int)LAYER_INDEX::PLAYER)
	{
		PlayerDamageByPercent(0.5f);
	}
}

void CLucidToadstoolScript::SaveComponent(FILE* _File)
{
}

void CLucidToadstoolScript::LoadComponent(FILE* _FILE)
{
}
