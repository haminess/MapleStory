#include "pch.h"
#include "CAssetMgr.h"
#include "CDevice.h"

CAssetMgr::CAssetMgr()
	: m_bAssetChanged(false)
{

}

CAssetMgr::~CAssetMgr()
{

}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _Key, UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat, UINT _BindFlag, D3D11_USAGE _Usage)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_Key);

	if (nullptr != pTex)
		return pTex;

	pTex = new CTexture;
	if (FAILED(pTex->Create(_Width, _Height, _PixelFormat, _BindFlag, _Usage)))
	{
		assert(nullptr);
		return nullptr;
	}

	pTex->SetKey(_Key);
	pTex->SetName(_Key);
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_Key, pTex.Get()));

	return pTex;
}

Ptr<CTexture> CAssetMgr::CreateTexture(const wstring& _Key, ComPtr<ID3D11Texture2D> _Tex2D)
{
	Ptr<CTexture> pTex = FindAsset<CTexture>(_Key);

	if (nullptr != pTex)
		return pTex;

	pTex = new CTexture;
	if (FAILED(pTex->Create(_Tex2D)))
	{
		assert(nullptr);
		return nullptr;
	}

	pTex->SetKey(_Key);
	pTex->SetName(_Key);
	m_mapAsset[(UINT)ASSET_TYPE::TEXTURE].insert(make_pair(_Key, pTex.Get()));

	return pTex;
}






Ptr<CFlipbook> CAssetMgr::CreateFlipbook(const wstring& _Key, Ptr<CTexture> _Atlas, Vector2 _Count, Vector2 _Size, Vector2 _LeftTop)
{
	Ptr<CFlipbook> pFlipbook = new CFlipbook;

	for (int i = 0; i < _Count.x; ++i)
	{
		for (int j = 0; j < _Count.y; ++j)
		{
			Ptr<CSprite> pSprite = new CSprite(true);
			pSprite->SetAtlasTexture(_Atlas);
			pSprite->SetLeftTop(Vector2(_LeftTop.x + i * _Size.x, _LeftTop.y + j * _Size.y));
			pSprite->SetSlice(Vector2(_Size.x, _Size.y));

			float maxSize = _Size.x > _Size.y ? _Size.x : _Size.y;
			pSprite->SetBackground(Vector2(maxSize, maxSize));

			wstring spriteKey = _Key + L"_" + std::to_wstring(i);

			// Sprite 등록
			AddAsset(spriteKey, pSprite);

			// Texture에 Sprite 등록
			_Atlas->AddSprite(pSprite.Get());

			// flipbook 에 Sprite 추가
			pFlipbook->AddSprite(pSprite);
		}
	}

	// flipbook 등록
	AddAsset(wstring(_Key + L"Anim"), pFlipbook);

	return pFlipbook;
}

void CAssetMgr::GetAssetNames(ASSET_TYPE _Type, vector<wstring>& _vecAssetNames)
{
	for (const auto& pair : m_mapAsset[(UINT)_Type])
	{
		_vecAssetNames.push_back(pair.first);
	}
}

void CAssetMgr::DeleteAsset(ASSET_TYPE _Type, const wstring& _Key)
{
	map<wstring, Ptr<CAsset>>& mapAsset = m_mapAsset[(UINT)_Type];

	map<wstring, Ptr<CAsset>>::iterator iter = mapAsset.find(_Key);

	if (mapAsset.end() == iter)
		return;

	mapAsset.erase(iter);

	m_bAssetChanged = true;
}

