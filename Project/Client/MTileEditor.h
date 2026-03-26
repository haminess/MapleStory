#pragma once
//#include "EditorUI.h"
#include "PresetList.h"
#include <Engine/CTileMap.h>
#include <Engine/MTile.h>
#include <Engine/MTileMap.h>

enum class TILE_EDIT
{
	DRAW,
	BOX,
	ERASE,
	END
};

struct tTileDummy
{
	CGameObject* pObj;
	int RenderTile[2];

	tTileDummy()
		: pObj(nullptr)
		, RenderTile {-1, -1}
	{ }
	~tTileDummy()
	{ }
	
	void SetTileTexture(TILE_TEX _TexType)
	{
		RenderTile[0] = (UINT)_TexType;
		RenderTile[1] = -1;
	}
	void SetTileTexture(TILE_TEX _Tex1, TILE_TEX _Tex2)
	{
		RenderTile[0] = (UINT)_Tex1;
		RenderTile[1] = (UINT)_Tex2;
	}

	void Render(bool _Show)
	{
		pObj->SetActive(_Show);
	}
};


class CTexture;
class MTileEditor :
	public PresetList
{
private:
	Ptr<MTile> m_TileAsset;

	// 레벨의 타일 오브젝트 정보를 받아와 데이터 수정
	CTileMap* m_TileMap;
	vector<bool> m_vecTileData;
	vector<tTileDummy> m_vecTileObj;
	vector<tMTileData> m_vecRenderData;
	CGameObject* m_EditObject;

	Vector2 m_StartPos;             // 시작 위치
	Vector2 m_TileSize;             // 타일 크기
	Vector2 m_TileCount;            // 그리드 개수

	TILE_EDIT m_Type;

	bool m_bEdit;
	int m_CurTileX;
	int m_CurTileY;


public:
	virtual void SelectPreset(int _Idx) override;
	

public:
	// ======
	// 마우스
	void MouseCheck();
	
	// ==========
	// 데이터 출력
	void DrawTile();
	void EraseTile();



	// ==========
	// 데이터 편집
	// x, y에 데이터 칠하기
	void AddTile(int _Col, int _Row);

	// x, y에 데이터 지우기
	void EraseTile(int _Col, int _Row);

	void UpdateTileObjectData(int _Col, int _Row);
	void UpdateTileObjectData(int _Idx);
	void UpdateTileTexture(int _Col, int _Row);
	void UpdateTileTexture(int _Idx);
	void CreateTileObject();
	void RenewTileObject();
	void ExportTileObject();



private:
	bool IsValid(int _Col, int _Row)
	{
		int idx = GetTileIndex(_Col, _Row);
		return (0 <= idx && idx < m_TileCount.x * m_TileCount.y);
	}
	bool IsExist(int _Col, int _Row)
	{
		return m_vecTileData[GetTileIndex(_Col, _Row)];
	}
	bool IsEven(int _Num) { return _Num % 2 == 0 ? true : false; }
	int IsSlope(int _Col, int _Row)
	{
		bool isTopLine = IsValid(_Col, _Row - 1) ? IsExist(_Col, _Row - 1) : false;
		bool isBottomLine = IsValid(_Col, _Row + 1) ? IsExist(_Col, _Row + 1) : false;
		bool isLeftLine = IsValid(_Col - 1, _Row) ? IsExist(_Col - 1, _Row) : false;
		bool isRightLine = IsValid(_Col + 1, _Row) ? IsExist(_Col + 1, _Row) : false;

		if (isBottomLine && !isTopLine)
		{
			if (isRightLine && !isLeftLine) return 0;		// 왼쪽위가 경사면
			else if (isLeftLine && !isRightLine) return 1;	// 오른쪽위가 경사면
		}
		else if (isTopLine && !isBottomLine)
		{
			if (isRightLine && !isLeftLine) return 2;		// 왼쪽아래가 경사면
			else if (isLeftLine && !isRightLine) return 3;	// 오른쪽아래가 경사면
		}

		// 대각선이 아님
		return -1;
	}

	int GetTileIndex(int _Col, int _Row)
	{
		return m_TileCount.x * _Row + _Col;
	}

	Vector2 GetTileCount(int _Idx)
	{
		return Vector2(_Idx % (int)m_TileCount.x, _Idx / (int)m_TileCount.x);
	}

	Vector2 GetTilePos(int _Col, int _Row)
	{
		return Vector2(m_StartPos.x + _Col * m_TileSize.x / 2.f, m_StartPos.y - _Row * m_TileSize.y / 2.f);
	}
	Vector2 GetTilePos(int _Idx)
	{
		Vector2 vCount = GetTileCount(_Idx);
		return Vector2(m_StartPos.x + (int)vCount.x * m_TileSize.x / 2.f, m_StartPos.y - (int)vCount.y * m_TileSize.y / 2.f);
	}
	Vector2 GetRenderPos(DIRECTION _Dir, Vector2 _Pos, Vector2 _Scale)
	{
		Vector2 vRenderPos = _Pos;
		switch (_Dir)
		{
		case DIRECTION::UP:
			vRenderPos += Vector2(0.f, _Scale.y) / 2.f;
			break;
		case DIRECTION::DOWN:
			vRenderPos += Vector2(0.f, -_Scale.y) / 2.f;
			break;
		case DIRECTION::LEFT:
			vRenderPos += Vector2(-_Scale.x, 0.f) / 2.f;
			break;
		case DIRECTION::RIGHT:
			vRenderPos += Vector2(_Scale.x, 0.f) / 2.f;
			break;
		case DIRECTION::CENTER:
			break;
		}

		return vRenderPos;
	}



public:
	void DrawGrid();
	void DrawTileData();

public:
	virtual void Render_Editor() override;

public:
	void Load();
	void Save();

public:
	MTileEditor();
	~MTileEditor();
};

