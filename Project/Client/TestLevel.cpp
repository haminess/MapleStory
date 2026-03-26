#include "pch.h"
#include "TestLevel.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include <Engine/CCollisionMgr.h>

#include <Scripts/CLevelScript.h>
#include <Scripts/CPlayerScript.h>
#include <Scripts/CSkillManager.h>
#include <Scripts/CPlatformerScript.h>
#include <Scripts/CMissileScript.h>
#include <Scripts/CCameraScript.h>
#include <Scripts/CPlatformScript.h>
#include <Scripts/CParticleSystemScript.h>
#include <Scripts/CMonsterScript.h>
#include <Scripts/CMouseScript.h>
#include <Scripts/CLinePlatform.h>
#include <Engine/MTileMap.h>
#include <Scripts/CLineWall.h>
#include <Scripts/CRope.h>
#include <Scripts/CIdleState.h>
#include <Scripts/CAttackState.h>
#include <Scripts/CTraceState.h>
#include <Scripts/CPatrolState.h>
#include <Scripts/CHitState.h>
#include <Scripts/CDieState.h>

#include <Scripts/CHeroSlashBlastSkill.h>

#include <Engine/CClearColorCS.h>
#include <Engine/CStructuredBuffer.h>
#include <Engine/CPrefab.h>


