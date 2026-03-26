#include "pch.h"
#include "MTile.h"
#include "CAssetMgr.h"

MTile::MTile(bool _EngineRes)
	: CAsset(ASSET_TYPE::MTILE, _EngineRes)
	, m_FirstTex(nullptr)
	, m_TileTexArray(nullptr)
{
}

MTile::~MTile()
{
}


void MTile::LoadTileTexture(const wstring& _TileName)
{
	SetName(_TileName);
	SetKey(_TileName);
	SetRelativePath(L"Tile\\Maple\\" + GetName() + L".tile");

	wstring path = L"Texture\\tile\\";
	wstring name[11] = {
	_TileName + L".img.edU.0",
	_TileName + L".img.edD.0",
	_TileName + L".img.enH0.0",
	_TileName + L".img.enH1.0",
	_TileName + L".img.enV0.0",
	_TileName + L".img.enV1.0",
	_TileName + L".img.bsc.0",
	_TileName + L".img.slLU.0",
	_TileName + L".img.slRU.0",
	_TileName + L".img.slLD.0",
	_TileName + L".img.slRD.0"
	};

	vector<Ptr<CTexture>> vecTexArray;
	vecTexArray.resize(11);

	for (int i = 0; i < 11; ++i)
	{
		wstring relPath = path + name[i] + L".png";
		Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(relPath, relPath);



		if (nullptr == pTex)
			vecTexArray[i] = CAssetMgr::GetInst()->FindAsset<CTexture>(path + L"fallenLeaves.img.bsc.0.png");
		else
			vecTexArray[i] = pTex;

		if (i == 0)
			m_FirstTex = pTex;

		m_Tex[i] = pTex;
	}

	m_TileTexArray = new CTexture;
	m_TileTexArray->CreateArrayTexture(vecTexArray, m_TileSize);
}


int MTile::Save(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");
	assert(pFile);

	SaveWString(m_TileName, pFile);

	fclose(pFile);

	return S_OK;
}


int MTile::Load(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");
	assert(pFile);

	LoadWString(m_TileName, pFile);
	LoadTileTexture(m_TileName);

	fclose(pFile);

	return S_OK;
}
