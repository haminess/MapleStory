#pragma once

#include "CSprite.h"
#include "CTexture.h"
#include "CAssetMgr.h"

// CSprite에 대한 특수화 버전
template<>
void LoadAssetRef<CSprite>(Ptr<CSprite>& _Asset, FILE* _File)
{
	bool bAsset;
	fread(&bAsset, sizeof(bool), 1, _File);
	if (bAsset)
	{
		wstring Name, Key, Path;
		LoadWString(Name, _File);
		LoadWString(Key, _File);
		LoadWString(Path, _File);

		// CSprite일 경우 CTexture로 로드
		Ptr<CTexture> pTex = CAssetMgr::GetInst()->Load<CTexture>(Path, Path);

		Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(Key);
		if (pSprite == nullptr)
			_Asset = CAssetMgr::GetInst()->Load<CSprite>(Key, Key);
		else
			_Asset = pSprite;

		_Asset->SetName(Name);
	}
}