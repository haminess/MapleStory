#pragma once
#include "CSkillScript.h"

class CLucidExplosionPrisonSkill :
	public CSkillScript
{
private:
	CGameObject* m_Prison;
	CGameObject* m_SafeZone;

	bool m_IsSafe;

public:
	void Attack();

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Use() override;
	virtual void Tick() override;

	void SafeBeginOverlap(DWORD_PTR _Col1, DWORD_PTR _Col2);
	void SafeEndOverlap(DWORD_PTR _Col1, DWORD_PTR _Col2);

public:
	CLONE(CLucidExplosionPrisonSkill);
	CLucidExplosionPrisonSkill(UINT _Type = (UINT)SCRIPT_TYPE::LUCIDEXPLOSIONPRISONSKILL);
	CLucidExplosionPrisonSkill(const CLucidExplosionPrisonSkill& _Other);
	~CLucidExplosionPrisonSkill();
};



#include <Engine/CState.h>

////////////////////////////////
/// Explosion Prison : State ///
////////////////////////////////

class CLucidPrisonState_Regen :
	public CState
{
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
	CLONE(CLucidPrisonState_Regen);
	CLucidPrisonState_Regen() : CState((UINT)SCRIPT_TYPE::LUCIDPRISONSTATE_REGEN) {}
	~CLucidPrisonState_Regen() {}
};


class CLucidPrisonState_Stay :
	public CState
{

public:
	virtual void Enter() override
	{
		GetStateMachine()->FlipbookPlayer()->Play(1, 10.f, true);

		CGameObject* pNum = GetStateMachine()->GetOwner()->GetChild(0);
		if (pNum)
		{
			pNum->SetActive(true);
			pNum->FlipbookPlayer()->Play(0, 1.f, false);
		}
	}
	virtual void Tick() override
	{
		CGameObject* pNum = GetStateMachine()->GetOwner()->GetChild(0);
		if (pNum && pNum->FlipbookPlayer()->IsFinishedOnce())
		{
			GetStateMachine()->ChangeState(L"DieState");
		}
	}
	virtual void Exit() override {}

public:
	CLONE(CLucidPrisonState_Stay);
	CLucidPrisonState_Stay() : CState((UINT)SCRIPT_TYPE::LUCIDPRISONSTATE_STAY) {}
	~CLucidPrisonState_Stay() {}
};


class CLucidPrisonState_Die :
	public CState
{
private:
	SCRIPT_DELEGATE m_Attack;
	CScript* m_Inst;

public:
	virtual void Enter() override
	{
		GetStateMachine()->FlipbookPlayer()->Play(2, 10.f, false);

		if (m_Inst && m_Attack)
			(m_Inst->*m_Attack)();
	}
	virtual void Tick() override
	{
		if (GetStateMachine()->FlipbookPlayer()->IsFinishedOnce())
			GetStateMachine()->GetOwner()->SetActive(false);
	}
	virtual void Exit() override {}

public:
	CLONE(CLucidPrisonState_Die);
	CLucidPrisonState_Die() : CState((UINT)SCRIPT_TYPE::LUCIDPRISONSTATE_DIE) {}
	~CLucidPrisonState_Die() {}

	friend class CLucidExplosionPrisonSkill;
};

