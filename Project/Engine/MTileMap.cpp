#include "pch.h"
#include "MTileMap.h"
#include "CStructuredBuffer.h"
#include "CAssetMgr.h"

MTileMap::MTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_Buffer(nullptr)
	, m_TileSizeBuffer(nullptr)
{
	// Mesh / Material 
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"MTileMtrl"));

	// 에셋 설정
	SetTileAsset(CAssetMgr::GetInst()->FindAsset<MTile>(L"fallenLeaves"));

	// 타일 데이터 설정
	vector<tMTileData> vecData = CreateSampleHorizontalLine();
	SetTileData(vecData);
}

MTileMap::MTileMap(const MTileMap& _Origin)
	: CRenderComponent(_Origin)
	, m_Buffer(nullptr)
	, m_TileSizeBuffer(nullptr)
	, m_vecTileData(_Origin.m_vecTileData)
	, m_TileAsset(_Origin.m_TileAsset)
{
	RenewData();
}

MTileMap::~MTileMap()
{
	delete m_Buffer;
	delete m_TileSizeBuffer;
}


vector<tMTileData> MTileMap::CreateSampleHorizontalLine()
{
	vector<tMTileData> vecTileData;
	vecTileData.resize(6);

	vector<tTexArraySize>& vSize = m_TileAsset->GetTexSize();
	float offset = 10.f;

	Vector2 vTileSize = vSize[(int)TILE_TEX::SURFACE].TexResolution;

	// 왼쪽 점 - 위쪽 텍스처 
	Vector2 vTexSize = vSize[(int)TILE_TEX::DOT_UP].TexResolution;
	vecTileData[0].TexIdx = (int)TILE_TEX::DOT_UP;
	vecTileData[0].TilePos = Vector2(-vTileSize.x / 2.f, vTexSize.y / 2.f);
	vecTileData[0].TileScale = vSize[(int)TILE_TEX::DOT_UP].TexResolution;

	// 왼쪽 점 - 아래쪽 텍스처
	vTexSize = vSize[(int)TILE_TEX::DOT_DOWN].TexResolution;
	vecTileData[1].TexIdx = (int)TILE_TEX::DOT_DOWN;
	vecTileData[1].TilePos = Vector2(-vTileSize.x / 2.f, -vTexSize.y / 2.f);
	vecTileData[1].TileScale = vSize[(int)TILE_TEX::DOT_DOWN].TexResolution;

	// 가운데 선 - 위쪽 텍스처
	vTexSize = vSize[(int)TILE_TEX::LINE_UP].TexResolution;
	vecTileData[2].TexIdx = (int)TILE_TEX::LINE_UP;
	vecTileData[2].TilePos = Vector2(0.f, vTexSize.y / 2.f);
	vecTileData[2].TileScale = vSize[(int)TILE_TEX::LINE_UP].TexResolution;

	// 가운데 선 - 아래쪽 텍스처
	vTexSize = vSize[(int)TILE_TEX::LINE_DOWN].TexResolution;
	vecTileData[3].TexIdx = (int)TILE_TEX::LINE_DOWN;
	vecTileData[3].TilePos = Vector2(0.f, -vTexSize.y / 2.f);
	vecTileData[3].TileScale = vSize[(int)TILE_TEX::LINE_DOWN].TexResolution;

	// 오른쪽 점 - 위쪽 텍스처
	vTexSize = vSize[(int)TILE_TEX::DOT_UP].TexResolution;
	vecTileData[4].TexIdx = (int)TILE_TEX::DOT_UP;
	vecTileData[4].TilePos = Vector2(vTileSize.x / 2.f, vTexSize.y / 2.f);
	vecTileData[4].TileScale = vSize[(int)TILE_TEX::DOT_UP].TexResolution;

	// 오른쪽 점 - 아래쪽 텍스처
	vTexSize = vSize[(int)TILE_TEX::DOT_DOWN].TexResolution;
	vecTileData[5].TexIdx = (int)TILE_TEX::DOT_DOWN;
	vecTileData[5].TilePos = Vector2(vTileSize.x / 2.f, -vTexSize.y / 2.f);
	vecTileData[5].TileScale = vSize[(int)TILE_TEX::DOT_DOWN].TexResolution;

	return vecTileData;
}

