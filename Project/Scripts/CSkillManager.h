#pragma once
#include <Engine/CScript.h>

class CSkillScript;
class CSkillManager :
    public CScript
{
private:
	vector<CSkillScript*> m_vecSkill;

public:
	const vector<CSkillScript*>& GetSkillList() { return m_vecSkill; }
	void RegisterSkill(KEY _Key, CSkillScript* _Skill);

public:
	virtual void Init() override;
	virtual void Tick() override;

public:
    CLONE(CSkillManager);
	CSkillManager();
	CSkillManager(const CSkillManager& _Other);
	~CSkillManager();
};

