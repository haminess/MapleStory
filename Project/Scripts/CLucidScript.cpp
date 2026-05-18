#include "pch.h"
#include "CLucidScript.h"

#include "CSkillScript.h"
#include "CLucidNightmareButterflyScript.h"
#include "CLucidExplosionPrisonSkill.h"
#include "CLucidLittleButterflyScript.h"
#include "CLucidLittleButterflySpawnScript.h"
#include "CLucidDragonScript.h"
#include "CLucidFairyDustSkill.h"
#include "CLucidDragonSkill.h"
#include "CLucidGolemScript.h"
#include "CLucidToadstoolScript.h"
#include "CLucidHornScript.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CTaskMgr.h>

CLucidScript::CLucidScript()
	: CMonsterScript(SCRIPT_TYPE::LUCIDSCRIPT)
	, m_TimerID(0)
{
}

CLucidScript::CLucidScript(const CLucidScript& _Other)
	: CMonsterScript(_Other)
	, m_TimerID(0)
{
}


CLucidScript::~CLucidScript()
{
	if(m_TimerID != 0)
		CTaskMgr::GetInst()->StopTimer(m_TimerID);
}

void CLucidScript::SpawnNightmareButterfly()
{
	FlipbookPlayer()->Play(3, 10.f, true);

	for (int i = 0; i < 3; ++i)
	{
		CGameObject* pBF = new CGameObject;
		pBF->SetName(L"NightmareButterfly");
		pBF->Transform()->SetWorldPos(Vector3(RandomRange(-800.f, 800.f), RandomRange(-130.f, 200.f), 0.f));

		CLucidNightmareButterflyScript* pScript = new CLucidNightmareButterflyScript;
		pBF->AddComponent(pScript);
		pScript->Spawn();

		CreateObject(pBF, 14, true);
	}
}

void CLucidScript::TeleportPlayer()
{
	FlipbookPlayer()->Play(4, 10.f, true);

	float RandomPosX = RandomRange(-800.f, 800.f);

	CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentPlayer();
	if (pPlayer)
	{
		pPlayer->Transform()->SetWorldPos(Vector3(RandomPosX, 0.f, -20.f));
		CRenderMgr::GetInst()->PlayAnimEff(m_AnimTeleportHit, pPlayer, {});
	}
}

void CLucidScript::SpawnLittleButterfly()
{
	m_LittleButterfly->SpawnLittleButterfly();
}


void CLucidScript::SpawnGolem()
{
	FlipbookPlayer()->Play(3, 10.f, true);

	int cnt = RandomRange(1, 4);
	for (int i = 0; i < cnt; ++i)
	{
		CGameObject* pObj = new CGameObject;
		pObj->SetName(L"Golem");
		pObj->AddComponent(new CLucidGolemScript);

		pObj->Transform()->SetWorldPos(Vector3(RandomRange(-1080.f, 1080.f), -57.f, 0.f));

		CreateObject(pObj, (int)LAYER_INDEX::MONSTER, true);
	}
}

void CLucidScript::SpawnToadstool()
{
	FlipbookPlayer()->Play(3, 10.f, true);

	int cnt = RandomRange(1, 3);
	for (int i = 0; i < cnt; ++i)
	{
		CGameObject* pObj = new CGameObject;
		pObj->SetName(L"Toadstool");
		pObj->AddComponent(new CLucidToadstoolScript);

		pObj->Transform()->SetWorldPos(Vector3(RandomRange(-900.f, 900.f), -147.f, 0.f));

		CreateObject(pObj, (int)LAYER_INDEX::MONSTER, true);
	}
}