void TestLevel::CreateTestLevel()
{
	// Texture 로딩하기
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(L"PlayerTex", L"Texture//Character.png");
	Ptr<CTexture> pNoiseTex = CAssetMgr::GetInst()->Load<CTexture>(L"NoiseTex", L"Texture//noise//noise_03.jpg");
	Ptr<CTexture> pAtlasTex = CAssetMgr::GetInst()->Load<CTexture>(L"TileMapTex", L"Texture//TILE.bmp");
	Ptr<CTexture> pSpaceTex = CAssetMgr::GetInst()->Load<CTexture>(L"SpaceTex", L"Texture//space.jpeg");
	Ptr<CTexture> pBeachTex = CAssetMgr::GetInst()->Load<CTexture>(L"BeachTex", L"Texture//beach.jpeg");
	Ptr<CTexture> pWaterTex = CAssetMgr::GetInst()->Load<CTexture>(L"WaterTex", L"Texture//water.jpeg");
	Ptr<CTexture> pSnailTex = CAssetMgr::GetInst()->Load<CTexture>(L"SnailTex", L"Texture//maple//stand.png");


	// Texture 생성하기
	Ptr<CTexture> pTestTex =
		CAssetMgr::GetInst()->CreateTexture(L"TestTexture", 1055, 1055
			, DXGI_FORMAT_R8G8B8A8_UNORM
			, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);


	// Sound 로딩하기
	Ptr<CSound> pSound = CAssetMgr::GetInst()->Load<CSound>(L"Sound\\DM.wav", L"Sound\\DM.wav");
	//pSound->Play(0, 0.5f, true);



	// 레벨 생성
	CLevel* pLevel = new CLevel;
	pLevel->SetName(L"Test");

	// 테스트 레벨을 현재 레벨로 지정
	ChangeLevel(pLevel, LEVEL_STATE::PLAY);

	pLevel->GetLayer((int)LAYER_INDEX::BACKGROUND)->SetName(L"Background");
	pLevel->GetLayer((int)LAYER_INDEX::PLATFORM)->SetName(L"Ground");
	pLevel->GetLayer((int)LAYER_INDEX::DEFAULT)->SetName(L"Default");
	pLevel->GetLayer((int)LAYER_INDEX::PLAYER)->SetName(L"Player");
	pLevel->GetLayer((int)LAYER_INDEX::PLAYER_OBJECT)->SetName(L"PlayerObject");
	pLevel->GetLayer((int)LAYER_INDEX::MONSTER)->SetName(L"Monster");
	pLevel->GetLayer((int)LAYER_INDEX::MONSTER_OBJECT)->SetName(L"MonsterObject");

	// 레벨 오브젝트 생성
	CGameObject* pObject = nullptr;

	// MainCamera
	CGameObject* pCam = nullptr;
	pCam = new CGameObject;
	pCam->SetName(L"MainCamera");
	pCam->AddComponent(new CCamera);
	pCam->Transform()->SetRelativePos(0.f, 0.f, -100.f);
	CCameraScript* pCamScript = (CCameraScript*)pCam->AddComponent(new CCameraScript);

	// MainCamera 로 설정
	pCam->Camera()->SetPriority(0);

	// 모든 레이어를 찍도록 설정
	pCam->Camera()->LayerCheckAll();

	// 카메라 투영방식 설정
	pCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	pLevel->AddObject(0, pCam, true);
	//CreateObject(pObject, 0, false);


	// =================
	// 광원 오브젝트 추가
	// =================
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");
	pLightObj->AddComponent(new CLight2D);

	pLightObj->Transform()->SetRelativePos(-100.f, 0.f, 10.f);
	pLightObj->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light2D()->SetLightColor(Vector3(1.f, 1.f, 1.f));
	pLightObj->Light2D()->SetRadius(200.f);

	CreateObject(pLightObj, 0, false);



	// =============
	//// PostProcess
	//CGameObject* pMouse = new CGameObject;
	//pMouse->SetName(L"MousePostProcess");
	//pMouse->AddComponent(new CMeshRender);
	//pMouse->AddComponent(new CCollider2D);
	//pMouse->AddComponent(new CMouseScript);

	//pMouse->Transform()->SetRelativePos(Vector3(0.f, 0.f, 0.f));
	//pMouse->Transform()->SetRelativeScale(Vector3(300.f, 300.f, 1.f));

	//pMouse->Collider2D()->SetIndependentScale(false);
	//pMouse->Collider2D()->SetScale(Vector2(1.f, 1.f));

	//pMouse->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
	//pMouse->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"WaveMtrl"));

	//pMouse->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	////pMouse->MeshRender()->GetMaterial()->SetTexParam(TEX_1, pNoiseTex);
	//pMouse->MeshRender()->GetMaterial()->SetScalarParam(VEC2_2, Vector2(360.f, 220.f));
	//pMouse->MeshRender()->GetMaterial()->SetScalarParam(VEC2_3, Vector2(-680.f, 260.f));
	//pMouse->MeshRender()->GetMaterial()->SetScalarParam(VEC2_2, Vector2(-285.f, -250.f));
	//pMouse->MeshRender()->GetMaterial()->SetScalarParam(VEC2_3, Vector2(840.f, -160.f));
	//pLevel->AddObject(2, pMouse, false);


	//// PostProcess2
	//pPostProcess = new CGameObject;
	//pPostProcess->SetName(L"PostProcess");
	//pPostProcess->AddComponent(new CMeshRender);
	//pPostProcess->AddComponent(new CMouseScript);

	//pPostProcess->Transform()->SetRelativePos(Vector3(0.f, 0.f, 0.f));
	//pPostProcess->Transform()->SetRelativeScale(Vector3(3000.f, 1920.f, 1.f));

	//pPostProcess->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pPostProcess->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DistortionMtrl"));

	//pPostProcess->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex"));
	//pPostProcess->MeshRender()->GetMaterial()->SetTexParam(TEX_1, pNoiseTex);
	//m_CurLevel->AddObject(2, pPostProcess, false);


	//// LineObject
	//pObject = new CGameObject;
	//pObject->SetName(L"LinePlatform");
	//pObject->AddComponent(new CCollider2D);
	//pObject->AddComponent(new CLinePlatform);
	//pObject->Collider2D()->SetType(COLLIDER_TYPE::LINE);
	//pObject->Transform()->SetRelativePos(Vector3(0.f, 100.f, 1.f));
	//pObject->Transform()->SetRelativeScale(Vector3(100.f, 100.f, 1.f));
	//pObject->Collider2D()->SetLinePos(Vector2(-10.f, 1.f), Vector2(10.f, 1.f));
	//pObject->GetScript<CLinePlatform>()->SetPos(Vector2(-500.f, -0.f), Vector2(500.f, 0.f));
	//pObject->Collider2D()->SetIndependentScale(true);
	//CreateObject(pObject, 1, false);



	// Rope
	pObject = new CGameObject;
	pObject->SetName(L"Rope");
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CRope);
	pObject->Transform()->SetRelativePos(Vector3(100.f, 0.f, 1.f));
	pObject->Transform()->SetRelativeScale(Vector3(50.f, 600.f, 1.f));
	pObject->Collider2D()->SetIndependentScale(false);
	pObject->Collider2D()->SetScale(Vector2(1.f, 1.f));
	CreateObject(pObject, (int)LAYER_INDEX::DEFAULT, false);

	///////////////////////////
	//// Maple Tile Object //// 
	///////////////////////////
	//pObject = new CGameObject;
	//pObject->SetName(L"TempTile");
	//pObject->AddComponent(new MTileMap);
	//pObject->Transform()->SetRelativePos(Vector3(0.f, 0.f, -100.f));
	//pObject->Transform()->SetRelativeScale(Vector3(600.f, 600.f, 1.f));
	//CreateObject(pObject, (int)LAYER_INDEX::PLATFORM, false);



	// ==== Player ====
	CGameObject* pPlayer = new CGameObject;
	pPlayer->SetName(L"Player");

	pPlayer->AddComponent(new CPlatformerScript);
	pPlayer->AddComponent(new CCollider2D);
	pPlayer->AddComponent(new CRigidBody2D);
	pPlayer->AddComponent(new CMeshRender);
	pPlayer->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	pPlayer->AddComponent(new CFlipbookPlayer);

	CSkillManager* pSM = new CSkillManager;
	pPlayer->AddComponent(pSM);


	pPlayer->Transform()->SetRelativePos(0.f, 200.f, 0.f);
	pPlayer->Transform()->SetRelativeScale(80.f, 80.f, 1.f);

	pPlayer->Collider2D()->SetType(COLLIDER_TYPE::BOX);
	pPlayer->Collider2D()->SetIndependentScale(true);
	pPlayer->Collider2D()->SetOffset(Vector2(0.f, 5.f));
	pPlayer->Collider2D()->SetScale(Vector2(10.f, 10.f));

	pPlayer->RigidBody2D()->UseGravity(true);
	pPlayer->RigidBody2D()->SetGravityAccel(2300.f);
	pPlayer->RigidBody2D()->SetFriction(1500.f);



	// 카메라 플레이어 따라가기
	pCamScript->SetTarget(pPlayer);

	pPlayer->AddComponent(new CPlayerScript);

	// 스킬 추가
	//CGameObject* pSkill = new CGameObject;
	//pSkill->SetName(L"MaewhaSkill");
	//pSkill->AddComponent(new CRenMaewhaSkill);
	//pPlayer->AddChild(pSkill);

	//// Child
	//CGameObject* pMonster = new CGameObject;
	//pMonster->SetName(L"Monster");
	//pMonster->AddComponent(new CMeshRender);
	//pMonster->AddComponent(new CFlipbookPlayer);
	//pMonster->AddComponent(new CCollider2D);
	//pMonster->AddComponent(new CStateMachine);
	//pMonster->AddComponent(new CRigidBody2D);
	//pMonster->AddComponent(new CMonsterScript);
	//pMonster->AddComponent(new CPlatformerScript);

	//pMonster->Collider2D()->SetType(COLLIDER_TYPE::BOX);
	//pMonster->Collider2D()->SetIndependentScale(true);
	//pMonster->Collider2D()->SetOffset(Vector2(0.f, 0.f));
	//pMonster->Collider2D()->SetScale(Vector2(5.f, 5.f));

	//pMonster->StateMachine()->AddState(L"IdleState", new CIdleState);
	//pMonster->StateMachine()->AddState(L"AttackState", new CAttackState);
	//pMonster->StateMachine()->AddState(L"TraceState", new CTraceState);
	//pMonster->StateMachine()->AddState(L"PatrolState", new CPatrolState);
	//pMonster->StateMachine()->AddState(L"HitState", new CHitState);
	//pMonster->StateMachine()->AddState(L"DieState", new CDieState);

	//pMonster->Transform()->SetRelativePos(2.f, 200.f, 0.f);
	//pMonster->Transform()->SetRelativeScale(40.f, 40.f, 1.f);

	//CGameObject* pChild = new CGameObject;
	//pChild->SetName(L"MonsterBody");
	//pChild->AddComponent(new CCollider2D);
	//pChild->Collider2D()->SetScale(Vector2(50.f, 50.f));
	//pChild->Collider2D()->SetOffset(Vector2(0.f, 25.f));
	//pChild->Collider2D()->SetIndependentScale(true);
	//pMonster->AddChild(pChild);

	//pChild = new CGameObject;
	//pChild->SetName(L"MonsterDetectRange");
	//pChild->AddComponent(new CCollider2D);
	//pChild->Collider2D()->SetScale(Vector2(100.f, 100.f));
	//pChild->Collider2D()->SetOffset(Vector2(0.f, 25.f));
	//pChild->Collider2D()->SetIndependentScale(true);
	//pMonster->AddChild(pChild);



	// 계층관계 설정
	//CreateObject(pMonster, (int)LAYER_INDEX::MONSTER, true);
	CreateObject(pPlayer, (int)LAYER_INDEX::PLAYER, true);

	// Plaform
	pObject = new CGameObject;
	pObject->AddComponent(new CPlatformScript);
	pObject->GetScript<CPlatformScript>()->SetPlatformPos(Vector3(-600.f, 500.f, 0.f), Vector3(-500.f, 500.f, 0.f), 600.f);
	CreateObject(pObject, (int)LAYER_INDEX::PLATFORM, false);

	pObject = new CGameObject;
	pObject->AddComponent(new CPlatformScript);
	pObject->GetScript<CPlatformScript>()->SetPlatformPos(Vector3(-2000.f, -100.f, 0.f), Vector3(2000.f, -100.f, 0.f), 50.f);
	CreateObject(pObject, (int)LAYER_INDEX::PLATFORM, false);

	pObject = new CGameObject;
	pObject->AddComponent(new CPlatformScript);
	pObject->GetScript<CPlatformScript>()->SetPlatformPos(Vector3(-100.f, -100.f, 0.f), Vector3(0.f, 0.f, 0.f), 50.f);
	CreateObject(pObject, (int)LAYER_INDEX::PLATFORM, false);

	pObject = new CGameObject;
	pObject->AddComponent(new CPlatformScript);
	pObject->GetScript<CPlatformScript>()->SetPlatformPos(Vector3(0.f, 0.f, 0.f), Vector3(100.f, 0.f, 0.f), 50.f);
	CreateObject(pObject, (int)LAYER_INDEX::PLATFORM, false);

	pObject = new CGameObject;
	pObject->AddComponent(new CPlatformScript);
	pObject->GetScript<CPlatformScript>()->SetPlatformPos(Vector3(100.f, 0.f, 0.f), Vector3(200.f, -100.f, 0.f), 50.f);
	CreateObject(pObject, (int)LAYER_INDEX::PLATFORM, false);

	pObject = new CGameObject;
	pObject->AddComponent(new CPlatformScript);
	pObject->GetScript<CPlatformScript>()->SetPlatformPos(Vector3(200.f, 500.f, 0.f), Vector3(300.f, 500.f, 0.f), 600.f);
	CreateObject(pObject, (int)LAYER_INDEX::PLATFORM, false);




	///////////////////////////
	////  Particle Object  //// 
	///////////////////////////
	//CGameObject* pParticle = new CGameObject;
	//
	//pParticle->SetName(L"Particle");
	//pParticle->AddComponent(new CParticleSystemScript);

	//CreateObject(pParticle, 2, false);


	///////////////////////////
	////    Tile Object	   //// 
	///////////////////////////
	//CGameObject* pTestObject = new CGameObject;
	//pTestObject->SetName(L"TileObject");

	//CreateObject(pTestObject, 0, true);

	//for (int i = 0; i < 2500; ++i)
	//{
	//	CGameObject* pTile = new CGameObject;
	//	AddChild(pTestObject, pTile);

	//	wstring name = L"Tile_" + std::to_wstring(i);
	//	pTile->SetName(name);

	//	// 타일을 실제로 렌더링할 자식 오브젝트
	//	for (int j = 0; j < 2; ++j)
	//	{
	//		CGameObject* pModel = new CGameObject;
	//		AddChild(pTile, pModel);

	//		wstring name = L"Tex_" + std::to_wstring(j);
	//		pModel->SetName(name);
	//	}
	//}

	
	CCollisionMgr::GetInst()->CollisionCheck((int)LAYER_INDEX::PLATFORM, (int)LAYER_INDEX::PLAYER);
	CCollisionMgr::GetInst()->CollisionCheck((int)LAYER_INDEX::PLAYER, (int)LAYER_INDEX::WALL);
	CCollisionMgr::GetInst()->CollisionCheck((int)LAYER_INDEX::PLATFORM, (int)LAYER_INDEX::MONSTER);
	CCollisionMgr::GetInst()->CollisionCheck((int)LAYER_INDEX::PLAYER, (int)LAYER_INDEX::MONSTER);
	CCollisionMgr::GetInst()->CollisionCheck((int)LAYER_INDEX::PLAYER, (int)LAYER_INDEX::MONSTER_OBJECT);
	CCollisionMgr::GetInst()->CollisionCheck((int)LAYER_INDEX::PLAYER, (int)LAYER_INDEX::DEFAULT);
	CCollisionMgr::GetInst()->CollisionCheck((int)LAYER_INDEX::PLAYER_OBJECT, (int)LAYER_INDEX::MONSTER);
	CCollisionMgr::GetInst()->CollisionCheck((int)LAYER_INDEX::MONSTER, (int)LAYER_INDEX::DEFAULT);

	// 레벨 상태 변경
	//::ChangeLevelState(LEVEL_STATE::PLAY);
}

