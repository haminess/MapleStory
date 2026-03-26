#include "pch.h"
#include "CAssetMgr.h"
#include "CDevice.h"
#include "CGameObject.h"
#include "CMeshRender.h"

void CAssetMgr::Init()
{
	CreateEngineMesh();
	CreateEngineTexture();
	CreateEngineGraphicShader();
	CreateEngineComputeShader();
	CreateEngineMaterial();
	CreateEngineSprite();
	CreateEnginePrefab();
}

void CAssetMgr::CreateEngineMesh()
{
	Ptr<CMesh> pMesh = nullptr;

	// =========
	// PointMesh
	// =========
	Vertex v;
	UINT i = 0;

	pMesh = new CMesh(true);
	pMesh->Create(&v, 1, &i, 1);
	AddAsset(L"PointMesh", pMesh);


	// =========
	// Rect Mesh
	// 4 개의 정점의 위치 설계
	// 0 -- 1
	// | \  |
	// 3 -- 2   
	// =========	
	Vertex arrVtx[4] = {};
	arrVtx[0].vPos = Vector3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vUV = Vector2(0.f, 0.f);
	arrVtx[0].vColor = Vector4(1.f, 0.f, 0.f, 1.f);

	arrVtx[1].vPos = Vector3(0.5f, 0.5f, 0.f);
	arrVtx[1].vUV = Vector2(1.f, 0.f);
	arrVtx[1].vColor = Vector4(0.f, 0.f, 1.f, 1.f);

	arrVtx[2].vPos = Vector3(0.5f, -0.5f, 0.f);
	arrVtx[2].vUV = Vector2(1.f, 1.f);
	arrVtx[2].vColor = Vector4(0.f, 1.f, 0.f, 1.f);

	arrVtx[3].vPos = Vector3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vUV = Vector2(0.f, 1.f);
	arrVtx[3].vColor = Vector4(1.f, 0.f, 1.f, 1.f);

	// 인덱스
	UINT Idx[6] = { 0 , 1 , 2, 0 , 2 , 3 };

	// 클래스 or 구조체 + 가상함수
	const type_info& info = typeid(CMesh);
	const char* pName = info.name();

	pMesh = new CMesh(true);
	pMesh->Create(arrVtx, 4, Idx, 6);
	AddAsset(L"RectMesh", pMesh);

	// ==============
	// RectMesh_Debug
	// 4 개의 정점의 위치 설계
	// 0 -- 1
	// | \  |
	// 3 -- 2   
	// ==============
	UINT DebugRectIdx[5] = { 0 , 1 , 2, 3 , 0 };

	pMesh = new CMesh(true);
	pMesh->Create(arrVtx, 4, DebugRectIdx, 5);
	AddAsset(L"RectMesh_Debug", pMesh);

	// ==============
	// LineMesh
	// 2 개의 정점의 위치 설계
	// 0 -- 1 
	// ==============
	arrVtx[0].vPos = Vector3(0.f, 0.f, 0.f);
	arrVtx[0].vUV = Vector2(0.f, 0.f);
	arrVtx[0].vColor = Vector4(1.f, 1.f, 1.f, 1.f);

	arrVtx[1].vPos = Vector3(1.f, 0.f, 0.f);
	arrVtx[1].vUV = Vector2(1.f, 0.f);
	arrVtx[1].vColor = Vector4(1.f, 1.f, 1.f, 1.f);
	UINT DebugLineIdx[2] = { 0 , 1 };

	pMesh = new CMesh(true);
	pMesh->Create(arrVtx, 2, DebugLineIdx, 2);
	AddAsset(L"LineMesh", pMesh);


	// ==========
	// CircleMesh
	// ==========
	vector<Vertex>		vecVtx;
	vector<UINT>	vecIdx;

	UINT Slice = 40;
	float Radius = 0.5f;
	float Theta = 0.f;

	// 중심점
	v.vPos = Vector3(0.f, 0.f, 0.f);
	v.vUV = Vector2(0.5f, 0.5f);
	vecVtx.push_back(v);

	for (UINT i = 0; i < Slice + 1; ++i)
	{
		v.vPos = Vector3(Radius * cosf(Theta), Radius * sinf(Theta), 0.f);
		v.vUV = Vector2(v.vPos.x + 0.5f, 1.f - (v.vPos.y + 0.5f));
		vecVtx.push_back(v);

		Theta += (XM_PI * 2.f) / Slice;
	}

	for (UINT i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 1);
		vecIdx.push_back(i + 2);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CircleMesh", pMesh);

	// =================
	// CircleMesh_Debug
	// =================	
	vecIdx.clear();
	for (size_t i = 0; i < vecVtx.size() - 1; ++i)
	{
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh(true);
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddAsset(L"CircleMesh_Debug", pMesh);
}

void CAssetMgr::CreateEngineTexture()
{
	// 텍스쳐 생성
	
	// NoiseTexture
	Load<CTexture>(L"NoiseTex", L"Texture//noise//noise_03.jpg");

	Load<CTexture>(L"SkillHit", L"Texture//maple//skill_hit.png");
	Load<CTexture>(L"DamageSkin", L"Texture//maple//number//0.png");
	Load<CTexture>(L"DebugRectTex", L"Texture\\DebugRectTex.png");

	// PostPorcessTexture
	Vector2 vResolution = CDevice::GetInst()->GetRenderResolution();
	CreateTexture(L"PostProcessTex", vResolution.x, vResolution.y
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	// TextureArray
	// Damage Skin
	vector<Ptr<CTexture>> vecTexArray;

	vecTexArray.resize(10);
	for (int i = 0; i < vecTexArray.size(); ++i)
	{
		wstring path = L"Texture\\DamageSkin\\default\\" + std::to_wstring(i) + L".png";
		vecTexArray[i] = CAssetMgr::GetInst()->Load<CTexture>(path, path);
	}

	Ptr<CTexture> pDamageSkin = new CTexture;
	pDamageSkin->CreateArrayTexture(vecTexArray);

	AddAsset<CTexture>(L"DamageSkinTex", pDamageSkin);

	// Tile
	Ptr<MTile> pTile = nullptr;

	pTile = new MTile(true);
	pTile->LoadTileTexture(L"fallenLeaves");
	AddAsset<MTile>(L"fallenLeaves", pTile);

	AddAsset<CTexture>(L"DefaultTileTex", pTile->GetTexArray());
	

	pTile = new MTile(true);
	pTile->LoadTileTexture(L"yellowToyCastle");
	AddAsset<MTile>(L"yellowToyCastle", pTile);

	pTile = new MTile(true);
	pTile->LoadTileTexture(L"arcana1");
	AddAsset<MTile>(L"arcana1", pTile);

	pTile = new MTile(true);
	pTile->LoadTileTexture(L"grassySoil");
	AddAsset<MTile>(L"grassySoil", pTile);

	pTile = new MTile(true);
	pTile->LoadTileTexture(L"woodBridge");
	AddAsset<MTile>(L"woodBridge", pTile);

	pTile = new MTile(true);
	pTile->LoadTileTexture(L"darkWood");
	AddAsset<MTile>(L"darkWood", pTile);
}


void CAssetMgr::CreateEngineGraphicShader()
{
	Ptr<CGraphicShader> pShader = nullptr;

	// ===========================
	// Std2DShader : 표준 2D 쉐이더
	// ===========================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddScalarParam("Test Parameter", VEC2_2, true);
	pShader->AddTexParam("Output Texture", TEX_0);

	AddAsset(L"Std2DShader", pShader);


	// ================================================
	// Std2DAlphaBlendShader : 표준 알파블렌드 2D 쉐이더
	// ================================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D_AlphaBlend");
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::LESS_EQUAL);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	AddAsset(L"Std2DAlphaBlendShader", pShader);


	// =================================
	// Std2D_PaperBurn : PaperBurn 쉐이더
	// =================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_Std2D_PaperBurn");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	AddAsset(L"Std2DPaperBurnShader", pShader);


	// ==================================
	// TileMapShader : 타일맵 전용 쉐이더
	// ==================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"Shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::LESS);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
	AddAsset(L"TileMapShader", pShader);

	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\tilemap_maple.fx", "VS_MTile");
	pShader->CreateGeometryShader(L"Shader\\tilemap_maple.fx", "GS_MTile");
	pShader->CreatePixelShader(L"Shader\\tilemap_maple.fx", "PS_MTile");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_PARTICLE);
	AddAsset(L"MTileShader", pShader);

	// ===========
	// PostProcess
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Post");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Post");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"PostProcessShader", pShader);



	// ===========
	// Distortion
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Distortion");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Distortion");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"DistortionShader", pShader);



	// ===========
	// Vortex
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Vortex");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Vortex");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"VortexShader", pShader);


	// ===========
	// Enlarge
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Enlarge");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Enlarge");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"EnlargeShader", pShader);

	// ===========
	// Wave
	// ===========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Wave");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Wave");

	pShader->SetBSState(BS_TYPE::DEFAULT);
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetRSState(RS_TYPE::CULL_NONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"WaveShader", pShader);


	// =================================
	// DebugShapeShader : 디버그용 쉐이더
	// =================================
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\debug.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"Shader\\debug.fx", "PS_DebugShape");
	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	AddAsset(L"DebugShapeShader", pShader);


	// ==============
	// ParticleShader
	// ==============
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"Shader\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(L"Shader\\particle.fx", "PS_Particle");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_PARTICLE);
	AddAsset(L"ParticleShader", pShader);

	// ==========
	// DamageSkin
	// ==========
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"Shader\\std2d.fx", "VS_DamageSkin");
	pShader->CreatePixelShader(L"Shader\\std2d.fx", "PS_DamageSkin");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	pShader->SetRSState(RS_TYPE::CULL_NONE);
	pShader->SetBSState(BS_TYPE::ALPHABLEND);
	pShader->SetDSState(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	AddAsset(L"DamageSkinShader", pShader);

	// ====
	// Fade
	// ====
	pShader = new CGraphicShader;

	pShader->CreateVertexShader(L"Shader\\postprocess.fx", "VS_Post");
	pShader->CreatePixelShader(L"Shader\\postprocess.fx", "PS_Fade");

	pShader->SetRSState(RS_TYPE::CULL_NONE);                    
	pShader->SetBSState(BS_TYPE::ALPHABLEND);                   
	pShader->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);             
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);      

	AddAsset(L"FadeShader", pShader);
}