void CLucidScript::Init()
{
	GetOwner()->SetName(L"Lucid");
	GetOwner()->SetLayerIdx((int)LAYER_INDEX::MONSTER);

	GetOwner()->AddComponent(new CMeshRender);
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	GetOwner()->AddComponent(new CFlipbookPlayer);
	FlipbookPlayer()->AddFlipbook(0, FIND_ANIM(L"Flipbook\\LucidSit_Stand.flip"));
	FlipbookPlayer()->AddFlipbook(1, FIND_ANIM(L"Flipbook\\LucidSit_Skill1.flip"));
	FlipbookPlayer()->AddFlipbook(2, FIND_ANIM(L"Flipbook\\LucidSit_Skill2.flip"));
	FlipbookPlayer()->AddFlipbook(3, FIND_ANIM(L"Flipbook\\LucidSit_Skill3.flip"));
	FlipbookPlayer()->AddFlipbook(4, FIND_ANIM(L"Flipbook\\LucidSit_Skill4.flip"));
	FlipbookPlayer()->AddFlipbook(5, FIND_ANIM(L"Flipbook\\LucidSit_Hit.flip"));
	FlipbookPlayer()->AddFlipbook(6, FIND_ANIM(L"Flipbook\\LucidSit_Die.flip"));
	FlipbookPlayer()->Play(0, 10.f, true);

	m_AnimTeleportHit = FIND_ANIM(L"Flipbook\\LucidTeleport_Hit.flip");
}

void CLucidScript::Begin()
{
	m_vecSkill.push_back(new CLucidFairyDustSkill);			// fairy dust
	m_vecSkill[0]->SetCoolTime(5.f);	

	m_vecSkill.push_back(new CLucidDragonSkill);			// dragon
	m_vecSkill[1]->SetCoolTime(50.f);	

	m_vecSkill.push_back(new CSkillScript);					// nightmare butterfly
	m_vecSkill[2]->SetCoolTime(5.f);	
	m_vecSkill[2]->BindDelegate(this, (SCRIPT_DELEGATE)&CLucidScript::SpawnNightmareButterfly);

	m_vecSkill.push_back(new CSkillScript);					// forced teleport
	m_vecSkill[3]->SetCoolTime(5.f);	
	m_vecSkill[3]->BindDelegate(this, (SCRIPT_DELEGATE)&CLucidScript::TeleportPlayer);
	
	m_vecSkill.push_back(new CLucidExplosionPrisonSkill);	// prison
	m_vecSkill[4]->SetCoolTime(5.f);	
	
	m_vecSkill.push_back(new CSkillScript);					// golem
	m_vecSkill[5]->SetCoolTime(5.f);	
	m_vecSkill[5]->BindDelegate(this, (SCRIPT_DELEGATE)&CLucidScript::SpawnGolem);
	
	m_vecSkill.push_back(new CSkillScript);					// toadstool
	m_vecSkill[6]->SetCoolTime(5.f);	
	m_vecSkill[6]->BindDelegate(this, (SCRIPT_DELEGATE)&CLucidScript::SpawnToadstool);

	for (auto& skill : m_vecSkill)
	{
		GetOwner()->AddComponent(skill);
		skill->SetCaster(GetOwner());
	}

	// little butterfly & horn
	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"Butterfly Spawner");

	m_LittleButterfly = new CLucidLittleButterflySpawnScript;
	pObj->AddComponent(m_LittleButterfly);

	CreateObject(pObj, (int)LAYER_INDEX::DEFAULT, false);

	// 5초마다 패턴
	UINT timer_id = CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CLucidScript::CastNextPattern, 5.f, true);
}

void CLucidScript::Tick()
{
	if (FlipbookPlayer()->IsFinishedOnce())
	{
		FlipbookPlayer()->Play(0, 10.f, true);
	}

	//// 5초마다 패턴
	//m_PatternDelay += DT;
	//if (m_PatternDelay > 5.f)
	//{
	//	int rand = RandomRange(0, m_vecSkill.size() - 1);
	//	if (m_vecSkill[rand]->IsCooltimeFinished())
	//	{
	//		m_vecSkill[rand]->Use();
	//		m_PatternDelay -= 5.f;
	//	}
	//}
}


void CLucidScript::CastNextPattern()
{
	int rand = RandomRange(0, m_vecSkill.size() - 1);
	if (m_vecSkill[rand]->IsCooltimeFinished())
	{
		m_vecSkill[rand]->Use();
		m_PatternDelay -= 5.f;
	}
}


