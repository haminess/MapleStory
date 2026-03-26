#include "pch.h"
#include "CLevelScript.h"
#include "CPlayerScript.h"
#include "CPortalScript.h"
#include "CSpawnerScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CGameObject.h>

CLevelScript::CLevelScript()
	: CScript((UINT)SCRIPT_TYPE::LEVELSCRIPT)
	, m_Map(nullptr)
	, m_Player(nullptr)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::TEXT, "BGM", &m_BgmName });

	const auto layer = CCollisionMgr::GetInst()->GetCollisionMatrix();
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		AddScriptParam(tScriptParam{ SCRIPT_PARAM::BOOL32, "layer " + std::to_string(i), &layer[i] });
	}
}

CLevelScript::~CLevelScript()
{
}

void CLevelScript::Begin()
{
	// Sound 로딩하기
	wstring name = L"Sound\\" + m_BgmName + L".wav";
	if (L"" != m_BgmName)
	{
		Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(name, name);
		if (nullptr != pSound) pSound->Play(0, 0.5f, true);
	}

	// 포탈 모으기
	vector<CGameObject*> vecPortal;
	CLevelMgr::GetInst()->FindObjectsByName(L"Portal", vecPortal);

	for (CGameObject* pObject : vecPortal)
	{
		CPortalScript* pPortal = pObject->GetScript<CPortalScript>();

		RegisterPortal(pPortal, pPortal->GetPortalID());
	}

	// 포탈 이후 플레이어 스폰
	CGameObject* pFind = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	if (pFind)
	{
		m_Player = pFind->GetScript<CPlayerScript>();
		m_Player->MoveToPortal();
	}

	// 스포너 모으기
	vector<CGameObject*> vecSpawner;
	CLevelMgr::GetInst()->FindObjectsByName(L"Spawner", vecSpawner);

	for (CGameObject* pObject : vecSpawner)
	{
		CSpawnerScript* pSpanwer = pObject->GetScript<CSpawnerScript>();
		if (pSpanwer)
			m_vecSpawner.push_back(pSpanwer);
	}
}

void CLevelScript::SaveComponent(FILE* _File)
{
	SaveWString(m_BgmName, _File);
}

void CLevelScript::LoadComponent(FILE* _FILE)
{
	LoadWString(m_BgmName, _FILE);
}

void CLevelScript::RegisterPortal(CPortalScript* _Portal, int _ID)
{
	if (m_vecPortal.size() <= _ID)
		m_vecPortal.resize(_ID + 1);

	m_vecPortal[_ID] = _Portal;
}

CPortalScript* CLevelScript::GetPortal(int _PortalNum)
{
	if (_PortalNum >= m_vecPortal.size() || m_vecPortal.size() <= 0)
		return nullptr;

	return m_vecPortal[_PortalNum];
}

