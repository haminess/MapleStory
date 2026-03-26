#pragma once
#include <Engine/CScript.h>
class CLucidLittleButterflyBombScript :
	public CScript
{
private:
	float m_Ratio;
	float m_Speed;
	float m_AttackTime;
	Vector3 m_Prev;
	Vector3 m_Dest;

public:
	void Attack();
	void Bomb();
	void Spawn();

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
	CLONE(CLucidLittleButterflyBombScript);
	CLucidLittleButterflyBombScript();
	~CLucidLittleButterflyBombScript();
};

