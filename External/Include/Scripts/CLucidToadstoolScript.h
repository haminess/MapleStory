#pragma once
#include "CMonsterScript.h"
class CLucidToadstoolScript :
	public CMonsterScript
{
private:

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

	virtual void SaveComponent(FILE* _File) override;
	virtual void LoadComponent(FILE* _FILE) override;

public:
	CLONE(CLucidToadstoolScript);
	CLucidToadstoolScript();
	CLucidToadstoolScript(const CLucidToadstoolScript& _Other);
	~CLucidToadstoolScript();
};
