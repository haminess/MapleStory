#pragma once
#include "CSkillScript.h"
class CHeroSwordOfBurningSoulSkill :
	public CSkillScript
{
private:
	class CHeroSwordOfBurningSoulScript* m_Sword;

public:
	virtual void Use() override;
	virtual void End() override;
	virtual void SkillKeyTap() override;

	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

public:
	CLONE(CHeroSwordOfBurningSoulSkill);
	CHeroSwordOfBurningSoulSkill(UINT _Type = (UINT)SCRIPT_TYPE::HEROSWORDOFBURNINGSOULSKILL);
	CHeroSwordOfBurningSoulSkill(const CHeroSwordOfBurningSoulSkill& _Other);
	~CHeroSwordOfBurningSoulSkill();
};

