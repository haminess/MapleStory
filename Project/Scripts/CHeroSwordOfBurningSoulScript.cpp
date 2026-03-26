#include "pch.h"
#include "CHeroSwordOfBurningSoulScript.h"
#include "CMonsterScript.h"

CHeroSwordOfBurningSoulScript::CHeroSwordOfBurningSoulScript()
	: CScript((UINT)SCRIPT_TYPE::HEROSWORDOFBURNINGSOULSCRIPT)
{
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Summon", this, (SCRIPT_DELEGATE)&CHeroSwordOfBurningSoulScript::Summon });
}

CHeroSwordOfBurningSoulScript::~CHeroSwordOfBurningSoulScript()
{
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Summon", this, (SCRIPT_DELEGATE)&CHeroSwordOfBurningSoulScript::Summon });
}


void CHeroSwordOfBurningSoulScript::Summon()
{
	StateMachine()->ChangeState(L"RegenState");

	// pos : 42.f, 89.f
}

void CHeroSwordOfBurningSoulScript::Attack()
{
	if(GetOwner()->GetParent())
		FlipbookPlayer()->Play(4, 10.f, false);
	else
		FlipbookPlayer()->Play(3, 10.f, false);
}

void CHeroSwordOfBurningSoulScript::Init()
{
	CScript::Init();

	m_HitAnim[0] = FIND_ANIM(L"Flipbook\\HeroSOBS_Hit.flip");
	m_HitAnim[1] = FIND_ANIM(L"Flipbook\\HeroSOBS_Hit2.flip");

	GetOwner()->SetName(L"SwordOfBurningSoul");
	GetOwner()->SetLayerIdx((int)LAYER_INDEX::PLAYER_OBJECT);

	GetOwner()->Transform()->UseIndependentScale(true);

	GetOwner()->AddComponent(new CMeshRender);
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	GetOwner()->AddComponent(new CFlipbookPlayer);
	GetOwner()->FlipbookPlayer()->AddFlipbook(0, FIND_ANIM(L"Flipbook\\HeroSOBS_Regen.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(1, FIND_ANIM(L"Flipbook\\HeroSOBS_Stand.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(2, FIND_ANIM(L"Flipbook\\HeroSOBS_Die.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(3, FIND_ANIM(L"Flipbook\\HeroSOBS_Attack.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(4, FIND_ANIM(L"Flipbook\\HeroSOBS_Attack2.flip"));

	GetOwner()->AddComponent(new CCollider2D);
	GetOwner()->Collider2D()->SetActive(false);
	GetOwner()->Collider2D()->SetIndependentScale(true);
	//GetOwner()->Collider2D()->SetScale(Vector2(0.5f, 1.f));
	//GetOwner()->Collider2D()->SetOffset(Vector2(0.1f, 0.f));

	GetOwner()->AddComponent(new CStateMachine);
	GetOwner()->StateMachine()->AddState(L"RegenState", new CHeroSOBSState_Regen);
	GetOwner()->StateMachine()->AddState(L"IdleState", new CHeroSOBSState_Idle);
	GetOwner()->StateMachine()->AddState(L"AttackState", new CHeroSOBSState_Attack);
	GetOwner()->StateMachine()->AddState(L"DieState", new CHeroSOBSState_Die);
	GetOwner()->StateMachine()->ChangeState(L"RegenState");
}

void CHeroSwordOfBurningSoulScript::Begin()
{
}

void CHeroSwordOfBurningSoulScript::Tick()
{
}

void CHeroSwordOfBurningSoulScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	CMonsterScript* pMonster = _OtherObject->GetScript<CMonsterScript>();
	if (pMonster)
	{
		if (GetOwner()->GetParent())
			pMonster->Hit(10, 6, m_HitAnim[1], GetKnockbackDir(_OtherCollider, _Collider));
			else
			pMonster->Hit(10, 6, m_HitAnim[0], GetKnockbackDir(_OtherCollider, _Collider));
	}
}