CLevel* TestLevel::CreateEmptyLevel()
{
	// 레벨 생성
	CLevel* pLevel = new CLevel;

	pLevel->GetLayer((int)LAYER_INDEX::BACKGROUND)->SetName(L"Background");
	pLevel->GetLayer((int)LAYER_INDEX::PLATFORM)->SetName(L"Ground");
	pLevel->GetLayer((int)LAYER_INDEX::DEFAULT)->SetName(L"Default");
	pLevel->GetLayer((int)LAYER_INDEX::PLAYER)->SetName(L"Player");
	pLevel->GetLayer((int)LAYER_INDEX::PLAYER_OBJECT)->SetName(L"PlayerObject");
	pLevel->GetLayer((int)LAYER_INDEX::MONSTER)->SetName(L"Monster");
	pLevel->GetLayer((int)LAYER_INDEX::MONSTER_OBJECT)->SetName(L"MonsterObject");


	// MainCamera
	CGameObject* pCam = nullptr;
	pCam = new CGameObject;
	pCam->SetName(L"MainCamera");
	pCam->AddComponent(new CCamera);
	pCam->Transform()->SetRelativePos(0.f, 0.f, -100.f);
	CCameraScript* pCamScript = (CCameraScript*)pCam->AddComponent(new CCameraScript);

	// MainCamera 로 설정
	pCam->Camera()->SetPriority(0);

	// 모든 레이어를 찍도록 설정
	pCam->Camera()->LayerCheckAll();

	// 카메라 투영방식 설정
	pCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);

	pLevel->AddObject(0, pCam, true);


	// =================
	// 광원 오브젝트 추가
	// =================
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Directional Light");
	pLightObj->AddComponent(new CLight2D);

	pLightObj->Transform()->SetRelativePos(-100.f, 0.f, 10.f);
	pLightObj->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light2D()->SetLightColor(Vector3(1.f, 1.f, 1.f));
	pLightObj->Light2D()->SetRadius(200.f);

	pLevel->AddObject(0, pLightObj, false);

	CGameObject* pLevelObj = new CGameObject;
	pLevelObj->SetName(L"LevelMgr");
	pLevelObj->AddComponent(new CLevelScript);

	pLevel->AddObject(0, pLevelObj, false);

	return pLevel;
}
