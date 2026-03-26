#include "pch.h"
#include "CSpawnerScript.h"

CSpawnerScript::CSpawnerScript(UINT _Type)
	: CScript(_Type)
	, m_PrefabName(L"")
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::TEXT, "Spawn Target", &m_PrefabName });
}

CSpawnerScript::CSpawnerScript(const CSpawnerScript& _Other)
	: CScript(_Other)
	, m_PrefabName(_Other.m_PrefabName)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::TEXT, "Spawn Target", &m_PrefabName });
}

CSpawnerScript::~CSpawnerScript()
{
}

void CSpawnerScript::Spawn()
{
	if (m_Prefab == nullptr)
		return;

	CGameObject* pNewObject = m_Prefab->Instantiate();
	pNewObject->Transform()->SetWorldPos(GetOwner()->Transform()->GetWorldPos());
	CreateObject(pNewObject, pNewObject->GetLayerIdx(), true);
}

void CSpawnerScript::Begin()
{
	m_Prefab = CAssetMgr::GetInst()->FindPrefabByName(m_PrefabName);
	Spawn();
}

void CSpawnerScript::Tick()
{
	DrawDebugCircle(
		Vector4(0.f, 1.f, 1.f, 1.f),
		GetOwner()->Transform()->GetWorldPos(),
		GetOwner()->Transform()->GetRelativeScale().x
	);
}

void CSpawnerScript::SaveComponent(FILE* _File)
{
	SaveWString(m_PrefabName, _File);
}

void CSpawnerScript::LoadComponent(FILE* _File)
{
	LoadWString(m_PrefabName, _File);
}
