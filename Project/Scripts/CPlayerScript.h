#pragma once
#include "CLifeScript.h"


class CSkillScript;
class CLevelScript;
class CSliderUI;
class CTextUI;
class CPlayerScript :
	public CLifeScript
{
private:
	float   m_PlayerSpeed;
	Vector3 m_PlayerPos;
	float   m_PaperBurnIntence;
	CGameObject* m_Platform;

	CGameObject m_AttackObj;
	CGameObject* m_Attack;

	Ptr<CTexture>   m_TargetTex;
	Ptr<CPrefab>    m_Prefab;
	
	// Skill
	CSkillScript* pSkill;

	// Map
	CLevelScript* m_LevelScript;
	CGameObject* m_Map;
	int m_MovePortalNum;

	// Hit Blinking
	float   m_fBlinkTime;       // ±ôºýÀÓ Áö¼Ó ½Ã°£
	float   m_fBlinkAccTime;    // ´©Àû ½Ã°£
	float   m_fBlinkInterval;   // ±ôºýÀÓ °£°Ý
	bool    m_bIsBlinking;      // ±ôºýÀÓ »óÅÂ

	CGameObject* m_PlayerModel; // ÇÃ·¹ÀÌ¾î ¸ðµ¨ Ä³½Ì
	float m_AnimSpeed;

	bool m_IsRope;

	// UI
	CSliderUI* m_HPSlider;
	CTextUI* m_HPText;
	CSliderUI* m_MPSlider;
	CTextUI* m_MPText;

public:
	bool IsRope() { return m_IsRope; }
	void SetRope(bool _Rope);

private:
	void Move();
	void Jump();
	void Attack();
	void EndHit();
	void BeginAttack();
	void EndAttack();
	void Animation();

public:
	void Damage(Vector2 _AttackPos, int _Power);
	void DamageByPercent(float _Per);

public:
	void MoveToPortal(int _PortalID = -1);

private:
	void ProjectileTestFunc(DWORD_PTR _Collider, DWORD_PTR _OtherCollider);

public:
	virtual void Dead() override;

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void SaveComponent(FILE* _File) override;
	virtual void LoadComponent(FILE* _File) override;


public:
	CLONE(CPlayerScript);
	CPlayerScript();
	~CPlayerScript();
};

#include <Engine/CState.h>
class CPlayerIdleState :
	public CState
{
public:
	virtual void Enter() 
	{
		GetStateMachine()->GetOwner()->FlipbookPlayer()->Play(0, 10.f, true);
	}
	virtual void Tick() {}
	virtual void Exit() {}

public:
	CLONE(CPlayerIdleState);
	CPlayerIdleState() : CState((UINT)SCRIPT_TYPE::PLAYERIDLESTATE) {}
	~CPlayerIdleState() {}
};
