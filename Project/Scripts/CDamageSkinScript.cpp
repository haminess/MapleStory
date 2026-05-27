#include "pch.h"
#include "CDamageSkinScript.h"
#include <Engine/CAssetMgr.h>

CDamageSkinScript::CDamageSkinScript(UINT _Type)
	: CScript((UINT)SCRIPT_TYPE::DAMAGESKINSCRIPT)
	, m_Damage(9999999)
	, m_TexArray(nullptr)
	, m_fLifeTime(2.f)
	, m_fSpeed(10.f)
	, m_fCurTime(0.f)
	, m_fGap(0.7f)
{
	m_TexArray = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DamageSkinTex");

	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FUNCTION, "Play",this, (SCRIPT_DELEGATE)&CDamageSkinScript::PlaySetted});
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Damage", &m_Damage });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "LifeTime", &m_fLifeTime });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "CurTime", &m_fCurTime });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Speed", &m_fSpeed });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Gap", &m_fGap });
}

CDamageSkinScript::CDamageSkinScript(const CDamageSkinScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::DAMAGESKINSCRIPT)
	, m_Damage(_Other.m_Damage)
	, m_TexArray(_Other.m_TexArray)
	, m_fLifeTime(_Other.m_fLifeTime)
	, m_fSpeed(_Other.m_fSpeed)
	, m_fCurTime(0.f)
	, m_fGap(_Other.m_fGap)
{
	m_TexArray = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DamageSkinTex");

	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FUNCTION, "Play",this, (SCRIPT_DELEGATE)&CDamageSkinScript::PlaySetted });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::INT, "Damage", &m_Damage });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "LifeTime", &m_fLifeTime });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "CurTime", &m_fCurTime });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Speed", &m_fSpeed });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Gap", &m_fGap });
}

CDamageSkinScript::~CDamageSkinScript()
{
}


void CDamageSkinScript::SaveComponent(FILE* _File)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
	fwrite(&m_fLifeTime, sizeof(float), 1, _File);
	fwrite(&m_fGap, sizeof(float), 1, _File);
	fwrite(&m_Damage, sizeof(int), 1, _File);
}

void CDamageSkinScript::LoadComponent(FILE* _File)
{
	fread(&m_fSpeed, sizeof(float), 1, _File);
	fread(&m_fLifeTime, sizeof(float), 1, _File);
	fread(&m_fGap, sizeof(float), 1, _File);
	fread(&m_Damage, sizeof(int), 1, _File);
}


void CDamageSkinScript::Init()
{
	// 숫자 자리수 계산
	int digit = 0;
	if (m_Damage == 0)
		digit = 1;
	else
	{
		int temp = m_Damage;
		while (temp > 0) {
			digit++;
			temp /= 10;
		}
	}

	if (GetOwner()->MeshRender())
	{
		GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
		GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DamageSkinMtrl"));
		GetOwner()->MeshRender()->GetMaterial()->SetTexParam(TEX_ARR_0, m_TexArray);

		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, 1.f);
		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_1, m_fGap);
		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, m_Damage);
		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, digit);
	}
}

void CDamageSkinScript::Begin()
{

	//m_vStartPos = Transform()->GetWorldPos();
	//m_fLifeTime = 1.f;
	//m_fSpeed = 100.f;
}

void CDamageSkinScript::Tick()
{
	m_fCurTime += DT;
	float alpha = 1.f - (m_fCurTime / m_fLifeTime);  // 시간에 따라 0~1 사이 알파값 계산

	if (m_fCurTime >= m_fLifeTime)
	{
		DestroyObject(GetOwner());
		return;
	}

	// 위로 이동
	Vector3 vCurPos = Transform()->GetWorldPos();
	vCurPos.y += m_fSpeed * DT;
	Transform()->SetWorldPos(vCurPos);

	// 숫자 자리수 계산
	int digit = 0;
	if (m_Damage == 0)
		digit = 1;
	else
	{
		int temp = m_Damage;
		while (temp > 0) {
			digit++;
			temp /= 10;
		}
	}

	// 알파값 적용
	if (GetOwner()->MeshRender())
	{
		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, alpha);
		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_1, m_fGap);
		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, m_Damage);
		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, digit);
	}
}

void CDamageSkinScript::PlayDamageEffect(int _Damage, Vector3 _WorldPos, float _LifeTime, float _Speed)
{
	static Ptr<CPrefab> pPrefab = FIND_PREFAB(L"Prefab\\DamageSkin.pref");

	CGameObject* pObj = pPrefab->Instantiate();
	CDamageSkinScript* pDamage = pObj->GetScript<CDamageSkinScript>();
	if (pDamage)
	{
		static int cnt = 0;
		pDamage->Play(_Damage, _WorldPos, _LifeTime, _Speed);
	}

	CreateObject(pObj, (int)LAYER_INDEX::EFFECT, true);
}

void CDamageSkinScript::Play(int _Damage, Vector3 _WorldPos, float _LifeTime, float _Speed)
{
	m_Damage = _Damage;

	m_fCurTime = 0.f;
	m_fLifeTime = _LifeTime;
	m_fSpeed = _Speed;

	m_vStartPos = _WorldPos;

	Transform()->SetWorldPos(_WorldPos);
}



