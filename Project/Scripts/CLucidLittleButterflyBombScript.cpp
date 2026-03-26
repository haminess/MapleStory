#include "pch.h"
#include "CLucidLittleButterflyBombScript.h"
#include "CSpawnState.h"
#include "CPlayerScript.h"
#include "CProjectileScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CTaskMgr.h>

CLucidLittleButterflyBombScript::CLucidLittleButterflyBombScript()
	: CScript((UINT)SCRIPT_TYPE::LUCIDLITTLEBUTTERFLYBOMBSCRIPT)
	, m_Ratio(1.f)
	, m_Speed(0.2f)
{
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Spawn", this, (SCRIPT_DELEGATE)&CLucidLittleButterflyBombScript::Spawn });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Attack", this, (SCRIPT_DELEGATE)&CLucidLittleButterflyBombScript::Attack });
}

CLucidLittleButterflyBombScript::~CLucidLittleButterflyBombScript()
{
}


void CLucidLittleButterflyBombScript::Attack()
{
	CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentPlayer();
	if (!pPlayer)
		return;

	m_Dest = pPlayer->Transform()->GetWorldPos();
	m_Ratio = 0.f;

	CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CLucidLittleButterflyBombScript::Bomb, 0.5f, false);
}

void CLucidLittleButterflyBombScript::Bomb()
{
	GetOwner()->FlipbookPlayer()->Play(1, 10.f, false);
	Collider2D()->SetActive(true);
}

void CLucidLittleButterflyBombScript::Spawn()
{
	GetOwner()->SetActive(true);
	GetOwner()->FlipbookPlayer()->Play(0, 10.f, true);
	Collider2D()->SetActive(false);

	m_Prev = Transform()->GetWorldPos();
	m_Ratio = 1.f;

	const float time[5] = { 3.f, 3.5f, 4.f, 4.5f, 5.f };
	m_AttackTime = time[RandomRange(0, 4)];
	CTaskMgr::GetInst()->SetTimer(this, (SCRIPT_DELEGATE)&CLucidLittleButterflyBombScript::Attack, m_AttackTime, false);
}


void CLucidLittleButterflyBombScript::Init()
{
	CScript::Init();

	GetOwner()->SetName(L"LB Bomb");
	GetOwner()->SetLayerIdx((int)LAYER_INDEX::MONSTER_OBJECT);

	GetOwner()->AddComponent(new CMeshRender);
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	GetOwner()->AddComponent(new CFlipbookPlayer);
	GetOwner()->FlipbookPlayer()->AddFlipbook(0, FIND_ANIM(L"Flipbook\\LucidLittleButterfly_BombFly.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(1, FIND_ANIM(L"Flipbook\\LucidLittleButterfly_Bomb.flip"));
	GetOwner()->FlipbookPlayer()->Play(0, 10.f, true);

	GetOwner()->AddComponent(new CCollider2D);
	GetOwner()->Collider2D()->SetIndependentScale(true);
	GetOwner()->Collider2D()->SetScale(Vector2(40.f, 40.f));
	GetOwner()->Collider2D()->SetActive(false);
}

void CLucidLittleButterflyBombScript::Begin()
{
	Spawn();
}

void CLucidLittleButterflyBombScript::Tick()
{
	// 폭발하면 비활성화
	if (!FlipbookPlayer()->IsLoop() && FlipbookPlayer()->IsFinishedOnce())
	{
		//GetOwner()->SetActive(false);
		DestroyObject(GetOwner());
	}

	// 공격 시작하면 이동
	if (m_Ratio >= 1.f)
		return;

	m_Ratio += DT * m_Speed;

	Vector3 vMove = Vector3::Lerp(Transform()->GetWorldPos(), m_Dest, m_Ratio);
	Transform()->SetWorldPos(vMove);
}

void CLucidLittleButterflyBombScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	PlayerDamageByPercent(0.2f);
}
