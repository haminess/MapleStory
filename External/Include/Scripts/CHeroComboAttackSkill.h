#pragma once
#include "CSkillScript.h"
class CHeroComboAttackSkill :
    public CSkillScript
{
private:
	int m_Combo;
	CGameObject* m_Back;
	vector<CGameObject*> m_vecCombo;

	Ptr<CFlipbook> m_ComboOnAnim[2];

	float m_Rotator;

private:
	void ClearCombo();
	void ActivateCombo(int _Combo);
	void UpdateCombo();

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void Use() override;
	virtual void End() override;
	virtual void SkillKeyTap() override { Use(); }

public:
	CLONE(CHeroComboAttackSkill);
	CHeroComboAttackSkill(UINT _Type = (UINT)SCRIPT_TYPE::HEROCOMBOATTACKSKILL);
	CHeroComboAttackSkill(const CHeroComboAttackSkill& _Other);
	~CHeroComboAttackSkill();
};

