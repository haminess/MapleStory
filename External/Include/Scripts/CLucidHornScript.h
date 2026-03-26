#pragma once
#include <Engine/CScript.h>
class CLucidLittleButterflySpawnScript;
class CLucidHornScript :
	public CScript
{
private:
	Vector3 m_DiaOffset;
	float	m_DiaDist;
	
	CGameObject*	m_Diamond[3] = {};
	int				m_Activate;

	int				m_Phase;

	CLucidLittleButterflySpawnScript* m_LittleButterfly;

private:
	void CreateDiamond();
	void ResetDiamond();
	void Ready();

public:
	void AddDiamond(int Add);
	void Use();


public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

public:
	CLONE(CLucidHornScript);
	CLucidHornScript();
	~CLucidHornScript();

	friend class CLucidLittleButterflySpawnScript;
};

