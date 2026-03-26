#include "pch.h"
#include "CPortalScript.h"
#include "CLevelScript.h"
#include "CPlayerScript.h"

#include <Engine/CRenderMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/func.h>

CPortalScript::CPortalScript(UINT _Type)
	: CScript((UINT)SCRIPT_TYPE::PORTALSCRIPT)
	, m_LevelName(L"None")
	, m_MoveID(0)
	, m_PortalID(0)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::TEXT, "Move Level Name", &m_LevelName });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Move Portal ID", &m_MoveID });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Portal ID", &m_PortalID });
}

CPortalScript::CPortalScript(const CPortalScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::PORTALSCRIPT)
	, m_LevelName(_Other.m_LevelName)
	, m_MoveID(_Other.m_MoveID)
	, m_PortalID(_Other.m_PortalID)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::TEXT, "Move Level Name", &m_LevelName });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Move Portal ID", &m_MoveID });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Portal ID", &m_PortalID });
}

CPortalScript::~CPortalScript()
{
}


void CPortalScript::SaveComponent(FILE* _File)
{
	SaveWString(m_LevelName, _File);
	//fwrite(&m_MoveID, sizeof(int), 1, _File);
	//fwrite(&m_PortalID, sizeof(int), 1, _File);
}

void CPortalScript::LoadComponent(FILE* _File)
{
	LoadWString(m_LevelName, _File);
	//fread(&m_MoveID, sizeof(int), 1, _File);
	//fread(&m_PortalID, sizeof(int), 1, _File);
}


void CPortalScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (KEY_TAP(KEY::UP) && _OtherObject->GetName() == L"Player")
	{
		if (L"None" == m_LevelName || L"" == m_LevelName)
		{
			_OtherObject->GetScript<CPlayerScript>()->MoveToPortal(m_MoveID);
		}
		else
		{
			CRenderMgr::GetInst()->StartFade(true);
			ChangeLevel(m_LevelName);
		}
	}
}