void MTileMap::SaveComponent(FILE* _File)
{
	SaveAssetRef(m_TileAsset, _File);

	int count = m_vecTileData.size();
	fwrite(&count, sizeof(int), 1, _File);
	
	for (int i = 0; i < count; ++i)
	{
		fwrite(&m_vecTileData[i].TexIdx, sizeof(int), 1, _File);
		fwrite(&m_vecTileData[i].TilePos, sizeof(Vector2), 1, _File);
		fwrite(&m_vecTileData[i].TileScale, sizeof(Vector2), 1, _File);
		fwrite(&m_vecTileData[i].Padding, sizeof(int), 3, _File);
	}
}

void MTileMap::LoadComponent(FILE* _File)
{
	LoadAssetRef(m_TileAsset, _File);

	//int count = 0;
	//fread(&count, sizeof(int), 1, _File);
	//fread(&m_vecTileData, sizeof(tMTileData), count, _File);

	int count = 0;
	fread(&count, sizeof(int), 1, _File);

	m_vecTileData.resize(count);
	for (int i = 0; i < count; ++i)
	{
		fread(&m_vecTileData[i].TexIdx, sizeof(int), 1, _File);
		fread(&m_vecTileData[i].TilePos, sizeof(Vector2), 1, _File);
		fread(&m_vecTileData[i].TileScale, sizeof(Vector2), 1, _File);
		fread(&m_vecTileData[i].Padding, sizeof(int), 3, _File);
	}

	SetTileData(m_vecTileData);

}


void MTileMap::SetTileAsset(Ptr<MTile> _Asset)
{
	m_TileAsset = _Asset;

	vector<tTexArraySize> vecSize;
	vector<tTexArraySize>& vCopy = m_TileAsset->GetTexSize();

	vecSize.resize(vCopy.size());
	for (int i = 0; i < vCopy.size(); ++i)
	{
		vecSize[i].TexSizeUV = vCopy[i].TexSizeUV;
		vecSize[i].TexResolution = vCopy[i].TexResolution;
	}

	// 데이터 버퍼에 세팅
	if (m_TileSizeBuffer != nullptr)
	{
		delete m_TileSizeBuffer;
		m_TileSizeBuffer = nullptr;
	}

	m_TileSizeBuffer = new CStructuredBuffer;
	m_TileSizeBuffer->Create(sizeof(tTexArraySize), vecSize.size(), SB_TYPE::SRV_ONLY, true);
	m_TileSizeBuffer->SetData(vecSize.data(), vecSize.size());
}

void MTileMap::SetTileData(const vector<tMTileData>& _Data)
{
	m_vecTileData.resize(_Data.size());
	for (int i = 0; i < _Data.size(); ++i)
	{
		m_vecTileData[i] = _Data[i];
	}

	// 데이터 버퍼에 세팅
	if (m_Buffer != nullptr)
	{
		delete m_Buffer;
		m_Buffer = nullptr;
	}

	m_Buffer = new CStructuredBuffer;

	if (FAILED(m_Buffer->Create(sizeof(tMTileData), m_vecTileData.size(), SB_TYPE::SRV_ONLY, true))) {
		assert(nullptr);
	}
	m_Buffer->SetData(m_vecTileData.data(), m_vecTileData.size());
}

void MTileMap::Render()
{
	if (m_Buffer == nullptr || m_TileSizeBuffer == nullptr)
		return;

	// 위치정보 바인딩
	Transform()->Binding();

	// 타일 버퍼 바인딩
	m_Buffer->Binding(16);
	m_TileSizeBuffer->Binding(17);

	// 재질정보 바인딩
	GetMaterial()->SetTexParam(TEX_ARR_0, m_TileAsset->GetTexArray());
	GetMaterial()->Binding();

	// 렌더링
	GetMesh()->Render_Particle(m_vecTileData.size());

	// 타일 버퍼 바인딩 해제
	m_Buffer->Clear(16);
	m_TileSizeBuffer->Clear(17);
}
