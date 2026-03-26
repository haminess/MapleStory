#include "pch.h"
#include "CMouseScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CMaterial.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CDevice.h>
#include <Engine/CEngine.h>
#include <Engine/CCamera.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include "CParticleSystemScript.h"

CMouseScript::CMouseScript()
	: CScript((UINT)SCRIPT_TYPE::MOUSESCRIPT)
	, m_Cam(nullptr)
	, m_Time(0.f)
	, m_HitParticle(nullptr)
	, m_SkillHitTex(nullptr)
	, m_ParticleChild(nullptr)
{
	//m_Cam = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"MainCamera");
	//m_Texture = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Texture");
	AddScriptParam({SCRIPT_PARAM::FLIPBOOK, "", nullptr});
}

CMouseScript::~CMouseScript()
{
}

tParticleModule InitModule()
{
	tParticleModule Module = {};
	// Spawn Module
	Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
	Module.SpawnRate = 200;
	Module.vSpawnColor = Vector4(1.f, 3.f, 4.f, 1.f);
	Module.MinLife = 1.f;
	Module.MaxLife = 5.f;
	Module.vSpawnMinScale = Vector3(50.f, 50.f, 1.f);
	Module.vSpawnMaxScale = Vector3(50.f, 50.f, 1.f);

	Module.SpawnShape = 1;
	Module.SpawnShapeScale.x = 200.f;

	Module.BlockSpawnShape = 1;
	Module.BlockSpawnShapeScale.x = 0.f;

	Module.SpaceType = 2; // Local Space 

	// Spawn Burst Module
	Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
	Module.SpawnBurstRepeat = true;
	Module.SpawnBurstCount = 100;
	Module.SpawnBurstRepeatTime = 3.f;

	// Scale Module
	Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
	Module.StartScale = 1.f;
	Module.EndScale = 1.f;

	// AddVelocity Module
	Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = false;
	Module.AddVelocityType = 1;
	Module.AddVelocityFixedDir = Vector3(0.f, 1.f, 0.f);
	Module.AddMinSpeed = 100.f;
	Module.AddMaxSpeed = 500.f;

	// Drag Module (감속)
	Module.Module[(UINT)PARTICLE_MODULE::DRAG] = false;
	Module.DestNormalizedAge = 1.f;
	Module.LimitSpeed = 0.f;

	// Noise Force Module
	Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	Module.NoiseForceTerm = 0.3f;
	Module.NoiseForceScale = 100.f;

	// Render Module
	Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
	Module.EndColor = Vector3(10.f, 1.f, 1.f);
	Module.FadeOut = true;
	Module.StartRatio = 0.9f;
	Module.VelocityAlignment = false;

	return Module;
}

