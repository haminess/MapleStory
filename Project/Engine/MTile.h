#pragma once
#include "CAsset.h"


enum class TILE_TEX
{
	DOT_UP,
	DOT_DOWN,
	LINE_UP,
	LINE_DOWN,
	LINE_LEFT,
	LINE_RIGHT,
	SURFACE,
	SURFACE_TOP_LEFT,
	SURFACE_TOP_RIGHT,
	SURFACE_BOTTOM_LEFT,
	SURFACE_BOTTOM_RIGHT,
	END
};


class CTexture;
class MTile :
	public CAsset
{
private:
	wstring			m_TileName;
	Ptr<CTexture>	m_TileTexArray;
	Ptr<CTexture>	m_FirstTex;
	vector<tTexArraySize> m_TileSize;

	Ptr<CTexture>	m_Tex[(UINT)TILE_TEX::END];
	// 타일 타입에 따라 여러 텍스쳐가 랜덤으로 선택될 수 있어야 한다.

public:
	wstring			GetTileName() { return m_TileName; }
	void SetTileName(wstring _Set) { m_TileName = _Set; }
	Ptr<CTexture>	GetTexArray() { return m_TileTexArray; }
	Ptr<CTexture>	GetFirstTex() { return m_FirstTex; }
	Ptr<CTexture>	GetTileTex(TILE_TEX _Tex) { return m_Tex[(int)_Tex]; }
	void SetTileTex(Ptr<CTexture> _Set) { m_TileTexArray = _Set; }
	vector<tTexArraySize>& GetTexSize() { return m_TileSize; }


public:
	void LoadTileTexture(const wstring& _TileName);

private:
	virtual int Load(const wstring& _strFilePath);
	virtual int Save(const wstring& _strFilePath);

public:
	CLONE_DISABLE(MTile);
	MTile(bool _EngineRes = false);
	~MTile();
};

