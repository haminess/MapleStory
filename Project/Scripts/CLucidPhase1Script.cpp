#include "pch.h"
#include "CLucidPhase1Script.h"

#include "CLucidScript.h"
#include "CLucidFlowerExplosionScript.h"

CLucidPhase1Script::CLucidPhase1Script()
	: CScript(SCRIPT_TYPE::LUCIDPHASE1SCRIPT)
{
}

CLucidPhase1Script::CLucidPhase1Script(const CLucidPhase1Script& _Other)
	: CScript(_Other)
{
}


CLucidPhase1Script::~CLucidPhase1Script()
{
}

void CLucidPhase1Script::Init()
{

}

void CLucidPhase1Script::Begin()
{
	CGameObject* pLucid = new CGameObject;
	pLucid->AddComponent(new CLucidScript);
	CreateObject(pLucid, (int)LAYER_INDEX::MONSTER, true);

	CGameObject* pFE = new CGameObject;
	pFE->Transform()->SetWorldPos(pLucid->Transform()->GetWorldPos());
	pFE->AddComponent(new CLucidFlowerExplosionScript);
	CreateObject(pFE, (int)LAYER_INDEX::MONSTER_1, false);
}

void CLucidPhase1Script::Tick()
{
}

