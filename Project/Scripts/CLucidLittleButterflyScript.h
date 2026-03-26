#pragma once
#include <Engine/CScript.h>
class CLucidLittleButterflyScript :
	public CScript
{
private:
	int m_Type;
	int m_State;

	class CLucidLittleButterflyBombScript* m_Bomb[3] = {};

public:
	void Spawn();
	void Erase();
	void Attack();

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

public:
	CLONE(CLucidLittleButterflyScript);
	CLucidLittleButterflyScript();
	CLucidLittleButterflyScript(const CLucidLittleButterflyScript& _Other);
	~CLucidLittleButterflyScript();
};

