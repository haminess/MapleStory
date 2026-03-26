#pragma once
#include <Engine/CScript.h>
class CHeroSwordOfBurningSoulScript :
	public CScript
{
private:
	Ptr<CFlipbook> m_HitAnim[2];

public:
	void Summon();
	void Attack();

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

public:
	CLONE(CHeroSwordOfBurningSoulScript);
	CHeroSwordOfBurningSoulScript();
	~CHeroSwordOfBurningSoulScript();
};

#include <Engine\CState.h>

class CHeroSOBSState_Regen
	: public CState
{
private:

public:
	virtual void Enter() override 
	{ 
		GetStateMachine()->FlipbookPlayer()->Play(0, 10.f, false); 
	}
	virtual void Tick() override 
	{ 
		if (GetStateMachine()->FlipbookPlayer()->IsFinishedOnce()) 
			GetStateMachine()->ChangeState(L"IdleState"); 
	}
	virtual void Exit() override {}

public:
	CLONE(CHeroSOBSState_Regen);
	CHeroSOBSState_Regen() : CState((UINT)SCRIPT_TYPE::HEROSOBSSTATE_REGEN) {}
	~CHeroSOBSState_Regen() {}
};

class CHeroSOBSState_Idle
	: public CState
{
private:
	float m_Timer;

public:
	virtual void Enter() override
	{
		GetStateMachine()->FlipbookPlayer()->Play(1, 10.f, true);
		m_Timer = 0.f;
	}
	virtual void Tick() override 
	{
		m_Timer += DT;
		if(m_Timer >= 1.f)
			GetStateMachine()->ChangeState(L"AttackState");
	}
	virtual void Exit() override 
	{
	}

public:
	CLONE(CHeroSOBSState_Idle);
	CHeroSOBSState_Idle() : CState((UINT)SCRIPT_TYPE::HEROSOBSSTATE_IDLE) {}
	~CHeroSOBSState_Idle() {}
};

class CHeroSOBSState_Attack
	: public CState
{
private:

public:
	virtual void Enter() override 
	{
		GetStateMachine()->GetOwner()->GetScript<CHeroSwordOfBurningSoulScript>()->Attack();

		GetStateMachine()->Collider2D()->SetActive(true);
	}
	virtual void Tick() override 
	{
		if (GetStateMachine()->FlipbookPlayer()->IsFinishedOnce())
			GetStateMachine()->ChangeState(L"IdleState");
	}
	virtual void Exit() override 
	{
		GetStateMachine()->Collider2D()->SetActive(false);
	}

public:
	CLONE(CHeroSOBSState_Attack);
	CHeroSOBSState_Attack() : CState((UINT)SCRIPT_TYPE::HEROSOBSSTATE_ATTACK) {}
	~CHeroSOBSState_Attack() {}
};

class CHeroSOBSState_Die
	: public CState
{
private:

public:
	virtual void Enter() override 
	{
		GetStateMachine()->FlipbookPlayer()->Play(2, 10.f, true);
	}
	virtual void Tick() override 
	{
		if (GetStateMachine()->FlipbookPlayer()->IsFinishedOnce())
			GetStateMachine()->GetOwner()->SetActive(false);
	}
	virtual void Exit() override {}

public:
	CLONE(CHeroSOBSState_Die);
	CHeroSOBSState_Die() : CState((UINT)SCRIPT_TYPE::HEROSOBSSTATE_DIE) {}
	~CHeroSOBSState_Die() {}
};