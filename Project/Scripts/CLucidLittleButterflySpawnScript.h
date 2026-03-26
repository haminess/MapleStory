#pragma once
#include <Engine/CScript.h>

class CLucidLittleButterflyScript;
class CLucidHornScript;

class CLucidLittleButterflySpawnScript :
	public CScript
{
private:
	vector<CLucidLittleButterflyScript*> m_vecButterfly;
	CLucidHornScript* m_Horn[2];
	float m_LBTimer = 0.f;

public:
	void SpawnLittleButterfly();
	void EraseLittleButterfly();
	void AttackLittleButterfly();
	void AddHorn();
	void UseHorn();

public:
	virtual void Init() override;
	virtual void Begin() override;
	virtual void Tick() override;

	virtual void SaveComponent(FILE* _File) override;
	virtual void LoadComponent(FILE* _FILE) override;

public:
	CLONE(CLucidLittleButterflySpawnScript);
	CLucidLittleButterflySpawnScript();
	CLucidLittleButterflySpawnScript(const CLucidLittleButterflySpawnScript& _Other);
	~CLucidLittleButterflySpawnScript();
};
