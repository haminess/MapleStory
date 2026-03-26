#pragma once
#include "CRenderComponent.h"

struct tMTileData
{
	int     TexIdx;
	int		Padding[3];
	Vector2 TilePos;
	Vector2 TileScale;
};



class MTile;
class CStructuredBuffer;
class MTileMap :
	public CRenderComponent
{
private:
	// 타일 에셋
	Ptr<MTile> m_TileAsset;

	// 타일 렌더링 데이터
	vector<tMTileData>  m_vecTileData;

	CStructuredBuffer* m_Buffer;			// 타일 데이터를 GPU 로 전달할 때 사용할 버퍼
	CStructuredBuffer* m_TileSizeBuffer;    // 타일 에셋의 텍스쳐 크기를 전달할 때 사용할 버퍼

public:
	Ptr<MTile> GetTileAsset() { return m_TileAsset; }
	const vector<tMTileData>&  GetTileData() { return m_vecTileData; }


public:
	void RenewData(Ptr<MTile> _Asset, const vector<tMTileData>& _Data)
	{
		SetTileAsset(_Asset);
		SetTileData(_Data);
	}

	void RenewData()
	{
		SetTileAsset(m_TileAsset);
		SetTileData(m_vecTileData);
	}

public:
	void SetTileAsset(Ptr<MTile> _Asset);
	void SetTileData(const vector<tMTileData>& _Data);

private:
	vector<tMTileData> CreateSampleHorizontalLine();

public:
	virtual void FinalTick() override {};
	virtual void Render() override;

	virtual void SaveComponent(FILE* _File) override;
	virtual void LoadComponent(FILE* _FILE) override;


public:
	CLONE(MTileMap);
	MTileMap();
	MTileMap(const MTileMap& _Origin);
	~MTileMap();
};

