#include "pch.h"
#include "CLucidLittleButterflyScript.h"
#include "CSpawnState.h"
#include "CPlayerScript.h"
#include "CProjectileScript.h"
#include "CLucidLittleButterflyBombScript.h"

#include <Engine/CLevelMgr.h>

CLucidLittleButterflyScript::CLucidLittleButterflyScript()
	: CScript((UINT)SCRIPT_TYPE::LUCIDLITTLEBUTTERFLYSCRIPT)
	, m_Type(0)
	, m_State(0)
	, m_Bomb()
{
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Spawn", this, (SCRIPT_DELEGATE)&CLucidLittleButterflyScript::Spawn });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Erase", this, (SCRIPT_DELEGATE)&CLucidLittleButterflyScript::Erase });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Attack", this, (SCRIPT_DELEGATE)&CLucidLittleButterflyScript::Attack });
}

CLucidLittleButterflyScript::CLucidLittleButterflyScript(const CLucidLittleButterflyScript& _Other)
	: CScript(_Other)
	, m_Type(0)
	, m_State(0)
	, m_Bomb()
{
	ClearScriptParam();
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Spawn", this, (SCRIPT_DELEGATE)&CLucidLittleButterflyScript::Spawn });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Erase", this, (SCRIPT_DELEGATE)&CLucidLittleButterflyScript::Erase });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Attack", this, (SCRIPT_DELEGATE)&CLucidLittleButterflyScript::Attack });
}

CLucidLittleButterflyScript::~CLucidLittleButterflyScript()
{
}

void CLucidLittleButterflyScript::Spawn()
{
	GetOwner()->SetActive(true);

	m_Type = RandomRange(0, 8);
	m_State = 0;
	GetOwner()->FlipbookPlayer()->Play(5 * m_Type, 10.f, true);

	for (int i = 0; i < 3; ++i)
	{
		m_Bomb[i]->GetOwner()->SetActive(false);
	}
}

void CLucidLittleButterflyScript::Erase()
{
	m_State = 3;
	GetOwner()->FlipbookPlayer()->Play(5 * m_Type + m_State, 10.f, true);
}

void CLucidLittleButterflyScript::Attack()
{
	m_State = 4;
	GetOwner()->FlipbookPlayer()->Play(5 * m_Type + m_State, 10.f, true);

	for (int i = 0; i < 3; ++i)
	{
		float angle = Radian(120 * (i + 1)) - 30.f;

		m_Bomb[i]->Spawn();
		m_Bomb[i]->GetOwner()->Transform()->SetWorldPos(Transform()->GetWorldPos() + Vector3(cosf(angle), sin(angle), 0.f) * 20.f);
	}
}

void CLucidLittleButterflyScript::Init()
{
	CScript::Init();

	GetOwner()->SetName(L"Little Butterfly");
	GetOwner()->SetLayerIdx((int)LAYER_INDEX::MONSTER_OBJECT);

	GetOwner()->AddComponent(new CMeshRender);
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	GetOwner()->AddComponent(new CFlipbookPlayer);
	const wstring keys[5] = { L"Flipbook\\LucidLittleButterfly_Fly_"
							, L"Flipbook\\LucidLittleButterfly_Fly2_"
							, L"Flipbook\\LucidLittleButterfly_Change_"
							, L"Flipbook\\LucidLittleButterfly_Erase_"
							, L"Flipbook\\LucidLittleButterfly_Prepare_"
	};

	for (int i = 0; i < 8; ++i)
	{
		for (int ki = 0; ki < 5; ++ki)
		{
			GetOwner()->FlipbookPlayer()->AddFlipbook(5 * i + ki, FIND_ANIM(keys[ki] + std::to_wstring(i) + L".flip"));
		}
	}
	// 사용
	// GetOwner()->FlipbookPlayer()->Play(나비 num + 애니 num, 10.f, true);

	GetOwner()->FlipbookPlayer()->Play(0, 10.f, true);


	for (int i = 0; i < 3; ++i)
	{
		CGameObject* pBomb = new CGameObject;
		m_Bomb[i] = new CLucidLittleButterflyBombScript;
		pBomb->AddComponent(m_Bomb[i]);
		
		CreateObject(pBomb, (int)LAYER_INDEX::MONSTER_OBJECT, true);
		pBomb->SetActive(false);
	}
}

void CLucidLittleButterflyScript::Begin()
{

}

void CLucidLittleButterflyScript::Tick()
{
	if (m_State > 2 && FlipbookPlayer()->IsFinishedOnce())
	{
		DestroyObject(GetOwner());
		//GetOwner()->SetActive(false);
	}
}

