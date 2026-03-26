#pragma once
#include <Engine/CScript.h>

class CSkillScript :
	public CScript
{
protected:
	CGameObject* m_SkillUser;
	KEY             m_KeyType;

	bool m_Started;
	int m_ManaCost;
	float m_CoolTime;
	float m_LastUsedTime;

	int m_AtkCount;
	int m_MobCount;


	CScript* m_DelInst;
	SCRIPT_DELEGATE m_DelUse;
	SCRIPT_DELEGATE m_DelTick;
	SCRIPT_DELEGATE m_DelEnd;


	// Skill을 사용하는 스크립트 != 맵에 존재하는 스킬 오브젝트

	// Skill 매화검**
	// 스킬은 델리게이트 사용!!!
	// KeyDown -> SkillScript Use -> 매화검 EnterState -> 매화검 StayState -> 매화검 ExitState
	// SkillScript : 초기화, 컴포넌트 추가 (FP, Col)
	// Enter : (child)start anim play
	// Stay : (child)middle anim play, (this)collider enable, keyup check
	// Exit : (child)end anim play, (this)collider disable
	// -> skill object를 disable 할 필요 없다!!
	// skill마다 skillscript, enter/stay/exit state 개발 필요

	// Skill 투사체
	// KeyDown -> SkillScript Use -> Projectile 생성
	// Object (StateMachine, FlipbookPlayer, Collider, ProjectileScript)
	// StateMachine이 필요없는 스킬도 있다!
	// 작성할 항목 : root skillscript, inst script 

	// statemachine을 꼭 써야되나?

public:
	void BindDelegate(CScript* _Inst, SCRIPT_DELEGATE _Use, SCRIPT_DELEGATE _Tick = nullptr, SCRIPT_DELEGATE _End = nullptr) 
	{ 
		m_DelInst = _Inst; 
		m_DelUse = _Use;
		m_DelTick = _Tick;
		m_DelEnd = _End;
	}


public:
	virtual void Use()
	{
		m_Started = true;
		m_LastUsedTime = CTimeMgr::GetInst()->GetTime();

		if (m_DelInst && m_DelUse)
			(m_DelInst->*m_DelUse)();
	}

	virtual void End() 
	{ 
		m_Started = false; 

		if (m_DelInst && m_DelEnd)
			(m_DelInst->*m_DelEnd)();
	}

	virtual void Attack(DWORD_PTR _Col, DWORD_PTR _OtherCol) {}

	virtual void SkillKeyTap() {}
	virtual void SkillKeyPressed() {}
	virtual void SkillKeyReleased() {}

public:
	KEY GetKey() { return m_KeyType; }
	void SetKey(KEY _Type) { m_KeyType = _Type; }

	CGameObject* GetUser() { return m_SkillUser; }
	void SetCaster(CGameObject* _User) { m_SkillUser = _User; }

	int GetManaCost() { return m_ManaCost; }
	void SetManaCost(int _Set) { m_ManaCost = _Set; }

	float GetCoolTime() { return m_CoolTime; }
	void SetCoolTime(float _Set) { m_CoolTime = _Set; }

	float GetLastUsedTime() { return m_LastUsedTime; }
	void SetLastUsedTime(float _Set) { m_LastUsedTime = _Set; }

	float GetElapsedTime() 
	{
		return CTimeMgr::GetInst()->GetTime() - m_LastUsedTime;
	}
	float GetCooltimeRatio()
	{
		if (m_CoolTime == 0.f)
			return 1.f;

		float ratio = GetElapsedTime() / m_CoolTime;
		return min(1.f, ratio);
	}

	bool IsStarted() { return m_Started; }
	bool IsCooltimeFinished() { return GetElapsedTime() >= m_CoolTime; }

	virtual void SaveComponent(FILE* _File) override;
	virtual void LoadComponent(FILE* _FILE) override;

public:
	virtual void Tick() override;

public:
	CLONE(CSkillScript);
	CSkillScript(UINT Type = (UINT)SCRIPT_TYPE::SKILLSCRIPT);
	CSkillScript(const CSkillScript& _Other);
	~CSkillScript();
};

