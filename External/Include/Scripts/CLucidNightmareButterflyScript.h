#pragma once
#include <Engine/CScript.h>
class CLucidNightmareButterflyScript :
	public CScript
{
public:
	CGameObject* m_AttackBall;

	float m_Timer;
	float m_ElapsedTime;

public:
	void Spawn();
	void Attack();

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void SaveComponent(FILE* _File) override;
	virtual void LoadComponent(FILE* _FILE) override;

	void BeginOverlap(CCollider2D* _Collider, CCollider2D* _OtherCollider);

public:
	CLONE(CLucidNightmareButterflyScript);
	CLucidNightmareButterflyScript();
	CLucidNightmareButterflyScript(const CLucidNightmareButterflyScript& _Other);
	~CLucidNightmareButterflyScript();
};

#include <Engine/CState.h>
class CLucidNightmareButterflyState_Regen :
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
			GetStateMachine()->ChangeState(L"FlyState");
	}
	virtual void Exit() override {}

public:
	CLONE(CLucidNightmareButterflyState_Regen)
		CLucidNightmareButterflyState_Regen() : CState((UINT)SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_REGEN) {}
	~CLucidNightmareButterflyState_Regen() {}
};


class CLucidNightmareButterflyState_Fly :
	public CState
{
public:
	const vector<Vector3> m_vecMovePoint =
	{
		{160.f,-380.f,0.f},
		{501.f,-350.f,0.f},
		{800.f,-380.f,0.f},
		{1100.f,-400.f,0.f},
		{1400.f,-370.f,0.f},
		{1700.f,-320.f,0.f},
		{1700.f,-300.f,0.f},
		{1400.f,-310.f,0.f},
		{1100.f,-360.f,0.f},
		{800.f,-370.f,0.f},
		{501.f,-360.f,0.f}
	};
	const Vector3 m_Offset = {-1200.f, 450.f, 0.f};


private:
	Vector3 m_MovePoint;
	float m_Speed;

public:
	virtual void Enter() override
	{
		GetStateMachine()->FlipbookPlayer()->Play(1, 10.f, true);

		m_MovePoint = m_Offset + m_vecMovePoint[RandomRange(0, m_vecMovePoint.size() - 1)];
	}
	virtual void Tick() override
	{
		Vector3 vCurPos = GetStateMachine()->Transform()->GetRelativePos();
		Vector3 vDir = (m_MovePoint - vCurPos).Normalize();
		GetStateMachine()->RigidBody2D()->SetVelocity(vDir * m_Speed);

		float dist = Vector3::Distance(GetStateMachine()->Transform()->GetRelativePos(), m_MovePoint);
		if (dist < 0.2f)
		{
			GetStateMachine()->Transform()->SetRelativePos(m_MovePoint);
			GetStateMachine()->ChangeState(L"AttackState");
		}
	}
	virtual void Exit() override
	{
		GetStateMachine()->RigidBody2D()->SetVelocity(Vector3(0.f, 0.f, 0.f));
	}

public:
	CLONE(CLucidNightmareButterflyState_Fly);

	CLucidNightmareButterflyState_Fly()
		: CState((UINT)SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_FLY)
		, m_Speed(150.f)
	{
	}
	~CLucidNightmareButterflyState_Fly()
	{
	}
};

class CLucidNightmareButterflyState_Attack :
	public CState
{
private:
	Ptr<CFlipbook> m_BallAnim;
	Ptr<CFlipbook> m_BallHitAnim;

	vector<CGameObject*> m_vecProjectile;

	bool m_IsAttackOnce;

public:
	void Attack();
	void Hit(DWORD_PTR _Collider, DWORD_PTR _OtherCollider);

public:
	virtual void Init() override
	{
		m_BallAnim = FIND_ANIM(L"Flipbook\\NightmareButterfly_Ball.flip");
		m_BallHitAnim = FIND_ANIM(L"Flipbook\\NightmareButterfly_Hit.flip");
	}
	virtual void Enter() override
	{
		GetStateMachine()->FlipbookPlayer()->Play(2, 10.f, false);
	}
	virtual void Tick() override
	{
		if (!m_IsAttackOnce && GetStateMachine()->FlipbookPlayer()->GetCurSpriteIdx() == 8)
		{
			Attack();
			m_IsAttackOnce = true;
		}
		else if (GetStateMachine()->FlipbookPlayer()->IsFinishedOnce())
		{
			m_IsAttackOnce = false;

			int rand = RandomRange(0, 1);
			if (rand > 0)
				GetStateMachine()->ChangeState(L"AttackState");
			else
				GetStateMachine()->ChangeState(L"FlyState");
		}
	}
	virtual void Exit() override {}

public:
	CLONE(CLucidNightmareButterflyState_Attack);
	CLucidNightmareButterflyState_Attack() : CState((UINT)SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_ATTACK) {}
	~CLucidNightmareButterflyState_Attack()
	{
		for (CGameObject* pProj : m_vecProjectile)
			DestroyObject(pProj);
	}
};

class CLucidNightmareButterflyState_Hit :
	public CState
{
public:
	virtual void Enter() override
	{
		GetStateMachine()->FlipbookPlayer()->Play(3, 10.f, false);
	}
	virtual void Tick() override {}
	virtual void Exit() override {}

public:
	CLONE(CLucidNightmareButterflyState_Hit);
	CLucidNightmareButterflyState_Hit() : CState((UINT)SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_HIT) {}
	~CLucidNightmareButterflyState_Hit() {}
};

class CLucidNightmareButterflyState_Die :
	public CState
{
public:
	virtual void Enter() override
	{
		GetStateMachine()->FlipbookPlayer()->Play(4, 10.f, false);
	}
	virtual void Tick() override
	{
		if (GetStateMachine()->FlipbookPlayer()->IsFinishedOnce())
			DestroyObject(GetStateMachine()->GetOwner());
	}
	virtual void Exit() override {}

public:
	CLONE(CLucidNightmareButterflyState_Die);
	CLucidNightmareButterflyState_Die() : CState((UINT)SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSTATE_DIE) {}
	~CLucidNightmareButterflyState_Die() {}
};

