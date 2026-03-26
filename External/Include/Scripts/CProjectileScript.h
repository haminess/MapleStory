#pragma once
#include <Engine/CScript.h>


class CProjectileScript :
	public CScript
{
private:
	float		m_Speed;

	bool		m_UseDir;
	Vector3     m_Dir;

	bool		m_UseDest;
	Vector3     m_PrevPos;
	Vector3     m_DestPos;
	float		m_DestRatio;
	float		m_DestTimer;

	float		m_Timer;
	float		m_ElapsedTime;

	// µ¨¸®°ÔÀÌÆ®
	CScript* m_DelInst;
	SCRIPT_DELEGATE m_Del_Begin;
	SCRIPT_DELEGATE m_Del_Over;
	SCRIPT_DELEGATE m_Del_End;


	CScript* m_Del1Inst;
	SCRIPT_DELEGATE_1 m_Del1_Begin;
	SCRIPT_DELEGATE_1 m_Del1_Over;
	SCRIPT_DELEGATE_1 m_Del1_End;

	CScript* m_Del2Inst;
	SCRIPT_DELEGATE_2 m_Del2_Begin;
	SCRIPT_DELEGATE_2 m_Del2_Over;
	SCRIPT_DELEGATE_2 m_Del2_End;

public:
	void SetSpeed(float _Speed) { m_Speed = _Speed; }
	float GetSpeed() { return m_Speed; }

	void SetDirection(Vector3 _Dir) 
	{ 
		m_UseDir = true; 
		m_Dir = _Dir; 
	}
	void SetTimer(float _Time) 
	{ 
		m_Timer = _Time;
	}
	void SetDestination(Vector3 _Dest, float _Timer = 1.f)
	{ 
		m_UseDest = true;
		m_PrevPos = GetOwner()->Transform()->GetWorldPos();
		m_DestPos = _Dest;
		m_DestRatio = 0.f;
		m_DestTimer = _Timer;
	}

public:
	void BindDelegate(CScript* _Inst, SCRIPT_DELEGATE _BeginFunc, SCRIPT_DELEGATE _OverFunc, SCRIPT_DELEGATE _EndFunc)
	{
		m_DelInst = _Inst;
		m_Del_Begin = _BeginFunc;
		m_Del_Over = _OverFunc;
		m_Del_End = _EndFunc;
	}
	void BindDelegate(CScript* _Inst, SCRIPT_DELEGATE_1 _BeginFunc, SCRIPT_DELEGATE_1 _OverFunc, SCRIPT_DELEGATE_1 _EndFunc)
	{
		m_Del1Inst = _Inst;
		m_Del1_Begin = _BeginFunc;
		m_Del1_Over = _OverFunc;
		m_Del1_End = _EndFunc;
	}
	void BindDelegate(CScript* _Inst, SCRIPT_DELEGATE_2 _BeginFunc, SCRIPT_DELEGATE_2 _OverFunc, SCRIPT_DELEGATE_2 _EndFunc)
	{
		m_Del2Inst = _Inst;
		m_Del2_Begin = _BeginFunc;
		m_Del2_Over = _OverFunc;
		m_Del2_End = _EndFunc;
	}

public:
	virtual void Begin() override;
	virtual void Tick() override;
	virtual void SaveComponent(FILE* _File) override;
	virtual void LoadComponent(FILE* _File) override;

	virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
	virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
	virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;


public:
	CLONE(CProjectileScript);
	CProjectileScript();
	~CProjectileScript();
};