#include "CParticleTickCS.h"

void CAssetMgr::CreateEngineComputeShader()
{
	AddAsset<CComputeShader>(L"ParticleTickCS", new CParticleTickCS);
}

void CAssetMgr::CreateEngineMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	// Std2DMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"Std2DMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// Std2DAlphaBlendMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"Std2DAlphaBlendMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DAlphaBlendShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// Std2DPaperBurnMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"Std2DPaperBurnMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DPaperBurnShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// TileMapMaterial
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"TileMapMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"TileMapShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"MTileMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"MTileShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// PostProcessMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"PostProcessMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"PostProcessShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	// DistortionMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"DistortionMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DistortionShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	// VortexMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"VortexMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"VortexShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	// EnlargeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"EnlargeMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"EnlargeShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);

	// WaveMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"WaveMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"WaveShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	// DebugShapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"DebugShapeMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DebugShapeShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	// ParticleMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"ParticleMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"ParticleShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	// DamageSkin
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"DamageSkinMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DamageSkinShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);


	// Fade
	pMtrl = new CMaterial(true);
	pMtrl->SetName(L"FadeMtrl");
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"FadeShader"));
	AddAsset<CMaterial>(pMtrl->GetName(), pMtrl);
}

void CAssetMgr::CreateEngineSprite()
{
	// 링크 애니메이션 생성
	Ptr<CTexture> pLinkTex = Load<CTexture>(L"LinkAtlas", L"Texture\\link.png");
	CreateFlipbook(L"LinkWalkDown", pLinkTex, Vector2(10, 1), Vector2(120.f, 130.f), Vector2(0.f, 520.f));

	// 메이플 이미지 로드
	Ptr<CTexture> pStand = Load<CTexture>(L"StandAtlas", L"Texture//maple//avatar_stand1_default.png");
	Ptr<CTexture> pWalk = Load<CTexture>(L"WalkAtlas", L"Texture//maple//avatar_walk1_default.png");
	Ptr<CTexture> pProne = Load<CTexture>(L"ProneAtlas", L"Texture//maple//avatar_prone(0)_default(0).png");
	Ptr<CTexture> pJump = Load<CTexture>(L"JumpAtlas", L"Texture//maple//avatar_jump(0)_default(0).png");
	Ptr<CTexture> pRope = Load<CTexture>(L"RopeAtlas", L"Texture//maple//avartar//rope.png");
	Ptr<CTexture> pAttack = Load<CTexture>(L"AttackAtlas", L"Texture//maple//avartar//swingO2.png");
	Ptr<CTexture> pSkill = Load<CTexture>(L"SkillAtlas", L"Texture//maple//avartar//swingO2.png");

	// 메이플 애니메이션 생성
	CreateFlipbook(L"Stand", pStand, Vector2(3, 1), Vector2(54.f, 72.f));
	CreateFlipbook(L"Walk", pWalk, Vector2(3, 1), Vector2(54.f, 72.f));
	CreateFlipbook(L"Jump", pJump, Vector2(1, 1), Vector2(54.f, 70.f));
	CreateFlipbook(L"Prone", pProne, Vector2(1, 1), Vector2(78.f, 47.f));
	CreateFlipbook(L"Rope", pRope, Vector2(1, 1), Vector2(51.f, 85.f));
	CreateFlipbook(L"Attack", pAttack, Vector2(3, 1), Vector2(79.f, 100.f));

	// 루시드 애니메이션 생성
	Ptr<CTexture> pLucid = Load<CTexture>(L"LucidAtlas", L"Texture//maple//RushLucidAtlas.png");
	CreateFlipbook(L"Lucid", pLucid, Vector2(8, 1), Vector2(414.f, 464.f));

	// tino 애니메이션 생성
	Ptr<CTexture> pTexture = Load<CTexture>(L"tino_stand", L"Texture//monster//tino_stand.png");
	CreateFlipbook(L"tino_stand", pTexture, Vector2(2, 1), Vector2(33.f, 34.f));

	pTexture = Load<CTexture>(L"tino_hit", L"Texture//monster//tino_hit.png");
	CreateFlipbook(L"tino_hit", pTexture, Vector2(1, 1), Vector2(44.f, 36.f));

	pTexture = Load<CTexture>(L"tino_move", L"Texture//monster//tino_move.png");
	CreateFlipbook(L"tino_move", pTexture, Vector2(6, 1), Vector2(35.f, 35.f));

	pTexture = Load<CTexture>(L"tino_die", L"Texture//monster//tino_die.png");
	CreateFlipbook(L"tino_die", pTexture, Vector2(12, 1), Vector2(60.f, 36.f));
}

void CAssetMgr::CreateEnginePrefab()
{
	Ptr<CPrefab> pPrefab = new CPrefab(true);
	AddAsset<CPrefab>(L"MObject", pPrefab);
	
	CGameObject* pObj = new CGameObject;
	pObj->AddComponent(new CMeshRender);
	pObj->MeshRender()->SetMaterial(FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	pObj->SetName(L"MObject");
	pPrefab->SetProtoObject(pObj);
}

void CAssetMgr::SaveAllPrefab()
{
	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)ASSET_TYPE::PREFAB].begin();

	for (; iter != m_mapAsset[(UINT)ASSET_TYPE::PREFAB].end(); ++iter)
	{
		iter->second->Save(CPathMgr::GetInst()->GetContentPath() + iter->second->GetRelativePath());
	}
}
