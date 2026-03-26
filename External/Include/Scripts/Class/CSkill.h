#pragma once
#include <Engine\CGameObject.h>
class CSkill :
	public CGameObject
{
private:
	float m_ManaCost;
	float m_CoolTime;
	float m_CastTime;
	float m_LastUsedTime;

public:
	virtual void Cast() = 0;
	virtual void Exit() {}

public:
	CSkill();
	~CSkill();
};

