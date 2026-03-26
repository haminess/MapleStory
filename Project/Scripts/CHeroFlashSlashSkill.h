#pragma once
#include "CSkillScript.h"
class CHeroFlashSlashSkill :
	public CSkillScript
{
private:
	Ptr<CFlipbook> m_AttackFlip;
	Ptr<CFlipbook> m_HitFlip;
	Ptr<CFlipbook> m_MoveFlip;

	Vector2 m_Range;
	float   m_Speed;
	float   m_Ratio;

	Vector3 m_Destination;
	bool    m_Dir[5];

	CGameObject* m_Detect;
	CGameObject* m_MoveEff;
	CGameObject* m_HitBox;

public:
	void Detect(DWORD_PTR _Col, DWORD_PTR _Other);
	void Hit(DWORD_PTR _Col, DWORD_PTR _Other);

private:
	void SetDestination(Vector3 _Dest)
	{
		m_Destination = _Dest;

		m_MoveEff->SetActive(true);
		m_MoveEff->FlipbookPlayer()->Play(0, 10.f, false);

		// 스킬 시전 위치로 이동
		m_MoveEff->Transform()->SetWorldPos(GetOwner()->Transform()->GetWorldPos());

		// 이동 방향으로 회전시키기
		Vector3 vDir = m_Destination - GetOwner()->Transform()->GetWorldPos();
		float rad = atanf(vDir.y / vDir.x) + XM_PI;
		float rotY = GetOwner()->Transform()->GetRelativeRotation().y + Radian(180.f);
		if (!m_Dir[3] && !m_Dir[4])
			rotY = 0.f;
		m_MoveEff->Transform()->SetRelativeRotation(Vector3(0.f, rotY, rad));
	}

public:
	virtual void Use() override;
	virtual void End() override;
	virtual void SkillKeyTap() override;

	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;


public:
	CLONE(CHeroFlashSlashSkill);
	CHeroFlashSlashSkill(UINT _Type = (UINT)SCRIPT_TYPE::HEROFLASHSLASHSKILL);
	CHeroFlashSlashSkill(const CHeroFlashSlashSkill& _Other);
	~CHeroFlashSlashSkill();
};

