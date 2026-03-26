#pragma once
#include "CMonsterScript.h"
class CLucidGolemScript :
	public CMonsterScript
{
private:
	// 플레이어 이전 위치
	float m_PrevPos;	

	CFlipbook* m_HitAnim;

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void BeginOverlap(CCollider2D* _Col, CGameObject* _OtherObj, CCollider2D* _OtherCol) override;
	virtual void Overlap(CCollider2D* _Col, CGameObject* _OtherObj, CCollider2D* _OtherCol) override;

public:
	CLONE(CLucidGolemScript);
	CLucidGolemScript();
	~CLucidGolemScript();
};


#include <Engine/CState.h>
class CLucidGolemState_Spawn :
	public CState
{
public:
	virtual void Enter() override
	{
		GetStateMachine()->FlipbookPlayer()->Play((int)MON_ANIM::SPAWN, 10.f, false);
		GetStateMachine()->RigidBody2D()->UseGravity(false);
	}
	virtual void Tick() override
	{
		if (GetStateMachine()->FlipbookPlayer()->IsFinishedOnce())
			GetStateMachine()->ChangeState(L"DropState");
	}
	virtual void Exit() override {}

public:
	CLONE(CLucidGolemState_Spawn)
		CLucidGolemState_Spawn() : CState((UINT)SCRIPT_TYPE::LUCIDGOLEMSTATE_SPAWN) {}
	~CLucidGolemState_Spawn() {}
};

class CLucidGolemState_Drop :
	public CState
{
public:
	virtual void Enter() override
	{
		GetStateMachine()->FlipbookPlayer()->Play(1, 10.f, false);
		GetStateMachine()->RigidBody2D()->UseGravity(true);
	}
	virtual void Tick() override
	{
		if (GetStateMachine()->FlipbookPlayer()->IsFinishedOnce())
			GetStateMachine()->ChangeState(L"IdleState");
	}
	virtual void Exit() override {}

public:
	CLONE(CLucidGolemState_Drop)
		CLucidGolemState_Drop() : CState((UINT)SCRIPT_TYPE::LUCIDGOLEMSTATE_DROP) {}
	~CLucidGolemState_Drop() {}
};
