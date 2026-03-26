#include "pch.h"
#include "CStrikeSkill.h"
#include "CMonsterScript.h"

CStrikeSkill::CStrikeSkill(UINT _Type)
	: CSkillScript(_Type)
	, m_Count(1)
	, m_Power(30)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Count", &m_Count });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Power", &m_Power });
}

CStrikeSkill::CStrikeSkill(const CStrikeSkill& _Other)
	: CSkillScript(_Other)
	, m_Count(1)
	, m_Power(30)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Count", &m_Count });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Power", &m_Power });
}

CStrikeSkill::~CStrikeSkill()
{
}

void CStrikeSkill::Init()
{
	//m_Arrow = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"Prefab\\SongOfHeavenArrow.pref");
	GetOwner()->FlipbookPlayer()->Play(0, 10, false);
}

void CStrikeSkill::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if ((int)LAYER_INDEX::MONSTER == _OtherObject->GetLayerIdx())
	{
		CMonsterScript* pMosnter = _OtherObject->GetScript<CMonsterScript>();
		if (pMosnter)
		{
			for (int i = 0; i < m_Count; ++i)
			{
				pMosnter->Damage(m_Power);

				Vector3 vHitForce = _OtherObject->Transform()->GetWorldPos() - Transform()->GetWorldPos();
				vHitForce.Normalize();

				_OtherObject->RigidBody2D()->AddVelocity(Vector3(vHitForce.x * 300.f, 0.f, 0.f));
			}
		}
	}
}
void CStrikeSkill::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if ((int)LAYER_INDEX::MONSTER == _OtherObject->GetLayerIdx())
	{
		int a = 0;
	}
}

void CStrikeSkill::SaveComponent(FILE* _File)
{
	CSkillScript::SaveComponent(_File);
	fwrite(&m_Count, sizeof(int), 1, _File);
	fwrite(&m_Power, sizeof(int), 1, _File);
}

void CStrikeSkill::LoadComponent(FILE* _File)
{
	CSkillScript::LoadComponent(_File);
	fread(&m_Count, sizeof(int), 1, _File);
	fread(&m_Power, sizeof(int), 1, _File);
}
