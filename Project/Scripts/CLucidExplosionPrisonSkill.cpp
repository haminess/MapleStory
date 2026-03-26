#include "pch.h"
#include "CLucidExplosionPrisonSkill.h"
#include "CProjectileScript.h"

#include <Engine/CLevelMgr.h>

CLucidExplosionPrisonSkill::CLucidExplosionPrisonSkill(UINT _Type)
	: CSkillScript(_Type)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FUNCTION, "Attack", this, (SCRIPT_DELEGATE)&CLucidExplosionPrisonSkill::Attack });
}

CLucidExplosionPrisonSkill::CLucidExplosionPrisonSkill(const CLucidExplosionPrisonSkill& _Other)
	: CSkillScript(_Other)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FUNCTION, "Attack", this, (SCRIPT_DELEGATE)&CLucidExplosionPrisonSkill::Attack });
}

CLucidExplosionPrisonSkill::~CLucidExplosionPrisonSkill()
{
}

void CLucidExplosionPrisonSkill::Attack()
{
	if (m_IsSafe)
	{
		m_Prison->FlipbookPlayer()->Play(2, 10.f, false);
	}
	else
	{
		m_Prison->FlipbookPlayer()->Play(3, 10.f, false);

		PlayerDamage(1.f);
	}

	m_SafeZone->StateMachine()->ChangeState(L"DieState");
}

void CLucidExplosionPrisonSkill::Init()
{
	m_Prison = CreateAnimObject();
	m_Prison->SetName(L"Prison");
	m_Prison->SetLayerIdx((int)LAYER_INDEX::EFFECT);
	m_Prison->SetActive(false);

	m_Prison->Transform()->UseIndependentScale(true);
	m_Prison->Transform()->SetRelativePos(0.f, 100.f, 0.f);

	m_Prison->FlipbookPlayer()->AddFlipbook(0, FIND_ANIM(L"Flipbook\\LucidPrison_Appear.flip"));
	m_Prison->FlipbookPlayer()->AddFlipbook(1, FIND_ANIM(L"Flipbook\\LucidPrison.flip"));
	m_Prison->FlipbookPlayer()->AddFlipbook(2, FIND_ANIM(L"Flipbook\\LucidPrison_Disppear.flip"));
	m_Prison->FlipbookPlayer()->AddFlipbook(3, FIND_ANIM(L"Flipbook\\LucidPrison_Explosion.flip"));

	CLucidPrisonState_Regen* pRegenState = new CLucidPrisonState_Regen;
	CLucidPrisonState_Stay* pStayState = new CLucidPrisonState_Stay;
	CLucidPrisonState_Die* pDieState = new CLucidPrisonState_Die;
	pDieState->m_Inst = this;
	pDieState->m_Attack = (SCRIPT_DELEGATE)&CLucidExplosionPrisonSkill::Attack;

	m_Prison->AddComponent(new CStateMachine);
	m_Prison->StateMachine()->AddState(L"RegenState", pRegenState);
	m_Prison->StateMachine()->AddState(L"IdleState", pStayState);
	m_Prison->StateMachine()->AddState(L"DieState", pDieState);
	m_Prison->StateMachine()->ChangeState(L"RegenState");

	CreateObject(m_Prison, (int)LAYER_INDEX::EFFECT, false);

	// lucid num
	CGameObject* pChild = CreateAnimObject();
	pChild->SetName(L"Prison Num");
	pChild->SetLayerIdx((int)LAYER_INDEX::EFFECT);
	pChild->SetActive(false);
	pChild->Transform()->UseIndependentScale(true);
	pChild->Transform()->SetRelativePos(-3.f, -34.f, 0.f);
	pChild->FlipbookPlayer()->AddFlipbook(0, FIND_ANIM(L"Flipbook\\LucidPrison_Num3.flip"));
	pChild->FlipbookPlayer()->Play(0, 1.f, false);
	m_Prison->AddChild(pChild);


	m_SafeZone = CreateAnimObject();
	m_SafeZone->SetName(L"Prison SafeZone");
	m_SafeZone->SetLayerIdx((int)LAYER_INDEX::MONSTER_OBJECT);
	m_SafeZone->SetActive(false);
	
	m_SafeZone->AddComponent(new CCollider2D);
	m_SafeZone->Collider2D()->SetIndependentScale(true);
	m_SafeZone->Collider2D()->SetScale(Vector2(100.f, 100.f));

	CProjectileScript* pProj = new CProjectileScript;
	SCRIPT_DELEGATE_2 begin = (SCRIPT_DELEGATE_2)&CLucidExplosionPrisonSkill::SafeBeginOverlap;
	SCRIPT_DELEGATE_2 end = (SCRIPT_DELEGATE_2)&CLucidExplosionPrisonSkill::SafeEndOverlap;
	pProj->BindDelegate(this, begin, nullptr, end);
	m_SafeZone->AddComponent(pProj);

	m_SafeZone->FlipbookPlayer()->AddFlipbook(0, FIND_ANIM(L"Flipbook\\LucidPrison_SafeAppear.flip"));
	m_SafeZone->FlipbookPlayer()->AddFlipbook(1, FIND_ANIM(L"Flipbook\\LucidPrison_Safe.flip"));
	m_SafeZone->FlipbookPlayer()->AddFlipbook(2, FIND_ANIM(L"Flipbook\\LucidPrison_SafeEnd.flip"));

	m_SafeZone->AddComponent(new CStateMachine);
	m_SafeZone->StateMachine()->AddState(L"RegenState", new CLucidPrisonState_Regen);
	m_SafeZone->StateMachine()->AddState(L"IdleState", new CLucidPrisonState_Stay);
	m_SafeZone->StateMachine()->AddState(L"DieState", new CLucidPrisonState_Die);
	m_SafeZone->StateMachine()->ChangeState(L"RegenState");

	CreateObject(m_SafeZone, (int)LAYER_INDEX::MONSTER_OBJECT, false);
}

void CLucidExplosionPrisonSkill::Begin()
{
}

void CLucidExplosionPrisonSkill::Use()
{
	CSkillScript::Use();

	FlipbookPlayer()->Play(3, 10.f, true);

	CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentPlayer();
	if (!pPlayer)
		return;

	m_Prison->SetActive(true);
	AddChild(pPlayer, m_Prison);
	m_Prison->StateMachine()->ChangeState(L"RegenState");

	m_SafeZone->SetActive(true);
	m_SafeZone->Transform()->SetWorldPos(Vector3(RandomRange(-900.f, 900.f), -238.f, 0.f));
	m_SafeZone->StateMachine()->ChangeState(L"RegenState");
}

void CLucidExplosionPrisonSkill::Tick()
{
}

void CLucidExplosionPrisonSkill::SafeBeginOverlap(DWORD_PTR _Col1, DWORD_PTR _Col2)
{
	m_IsSafe = true;
}

void CLucidExplosionPrisonSkill::SafeEndOverlap(DWORD_PTR _Col1, DWORD_PTR _Col2)
{
	m_IsSafe = false;
}