void CMouseScript::Init()
{
	Collider2D()->SetActive(false);

	m_ParticleChild = new CGameObject;

	m_ParticleChild->SetName(L"HitParticle");
	m_ParticleChild->AddComponent(new CParticleSystemScript);
	m_ParticleChild->AddComponent(new CParticleSystem);
	m_HitParticle = m_ParticleChild->ParticleSystem();
	m_ParticleChild->SetLayerIdx(0);

	GetOwner()->AddChild(m_ParticleChild);


	CGameObject* pObj = new CGameObject;

	pObj->SetName(L"DSParticle");
	pObj->AddComponent(new CParticleSystemScript);
	pObj->AddComponent(new CParticleSystem);
	m_DSParticle = pObj->ParticleSystem();
	pObj->SetLayerIdx(0);

	GetOwner()->AddChild(pObj);

	m_SkillHitTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"SkillHit");
	m_DamageSkinTex[0] = CAssetMgr::GetInst()->Load<CTexture>(L"DamageSkin_0", L"Texture//maple//number//0.png");
	m_DamageSkinTex[1] = CAssetMgr::GetInst()->Load<CTexture>(L"DamageSkin_1", L"Texture//maple//number//1.png");
	m_DamageSkinTex[2] = CAssetMgr::GetInst()->Load<CTexture>(L"DamageSkin_2", L"Texture//maple//number//2.png");
	m_DamageSkinTex[3] = CAssetMgr::GetInst()->Load<CTexture>(L"DamageSkin_3", L"Texture//maple//number//3.png");
	m_DamageSkinTex[4] = CAssetMgr::GetInst()->Load<CTexture>(L"DamageSkin_4", L"Texture//maple//number//4.png");
	m_DamageSkinTex[5] = CAssetMgr::GetInst()->Load<CTexture>(L"DamageSkin_5", L"Texture//maple//number//5.png");
	m_DamageSkinTex[6] = CAssetMgr::GetInst()->Load<CTexture>(L"DamageSkin_6", L"Texture//maple//number//6.png");
	m_DamageSkinTex[7] = CAssetMgr::GetInst()->Load<CTexture>(L"DamageSkin_7", L"Texture//maple//number//7.png");
	m_DamageSkinTex[8] = CAssetMgr::GetInst()->Load<CTexture>(L"DamageSkin_8", L"Texture//maple//number//8.png");
	m_DamageSkinTex[9] = CAssetMgr::GetInst()->Load<CTexture>(L"DamageSkin_9", L"Texture//maple//number//9.png");

	m_HitParticle->SetParticleTexture(m_SkillHitTex);
	m_HitParticle->SetMaxCount(500);
	m_HitParticle->SetModule(InitModule());
	/*m_Particle->GetModule().SpaceType = 2;
	m_Particle->GetModule().VelocityAlignment = 0;*/

	m_HitParticle->SetModuleData();
}

void CMouseScript::Begin()
{
}

void CMouseScript::Tick()
{
	{
		//// 마우스 정보 전송
		//Vector2 vResolution = CDevice::GetInst()->GetRenderResolution() / 2;

		//Vector2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
		//Vector2 vMouseWorldPos = CKeyMgr::GetInst()->GetMouseWorldPos();
		//GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(VEC2_0, vMousePos);
		//GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(VEC2_1, vMouseWorldPos);



		//if (nullptr != m_Texture->MeshRender()->GetMaterial()->GetTexture(0))
		//{
		//	if (L"BeachTex" == m_Texture->MeshRender()->GetMaterial()->GetTexture(0)->GetKey())
		//	{
		//		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(VEC2_2, Vector2(360.f, 220.f));
		//		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(VEC2_3, Vector2(-680.f, 260.f));
		//	}
		//	else
		//	{
		//		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(VEC2_2, Vector2(-285.f, -250.f));
		//		GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(VEC2_3, Vector2(840.f, -160.f));
		//	}
		//}

		//if (KEY_PRESSED(KEY::LBTN))
		//{
		//	m_Time += DT * 100.f;
		//}
		//if (KEY_RELEASED(KEY::LBTN))
		//{
		//	m_Time = 0;
		//}
		//GetOwner()->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, m_Time);
	}

	Transform()->SetWorldPos(Vector3(g_Data.MouseWorldPos.x, g_Data.MouseWorldPos.y, Transform()->GetWorldPos().z));

	if (KEY_TAP(KEY::LBTN))
	{
		Collider2D()->SetActive(true);
		//m_Particle->SetMaxCount(5);
		//m_Particle->SetModuleData();
	}
	if (KEY_RELEASED(KEY::LBTN))
	{
		Collider2D()->SetActive(false);
		//m_Particle->SetMaxCount(0);
		//m_Particle->SetModuleData();
	}
}

void CMouseScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

	m_HitParticle->SetParticleTexture(m_SkillHitTex);

	m_HitParticle->GetModule().SpaceType = 0;
	m_HitParticle->GetModule().SpawnBurstRepeatTime = 1;
	m_HitParticle->GetModule().VelocityAlignment = 0;
	m_HitParticle->GetModule().SpawnShapeScale = Vector3(100.f, 0.f, 0.f);
	m_HitParticle->SetModuleData();

	m_DSParticle->SetParticleTexture(m_DamageSkinTex[0]);
	m_DSParticle->GetModule().SpaceType = 0;
	m_DSParticle->GetModule().AddVelocityType = 3;
	m_DSParticle->GetModule().vSpawnMaxScale = Vector4(30.f, 30.f, 0.f, 0.f);
	m_DSParticle->GetModule().vSpawnMinScale = Vector4(30.f, 30.f, 0.f, 0.f);
	m_DSParticle->GetModule().SpawnShapeScale = Vector4(0.f, 0.f, 0.f, 0.f);
	m_DSParticle->GetModule().MaxLife = 1;
	m_DSParticle->GetModule().MinLife = 1;
	m_DSParticle->GetModule().AddMaxSpeed = 20.f;
	m_DSParticle->GetModule().AddMinSpeed = 20.f;
	m_DSParticle->GetModule().VelocityAlignment = 0;
	m_DSParticle->GetModule().SpawnBurstRepeatTime = 1;
	m_DSParticle->GetModule().SpawnShapeScale = Vector3(100.f, 0.f, 0.f);
	static int random = 0;
	m_DSParticle->SetParticleTexture(m_DamageSkinTex[random++ % 10]);
	m_DSParticle->SetModuleData();

	if (KEY_TAP(KEY::LBTN))
	{
		Collider2D()->SetActive(true);
		m_HitParticle->SetMaxCount(5);
		m_HitParticle->SetModuleData();

		m_DSParticle->SetMaxCount(5);
		m_DSParticle->SetModuleData();
	}
	if (KEY_RELEASED(KEY::LBTN))
	{
		Collider2D()->SetActive(false);
		m_HitParticle->SetMaxCount(0);
		m_HitParticle->SetModuleData();
		m_DSParticle->SetMaxCount(0);
		m_DSParticle->SetModuleData();
	}
	m_ParticleChild->Transform()->SetWorldPos(_OtherObject->Transform()->GetWorldPos());
	m_DSParticle->GetOwner()->Transform()->SetWorldPos(_OtherObject->Transform()->GetWorldPos() + Vector3(0.f, 1.f, 0.f) * 50.f);
}

void CMouseScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	m_ParticleChild->Transform()->SetWorldPos(_OtherObject->Transform()->GetWorldPos());
	m_DSParticle->GetOwner()->Transform()->SetWorldPos(_OtherObject->Transform()->GetWorldPos() + Vector3(0.f, 1.f, 0.f) * 50.f);

	if (KEY_TAP(KEY::LBTN))
	{
		Collider2D()->SetActive(true);
		m_HitParticle->SetMaxCount(5);
		m_HitParticle->SetModuleData();
		m_DSParticle->SetMaxCount(5);
		m_DSParticle->SetModuleData();
	}
	if (KEY_RELEASED(KEY::LBTN))
	{
		Collider2D()->SetActive(false);
		m_HitParticle->SetMaxCount(0);
		m_HitParticle->SetModuleData();
		m_DSParticle->SetMaxCount(0);
		m_DSParticle->SetModuleData();
	}
}

void CMouseScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	m_ParticleChild->Transform()->SetWorldPos(_OtherObject->Transform()->GetWorldPos());
	m_DSParticle->GetOwner()->Transform()->SetWorldPos(_OtherObject->Transform()->GetWorldPos() + Vector3(0.f, 1.f, 0.f) * 50.f);

	if (KEY_TAP(KEY::LBTN))
	{
		Collider2D()->SetActive(true);
		m_HitParticle->SetMaxCount(5);
		m_HitParticle->SetModuleData();
		m_DSParticle->SetMaxCount(5);
		m_DSParticle->SetModuleData();
	}
	if (KEY_RELEASED(KEY::LBTN))
	{
		Collider2D()->SetActive(false);
		m_HitParticle->SetMaxCount(0);
		m_HitParticle->SetModuleData();
		m_DSParticle->SetMaxCount(0);
		m_DSParticle->SetModuleData();
	}

	//m_Particle->ClearParticle();
}


