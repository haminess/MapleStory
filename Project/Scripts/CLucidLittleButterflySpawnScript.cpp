#include "pch.h"
#include "CLucidLittleButterflySpawnScript.h"
#include "CLucidLittleButterflyScript.h"
#include "CLucidHornScript.h"

CLucidLittleButterflySpawnScript::CLucidLittleButterflySpawnScript()
	: CScript((UINT)SCRIPT_TYPE::LUCIDLITTLEBUTTERFLYSPAWNSCRIPT)
{
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Add Horn", this, (SCRIPT_DELEGATE)&CLucidLittleButterflySpawnScript::AddHorn });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Use Horn", this, (SCRIPT_DELEGATE)&CLucidLittleButterflySpawnScript::UseHorn });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Add Butterfly", this, (SCRIPT_DELEGATE)&CLucidLittleButterflySpawnScript::SpawnLittleButterfly });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Attack", this, (SCRIPT_DELEGATE)&CLucidLittleButterflySpawnScript::AttackLittleButterfly });
}

CLucidLittleButterflySpawnScript::CLucidLittleButterflySpawnScript(const CLucidLittleButterflySpawnScript& _Other)
	: CScript(_Other)
{
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Add Horn", this, (SCRIPT_DELEGATE)&CLucidLittleButterflySpawnScript::AddHorn });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Use Horn", this, (SCRIPT_DELEGATE)&CLucidLittleButterflySpawnScript::UseHorn });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Add Butterfly", this, (SCRIPT_DELEGATE)&CLucidLittleButterflySpawnScript::SpawnLittleButterfly });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Attack", this, (SCRIPT_DELEGATE)&CLucidLittleButterflySpawnScript::AttackLittleButterfly });
}

CLucidLittleButterflySpawnScript::~CLucidLittleButterflySpawnScript()
{
}


void CLucidLittleButterflySpawnScript::SpawnLittleButterfly()
{
	const Vector3 offset[40] = {
		{120.f, -410.f, 0.f},{350.f, -470.f, 0.f},{471.f, -380.f, 0.f},{800.f, -290.f, 0.f},{960.f, -360.f, 0.f},{1130.f, -415.f, 0.f},{1210.f, -401.f, 0.f},{1460.f, -453.f, 0.f},{1700.f, -357.f, 0.f}, {1810.f, -439.f, 0.f},
		{70.f, -300.f, 0.f},{300.f, -330.f, 0.f},{518.f, -354.f, 0.f},{630.f, -300.f, 0.f},{710.f, -334.f, 0.f},{1105.f, -300.f, 0.f},{1204.f, -325.f, 0.f},{1420.f, -190.f, 0.f},{1560.f, -270.f, 0.f}, {1690.f, -350.f, 0.f},
		{100.f, -200.f, 0.f},{320.f, -280.f, 0.f},{530.f, -210.f, 0.f},{760.f, -220.f, 0.f},{840.f, -130.f, 0.f},{1140.f, -250.f, 0.f},{1230.f, -300.f, 0.f},{1300.f, -287.f, 0.f},{1720.f, -200.f, 0.f}, {1860.f, -120.f, 0.f},
		{140.f, -104.f, 0.f},{335.f, -140.f, 0.f},{520.f, -80.f, 0.f},{640.f, -95.f, 0.f},{950.f, -140.f, 0.f},{1100.f, -121.f, 0.f},{1140.f, -70.f, 0.f},{1310.f, -69.f, 0.f},{1560.f, -110.f, 0.f}, {1710.f, -180.f, 0.f}
	};

	int rand = RandomRange(0, 39);

	CGameObject* pBF = new CGameObject;
	pBF->SetName(L"LittleButterfly_" + std::to_wstring(m_vecButterfly.size()));
	pBF->Transform()->SetWorldPos(Vector3(-1020.f, 400.f, 0.f) + offset[rand]);

	CLucidLittleButterflyScript* pScript = new CLucidLittleButterflyScript;
	pBF->AddComponent(pScript);

	CreateObject(pBF, (int)LAYER_INDEX::MONSTER_OBJECT_1, false);

	m_vecButterfly.push_back(pScript);

	if (m_vecButterfly.size() >= 40)
	{
		AttackLittleButterfly();
	}
}

void CLucidLittleButterflySpawnScript::EraseLittleButterfly()
{
	for (int i = 0; i < m_vecButterfly.size(); ++i)
	{
		m_vecButterfly[i]->Erase();
	}

	m_vecButterfly.clear();
}

void CLucidLittleButterflySpawnScript::AttackLittleButterfly()
{
	for (int i = 0; i < m_vecButterfly.size(); ++i)
	{
		m_vecButterfly[i]->Attack();
	}

	m_vecButterfly.clear();
	m_LBTimer = 0.f;
}

void CLucidLittleButterflySpawnScript::AddHorn()
{
	m_Horn[0]->Ready();
	m_Horn[1]->Ready();
}

void CLucidLittleButterflySpawnScript::UseHorn()
{
	m_Horn[0]->Use();
	m_Horn[1]->Use();
}

void CLucidLittleButterflySpawnScript::Init()
{
}

void CLucidLittleButterflySpawnScript::Begin()
{
	m_Horn[0] = new CLucidHornScript;
	m_Horn[1] = new CLucidHornScript;
	m_Horn[0]->m_LittleButterfly = this;
	m_Horn[1]->m_LittleButterfly = this;

	CGameObject* pHorn = new CGameObject;
	pHorn->AddComponent(m_Horn[0]);
	pHorn->Transform()->SetRelativePos(-700.f, -152.f, 0.f);
	pHorn->Transform()->SetRelativeRotation(0.f, Radian(180.f), 0.f);
	CreateObject(pHorn, (int)LAYER_INDEX::MONSTER_OBJECT, false);

	pHorn = new CGameObject;
	pHorn->AddComponent(m_Horn[1]);
	pHorn->Transform()->SetRelativePos(700.f, -152.f, 0.f);
	CreateObject(pHorn, (int)LAYER_INDEX::MONSTER_OBJECT, false);
}

void CLucidLittleButterflySpawnScript::Tick()
{
	m_LBTimer += DT;
	if (m_LBTimer > 5.f)
	{
		m_LBTimer -= 5.f;
		SpawnLittleButterfly();
	}
}

void CLucidLittleButterflySpawnScript::SaveComponent(FILE* _File)
{
}

void CLucidLittleButterflySpawnScript::LoadComponent(FILE* _FILE)
{
}

