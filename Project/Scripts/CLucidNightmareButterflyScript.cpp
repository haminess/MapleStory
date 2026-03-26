#include "pch.h"
#include "CLucidNightmareButterflyScript.h"
#include "CProjectileScript.h"
#include "CPlayerScript.h"

CLucidNightmareButterflyScript::CLucidNightmareButterflyScript()
	: CScript((UINT)SCRIPT_TYPE::LUCIDNIGHTMAREBUTTERFLYSCRIPT)
	, m_Timer(20.f)
	, m_ElapsedTime(0.f)
{
}

CLucidNightmareButterflyScript::CLucidNightmareButterflyScript(const CLucidNightmareButterflyScript& _Other)
	: CScript(_Other)
{
}

CLucidNightmareButterflyScript::~CLucidNightmareButterflyScript()
{
}

void CLucidNightmareButterflyScript::Spawn()
{
	StateMachine()->ChangeState(L"RegenState");
}

void CLucidNightmareButterflyScript::Attack()
{
}

void CLucidNightmareButterflyScript::Init()
{
	GetOwner()->SetName(L"NightmareButterfly");
	GetOwner()->SetLayerIdx((int)LAYER_INDEX::MONSTER);

	GetOwner()->Transform()->UseIndependentScale(true);

	GetOwner()->AddComponent(new CMeshRender);
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	GetOwner()->AddComponent(new CRigidBody2D);
	GetOwner()->RigidBody2D()->UseGravity(false);

	GetOwner()->AddComponent(new CFlipbookPlayer);
	GetOwner()->FlipbookPlayer()->AddFlipbook(0, FIND_ANIM(L"Flipbook\\NightmareButterfly_Regen.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(1, FIND_ANIM(L"Flipbook\\NightmareButterfly_Fly.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(2, FIND_ANIM(L"Flipbook\\NightmareButterfly_Attack.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(3, FIND_ANIM(L"Flipbook\\NightmareButterfly_Hit.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(4, FIND_ANIM(L"Flipbook\\NightmareButterfly_Die.flip"));

	GetOwner()->AddComponent(new CStateMachine);
	GetOwner()->StateMachine()->AddState(L"RegenState", new CLucidNightmareButterflyState_Regen);
	GetOwner()->StateMachine()->AddState(L"FlyState", new CLucidNightmareButterflyState_Fly);
	GetOwner()->StateMachine()->AddState(L"AttackState", new CLucidNightmareButterflyState_Attack);
	GetOwner()->StateMachine()->AddState(L"HitState", new CLucidNightmareButterflyState_Hit);
	GetOwner()->StateMachine()->AddState(L"DieState", new CLucidNightmareButterflyState_Die);
}

void CLucidNightmareButterflyScript::Begin()
{
	GetOwner()->StateMachine()->ChangeState(L"RegenState");
}

void CLucidNightmareButterflyScript::Tick()
{
	m_ElapsedTime += DT;
	if (m_Timer > 0.f && m_ElapsedTime > m_Timer)
	{
		GetOwner()->StateMachine()->ChangeState(L"DieState");
		m_Timer = 0.f;
		m_ElapsedTime = 0.f;
	}
}

void CLucidNightmareButterflyScript::SaveComponent(FILE* _File)
{
}

void CLucidNightmareButterflyScript::LoadComponent(FILE* _FILE)
{
}

void CLucidNightmareButterflyScript::BeginOverlap(CCollider2D* _Collider, CCollider2D* _OtherCollider)
{
}

void CLucidNightmareButterflyState_Attack::Attack()
{
	CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentPlayer();
	if (pPlayer)
	{
		// Åº¸· »ý¼º
		CProjectileScript* pProj = CreateProjectile();
		pProj->GetOwner()->SetName(L"NB Projectile");
		pProj->GetOwner()->SetLayerIdx((int)LAYER_INDEX::MONSTER);

		pProj->GetOwner()->Transform()->SetWorldPos(GetStateMachine()->Transform()->GetWorldPos());

		pProj->GetOwner()->FlipbookPlayer()->AddFlipbook(0, m_BallAnim);
		pProj->GetOwner()->FlipbookPlayer()->Play(0, 10.f, true);

		// Åº¸· ¹ß»ç ¼³Á¤
		Vector3 vPlayer = CLevelMgr::GetInst()->GetCurrentPlayer()->Transform()->GetWorldPos();
		Vector3 vPos = GetStateMachine()->Transform()->GetWorldPos();
		Vector3 vDir = vPlayer - vPos;
		vDir.Normalize();

		pProj->SetDirection(vDir);
		pProj->SetSpeed(100.f);
		pProj->BindDelegate(this, (SCRIPT_DELEGATE_2)&CLucidNightmareButterflyState_Attack::Hit, nullptr, nullptr);

		m_vecProjectile.push_back(pProj->GetOwner());
		CreateObject(pProj->GetOwner(), (int)LAYER_INDEX::MONSTER, false);
	}
}

void CLucidNightmareButterflyState_Attack::Hit(DWORD_PTR _Collider, DWORD_PTR _OtherCollider)
{
	CCollider2D* Other = (CCollider2D*)_OtherCollider;

	if (Other->GetOwner()->GetName() == L"Player")
	{
		PlayHitAnim(m_BallHitAnim.Get(), Other->Transform()->GetWorldPos(), 1);
		Other->GetOwner()->GetScript<CPlayerScript>()->DamageByPercent(0.2f);
	}
}
