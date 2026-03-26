#include "pch.h"
#include "CLucidHornScript.h"
#include "CLucidScript.h"
#include "CLucidLittleButterflySpawnScript.h"

CLucidHornScript::CLucidHornScript()
	: CScript((UINT)SCRIPT_TYPE::LUCIDHORNSCRIPT)
	, m_DiaOffset(Vector3(-16.f, -97.f, 0.f))
	, m_DiaDist(26.f)
	, m_Phase(0)
{
	AddScriptParam({SCRIPT_PARAM::INT, "Count", &m_Activate});
	AddScriptParam({SCRIPT_PARAM::VEC3, "Offset", &m_DiaOffset});
	AddScriptParam({SCRIPT_PARAM::FLOAT, "Distance", &m_DiaDist});
	AddScriptParam({SCRIPT_PARAM::FUNCTION, "Create Dia", this, (SCRIPT_DELEGATE)&CLucidHornScript::CreateDiamond});
	AddScriptParam({SCRIPT_PARAM::FUNCTION, "Ready", this, (SCRIPT_DELEGATE)&CLucidHornScript::Ready});
	AddScriptParam({SCRIPT_PARAM::FUNCTION, "Use", this, (SCRIPT_DELEGATE)&CLucidHornScript::Use});
}

CLucidHornScript::~CLucidHornScript()
{
}


void CLucidHornScript::Init()
{
	CScript::Init();

	GetOwner()->SetName(L"Horn");
	GetOwner()->SetLayerIdx((int)LAYER_INDEX::MONSTER_OBJECT);

	GetOwner()->AddComponent(new CMeshRender);
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	GetOwner()->AddComponent(new CFlipbookPlayer);
	GetOwner()->FlipbookPlayer()->AddFlipbook(0, FIND_ANIM(L"Flipbook\\LucidHorn1_Stand.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(1, FIND_ANIM(L"Flipbook\\LucidHorn1_Ready.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(2, FIND_ANIM(L"Flipbook\\LucidHorn1_Use.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(3, FIND_ANIM(L"Flipbook\\LucidHorn2_Stand.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(4, FIND_ANIM(L"Flipbook\\LucidHorn2_Ready.flip"));
	GetOwner()->FlipbookPlayer()->AddFlipbook(5, FIND_ANIM(L"Flipbook\\LucidHorn2_Use.flip"));
	GetOwner()->FlipbookPlayer()->Play(m_Phase + 0, 10.f, true);

	GetOwner()->AddComponent(new CCollider2D);
	GetOwner()->Collider2D()->SetScale(Vector2(0.5f, 1.f));
	GetOwner()->Collider2D()->SetOffset(Vector2(0.1f, 0.f));
}

void CLucidHornScript::Begin()
{
	CreateDiamond();
}

void CLucidHornScript::Tick()
{
	if (!FlipbookPlayer()->IsLoop() && FlipbookPlayer()->IsFinishedOnce())
	{
		if(m_Activate > 0)
			FlipbookPlayer()->Play(m_Phase + 1, 10.f, true);
		else
			FlipbookPlayer()->Play(m_Phase + 0, 10.f, true);
	}
}

void CLucidHornScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (KEY_TAP(KEY::SPACE) && _OtherObject->GetLayerIdx() == (int)LAYER_INDEX::PLAYER)
	{
		// 나비 없애는 스크립트
		//Use();
		m_LittleButterfly->UseHorn();
	}
}

void CLucidHornScript::CreateDiamond()
{
	Ptr<CFlipbook> pDiaAnim1 = FIND_ANIM(L"Flipbook\\LucidHorn1_Eff.flip");
	Ptr<CFlipbook> pDiaAnim2 = FIND_ANIM(L"Flipbook\\LucidHorn2_Eff.flip");

	for (int i = 0; i < 3; ++i)
	{
		m_Diamond[i] = CreateAnimObject();
		m_Diamond[i]->FlipbookPlayer()->AddFlipbook(0, pDiaAnim1);
		m_Diamond[i]->FlipbookPlayer()->AddFlipbook(3, pDiaAnim2);
		m_Diamond[i]->FlipbookPlayer()->Play(m_Phase + 0, 10.f, true);
		m_Diamond[i]->Transform()->UseIndependentScale(true);
		m_Diamond[i]->Transform()->SetRelativePos(m_DiaOffset.x + m_DiaDist * i, m_DiaOffset.y, 0.f);

		GetOwner()->AddChild(m_Diamond[i]);

		m_Diamond[i]->SetLayerIdx((int)LAYER_INDEX::MONSTER_OBJECT_1);
		m_Diamond[i]->SetActive(false);
	}

}

void CLucidHornScript::AddDiamond(int Add)
{
	ResetDiamond();

	m_Activate = std::clamp(m_Activate + Add, 0, 3);

	for (int i = 0; i < m_Activate; ++i)
	{
		if (Transform()->GetRelativeRotation().y > 0)
		{
			m_Diamond[2 - i]->SetActive(true);
			m_Diamond[2 - i]->FlipbookPlayer()->Play(m_Phase + 0, 10.f, true);
		}
		else
		{
			m_Diamond[i]->SetActive(true);
			m_Diamond[i]->FlipbookPlayer()->Play(m_Phase + 0, 10.f, true);
		}
	}
}

void CLucidHornScript::ResetDiamond()
{
	for (int i = 0; i < 3; ++i)
	{
		m_Diamond[i]->SetActive(false);
	}
}

void CLucidHornScript::Ready()
{
	AddDiamond(1);
	FlipbookPlayer()->Play(m_Phase + 1, 10.f, true);
}

void CLucidHornScript::Use()
{
	if (m_Activate > 0)
	{
		AddDiamond(-1);
		FlipbookPlayer()->Play(m_Phase + 2, 10.f, false);
	}
}
