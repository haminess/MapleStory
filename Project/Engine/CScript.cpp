#include "pch.h"
#include "CScript.h"

CScript::CScript(UINT _ScriptType)
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_ScriptType(_ScriptType)
{
}

CScript::CScript(const CScript& _Other)
	: CComponent(COMPONENT_TYPE::SCRIPT)
	, m_ScriptType(_Other.m_ScriptType)
	, m_ScriptName(_Other.m_ScriptName)
	, m_vecMeta(_Other.m_vecMeta)
	, m_vecScriptParam(_Other.m_vecScriptParam)
{
	ClearScriptParam();
}

CScript::~CScript()
{
}


void CScript::SaveComponent(FILE* _File)
{
}

void CScript::LoadComponent(FILE* _FILE)
{
}

void CScript::Instantiate(Ptr<CPrefab> _Pref, Vector3 _WorldPos, int _Layer)
{
	if (nullptr == _Pref)
		return;

	CGameObject* pNewObject = _Pref->Instantiate();

	pNewObject->Transform()->SetRelativePos(_WorldPos);

	CreateObject(pNewObject, _Layer, false);
}

void CScript::Instantiate(Ptr<CPrefab> _Pref)
{
	if (nullptr == _Pref)
		return;

	CGameObject* pNewObject = _Pref->Instantiate();

	pNewObject->Transform()->SetRelativePos(GetOwner()->Transform()->GetWorldPos());

	CreateObject(pNewObject, GetOwner()->GetLayerIdx(), false);
}