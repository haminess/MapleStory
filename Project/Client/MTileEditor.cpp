#include "pch.h"
#include "MTileEditor.h"
#include <Engine/CEngine.h>
#include<Engine/MTile.h>
#include<Engine/CRenderMgr.h>
#include<Engine/CCamera.h>
#include<Engine/CTransform.h>
#include<Engine/CKeyMgr.h>
#include<Engine/CFontMgr.h>
#include<Engine/CLevelMgr.h>
#include<Engine/components.h>

MTileEditor::MTileEditor()
	: PresetList("Tile")
	, m_StartPos(-100.f, 100.f)
	, m_TileSize(200.f, 100.f)
	, m_TileCount(9, 9)
	, m_bEdit(true)
	, m_Type(TILE_EDIT::DRAW)
{
	Ptr<MTile> pTile = nullptr;

	pTile = CAssetMgr::GetInst()->FindAsset<MTile>(L"fallenLeaves");
	wstring wname = CPathMgr::GetInst()->GetNameFromPath(pTile->GetName(), false);
	AddPreset(string(wname.begin(), wname.end()), pTile.Get(), pTile->GetFirstTex());

	pTile = CAssetMgr::GetInst()->FindAsset<MTile>(L"yellowToyCastle");
	wname = CPathMgr::GetInst()->GetNameFromPath(pTile->GetName(), false);
	AddPreset(string(wname.begin(), wname.end()), pTile.Get(), pTile->GetFirstTex());

	pTile = CAssetMgr::GetInst()->FindAsset<MTile>(L"arcana1");
	wname = CPathMgr::GetInst()->GetNameFromPath(pTile->GetName(), false);
	AddPreset(string(wname.begin(), wname.end()), pTile.Get(), pTile->GetFirstTex());

	pTile = CAssetMgr::GetInst()->FindAsset<MTile>(L"grassySoil");
	wname = CPathMgr::GetInst()->GetNameFromPath(pTile->GetName(), false);
	AddPreset(string(wname.begin(), wname.end()), pTile.Get(), pTile->GetFirstTex());

	pTile = CAssetMgr::GetInst()->FindAsset<MTile>(L"woodBridge");
	wname = CPathMgr::GetInst()->GetNameFromPath(pTile->GetName(), false);
	AddPreset(string(wname.begin(), wname.end()), pTile.Get(), pTile->GetFirstTex());

	pTile = CAssetMgr::GetInst()->FindAsset<MTile>(L"darkWood");
	wname = CPathMgr::GetInst()->GetNameFromPath(pTile->GetName(), false);
	AddPreset(string(wname.begin(), wname.end()), pTile.Get(), pTile->GetFirstTex());


	SelectPreset(0);

	m_TileSize = m_TileAsset->GetTileTex(TILE_TEX::SURFACE)->GetResolution();

	m_vecTileData.resize(m_TileCount.x * m_TileCount.y, false);
	m_vecTileObj.resize(m_TileCount.x * m_TileCount.y);

	//CreateTileObject();
}

MTileEditor::~MTileEditor()
{
}


void MTileEditor::Render_Editor()
{
	// 타일 설정
	ImGui::Text("Start Position");
	ImGui::DragFloat2("##StartPos", (float*)&m_StartPos);

	ImGui::Text("Tile Size");
	ImGui::DragFloat2("##TileSize", (float*)&m_TileSize);

	ImGui::Text("Tile Count");
	int count[2] = { (int)m_TileCount.x, (int)m_TileCount.y };
	if (ImGui::DragInt2("##TileCount", (int*)count))
	{
		m_TileCount.x = count[0];
		m_TileCount.y = count[1];
	}

	if(ImGui::Button("Create"))
	{
		CreateTileObject();
	}

	ImGui::SameLine();
	if(ImGui::Button("Apply"))
	{
		RenewTileObject();
	}
	ImGui::SameLine();
	if(ImGui::Button("Export"))
	{
		ExportTileObject();
	}
	ImGui::SameLine();
	if(ImGui::Button("Clear"))
	{
		m_vecTileData.clear();
		RenewTileObject();
	}
	ImGui::Separator();


	// ============
	// edit section
	if (CRenderMgr::GetInst()->IsGrid())
		CRenderMgr::GetInst()->ShowGrid(false);

	DrawTileData();
	DrawGrid();
	MouseCheck();

	if (m_bEdit)
	{
		switch (m_Type)
		{
		case TILE_EDIT::DRAW:
			DrawTile();
			break;
		case TILE_EDIT::BOX:
			//DrawBoxTile();
			break;
		case TILE_EDIT::ERASE:
			EraseTile();
			break;
		}


	}

}


void MTileEditor::SelectPreset(int _Idx)
{
	if (_Idx <0 || _Idx > GetPresetCount() - 1)
		return;

	m_TileAsset = GetAsset<MTile>(_Idx);
}

void MTileEditor::MouseCheck()
{
	// 카메라 관련 값들을 계산
	CCamera* pMainCam = CRenderMgr::GetInst()->GetRenderCamera();
	if (pMainCam == nullptr)
		return;

	float scale = pMainCam->GetScale();
	float width = pMainCam->GetWidth() * scale;
	float height = width / pMainCam->GetAspectRatio();
	Vector3 vCamPos = pMainCam->Transform()->GetWorldPos();

	// 카메라의 좌상단과 우하단 좌표 계산
	Vector2 vCamLT = Vector2(vCamPos.x - width / 2.f, vCamPos.y + height / 2.f);
	Vector2 vCamRB = Vector2(vCamPos.x + width / 2.f, vCamPos.y - height / 2.f);

	// 마우스의 월드 좌표와 스크린 좌표를 가져옴
	Vector2 vMousePos = CKeyMgr::GetInst()->GetMouseWorldPos();
	Vector2 vMouseScreenPos = CKeyMgr::GetInst()->GetMousePos();

	// 시작 위치로부터의 상대적인 마우스 위치 계산
	Vector2 vRelativePos = Vector2(
		vMousePos.x - m_StartPos.x,
		-(vMousePos.y - m_StartPos.y)  // y축 반전
	);

	// 서브타일의 크기 계산 (한 타일을 3x3으로 나눔)
	float subTileWidth = m_TileSize.x / 2.f;   // 서브타일의 가로 크기
	float subTileHeight = m_TileSize.y / 2.f;  // 서브타일의 세로 크기

	// 현재 마우스가 위치한 타일의 인덱스 계산
	m_CurTileX = round(vRelativePos.x / subTileWidth);
	m_CurTileY = round(vRelativePos.y / subTileHeight);


	// 마우스가 유효한 그리드 영역 내에 있는지 확인
	if (m_CurTileX >= 0 && m_CurTileX < m_TileCount.x &&
		m_CurTileY >= 0 && m_CurTileY < m_TileCount.y)
	{
		m_bEdit = true;

		// 호버 위치의 실제 좌표 계산
		Vector2 hoverPos = GetTilePos(m_CurTileX, m_CurTileY);

		// 호버 표시용 노란색 원 그리기
		DrawDebugRect(Vector4(1.f, 1.f, 0.f, 0.5f),
			Vector3(hoverPos.x, hoverPos.y, 0.f),
			Vector2(10.f, 10.f),
			Vector3(0.f, 0.f, 0.f),
			false,
			0.f,
			true);

		// 호버 정보 텍스트 출력
		// tile_world_pos : hoverPos
		// tile_count : subPosX, subPosY
		bool isEvenX = m_CurTileX % 2 == 0 ? true : false;
		bool isEvenY = m_CurTileY % 2 == 0 ? true : false;
		wstring tileCount;
		tileCount.reserve(255);


		if (isEvenX && isEvenY)
			swprintf_s(tileCount.data(), tileCount.capacity(), L"%d, %d (면)", m_CurTileX, m_CurTileY);
		else if (isEvenX || isEvenY)
			swprintf_s(tileCount.data(), tileCount.capacity(), L"%d, %d (선)", m_CurTileX, m_CurTileY);
		else
			swprintf_s(tileCount.data(), tileCount.capacity(), L"%d, %d (점)", m_CurTileX, m_CurTileY);

		Vector2 vMouseScreenPos = CKeyMgr::GetInst()->GetMousePos();
		CFontMgr::GetInst()->DrawFont(tileCount.c_str(), vMouseScreenPos.x, vMouseScreenPos.y, 16, FONT_RGBA(0, 0, 0, 255));
	}
	else
	{
		m_bEdit = false;
	}
}

void MTileEditor::DrawTile()
{
	if (KEY_PRESSED(KEY::LBTN))
	{
		if (!IsExist(m_CurTileX, m_CurTileY))
		{
			AddTile(m_CurTileX, m_CurTileY);
		}
	}
	if (KEY_PRESSED(KEY::RBTN))
	{
		if (IsExist(m_CurTileX, m_CurTileY))
		{
			EraseTile(m_CurTileX, m_CurTileY);
		}
	}
}


void MTileEditor::EraseTile()
{
	if (KEY_PRESSED(KEY::LBTN))
	{
		if (IsExist(m_CurTileX, m_CurTileY))
		{
			EraseTile(m_CurTileX, m_CurTileY);
		}
	}
}

void MTileEditor::UpdateTileObjectData(int _Col, int _Row)
{
	UpdateTileObjectData(GetTileIndex(_Col, _Row));
}

void MTileEditor::UpdateTileObjectData(int _Idx)
{
	Vector2 colrow = GetTileCount(_Idx);
	bool isEvenX = IsEven(colrow.x);
	bool isEvenY = IsEven(colrow.y);


	// 데이터 있는 타일 활성화
	if (!m_vecTileData[_Idx])
		m_vecTileObj[_Idx].pObj->SetActive(false);
	else
		m_vecTileObj[_Idx].pObj->SetActive(true);

	// 타일에 텍스쳐 
	CGameObject* pTile1 = m_vecTileObj[_Idx].pObj->GetChild()[0];
	CGameObject* pTile2 = m_vecTileObj[_Idx].pObj->GetChild()[1];

	Vector2 vPos = GetTilePos(_Idx);
	Vector2 vPos1 = vPos;
	Vector2 vPos2 = vPos;
	float posZ = 0.f;

	int tex_idx[2] = { m_vecTileObj[_Idx].RenderTile[0], m_vecTileObj[_Idx].RenderTile[1] };
	Vector2 vScale1 = {};
	Vector2 vScale2 = {};

	tTileDummy& Tile = m_vecTileObj[_Idx];
	Ptr<CTexture> pRenderTex = nullptr;

	// 면 출력
	if (isEvenX && isEvenY)
	{ 
		int type = IsSlope(colrow.x, colrow.y);

		float offset = 0.f;

		switch (type)
		{
		case -1:
			Tile.SetTileTexture(TILE_TEX::SURFACE);
			break;
		case 0:
			Tile.SetTileTexture(TILE_TEX::SURFACE_TOP_LEFT);

			pRenderTex = m_TileAsset->GetTileTex(TILE_TEX::SURFACE_TOP_LEFT);
			offset = (pRenderTex->GetResolution().y - m_TileSize.y) / 2.f;
			vPos1.y += offset;
			break;
		case 1:
			Tile.SetTileTexture(TILE_TEX::SURFACE_TOP_RIGHT);

			pRenderTex = m_TileAsset->GetTileTex(TILE_TEX::SURFACE_TOP_RIGHT);
			offset = (pRenderTex->GetResolution().y - m_TileSize.y) / 2.f;
			vPos1.y += offset;
			break;
		case 2:
			Tile.SetTileTexture(TILE_TEX::SURFACE_BOTTOM_LEFT);

			pRenderTex = m_TileAsset->GetTileTex(TILE_TEX::SURFACE_BOTTOM_LEFT);
			offset = (pRenderTex->GetResolution().y - m_TileSize.y) / 2.f;
			vPos1.y -= offset;
			break;
		case 3:
			Tile.SetTileTexture(TILE_TEX::SURFACE_BOTTOM_RIGHT);

			pRenderTex = m_TileAsset->GetTileTex(TILE_TEX::SURFACE_BOTTOM_RIGHT);
			offset = (pRenderTex->GetResolution().y - m_TileSize.y) / 2.f;
			vPos1.y -= offset;
			break;
		}
		posZ = 0.f;

		if(nullptr != m_TileAsset->GetTileTex((TILE_TEX)Tile.RenderTile[0]))
			vScale1 = m_TileAsset->GetTileTex((TILE_TEX)Tile.RenderTile[0])->GetResolution();
	}

	// 가로선 출력
	else if (isEvenX)
	{
		m_vecTileObj[_Idx].SetTileTexture(TILE_TEX::LINE_UP, TILE_TEX::LINE_DOWN);

		// 상 데이터 없으면 렌더링
		if (0 <= colrow.y - 1 && m_vecTileData[_Idx - m_TileCount.x])
			pTile1->SetActive(false);
		else
			pTile1->SetActive(true);

		// 하 없으면 렌더링
		if (colrow.y + 1 < m_TileCount.y && m_vecTileData[_Idx + m_TileCount.x])
			pTile2->SetActive(false);
		else
			pTile2->SetActive(true);

		// 크기, 위치 조정
		vScale1 = m_TileAsset->GetTileTex((TILE_TEX)Tile.RenderTile[0])->GetResolution();
		vScale2 = m_TileAsset->GetTileTex((TILE_TEX)Tile.RenderTile[1])->GetResolution();
		vPos1 = GetRenderPos(DIRECTION::UP, vPos, vScale1);
		vPos2 = GetRenderPos(DIRECTION::DOWN, vPos, vScale2);
		posZ = 2.f;
	}
	// 세로선 출력
	else if (isEvenY)
	{
		m_vecTileObj[_Idx].SetTileTexture(TILE_TEX::LINE_LEFT, TILE_TEX::LINE_RIGHT);

		// 좌 데이터 없으면 렌더링
		if (0 <= colrow.x - 1 && m_vecTileData[_Idx - 1])
			pTile1->SetActive(false);
		else
			pTile1->SetActive(true);

		// 우 없으면 렌더링
		if (colrow.x + 1 < m_TileCount.x && m_vecTileData[_Idx + 1])
			pTile2->SetActive(false);
		else
			pTile2->SetActive(true);

		// 크기, 위치 조정
		if(nullptr != m_TileAsset->GetTileTex((TILE_TEX)Tile.RenderTile[0]))
			vScale1 = m_TileAsset->GetTileTex((TILE_TEX)Tile.RenderTile[0])->GetResolution();
		if (nullptr != m_TileAsset->GetTileTex((TILE_TEX)Tile.RenderTile[1]))
			vScale2 = m_TileAsset->GetTileTex((TILE_TEX)Tile.RenderTile[1])->GetResolution();
		vPos1 = GetRenderPos(DIRECTION::LEFT, vPos, vScale1);
		vPos2 = GetRenderPos(DIRECTION::RIGHT, vPos, vScale2);
		posZ = 1.f;
	}

	// 점 출력
	else
	{
		m_vecTileObj[_Idx].SetTileTexture(TILE_TEX::DOT_UP, TILE_TEX::DOT_DOWN);

		// 좌우 데이터 없으면 렌더링 x
		if (0 <= colrow.x - 1 && m_vecTileData[_Idx - 1] && colrow.x + 1 < m_TileCount.x && m_vecTileData[_Idx + 1])
		{
			m_vecTileObj[_Idx].pObj->SetActive(false);
			return;
		}

		// 상 데이터 없으면 렌더링
		if (0 <= colrow.y - 1 && m_vecTileData[_Idx - m_TileCount.x])
			pTile1->SetActive(false);
		else
			pTile1->SetActive(true);

		// 하 없으면 렌더링
		if (colrow.y + 1 < m_TileCount.y && m_vecTileData[_Idx + m_TileCount.x])
			pTile2->SetActive(false);
		else
			pTile2->SetActive(true);

		// 크기, 위치 조정
		vScale1 = m_TileAsset->GetTileTex((TILE_TEX)Tile.RenderTile[0])->GetResolution();
		vScale2 = m_TileAsset->GetTileTex((TILE_TEX)Tile.RenderTile[1])->GetResolution();
		vPos1 = GetRenderPos(DIRECTION::UP, vPos, vScale1);
		vPos2 = GetRenderPos(DIRECTION::DOWN, vPos, vScale2);
		posZ = 3.f;
	}

	// 사용하지 않는 자식 텍스쳐 오브젝트 비활성화
	if (pTile2->IsActive())
	{
		if (tex_idx[1] == -1)
			pTile2->SetActive(false);
		else
			pTile2->SetActive(true);
	}

	UpdateTileTexture(_Idx);

	pTile1->Transform()->SetRelativePos(Vector3(vPos1.x, vPos1.y, posZ));
	pTile1->Transform()->SetRelativeScale(Vector3(vScale1.x, vScale1.y, 1.f));

	pTile2->Transform()->SetRelativePos(Vector3(vPos2.x, vPos2.y, posZ));
	pTile2->Transform()->SetRelativeScale(Vector3(vScale2.x, vScale2.y, 1.f));

	if (m_vecTileObj[_Idx].pObj->IsActive())
	{
		if (pTile1->IsActive())
		{
			tMTileData tile = {};
			tile.TexIdx = Tile.RenderTile[0];
			tile.TilePos = pTile1->Transform()->GetWorldPos();
			tile.TileScale = pTile1->Transform()->GetWorldScale();
			m_vecRenderData.push_back(tile);
		}
		if (pTile2->IsActive())
		{
			tMTileData tile = {};
			tile.TexIdx = Tile.RenderTile[1];
			tile.TilePos = pTile2->Transform()->GetWorldPos();
			tile.TileScale = pTile2->Transform()->GetWorldScale();
			m_vecRenderData.push_back(tile);
		}
	}

}

void MTileEditor::UpdateTileTexture(int _Col, int _Row)
{
	UpdateTileTexture(GetTileIndex(_Col, _Row));
}

void MTileEditor::UpdateTileTexture(int _Idx)
{
	const tTileDummy& info = m_vecTileObj[_Idx];
	CGameObject* pTile1 = info.pObj->GetChild()[0];
	CGameObject* pTile2 = info.pObj->GetChild()[1];

	int tex_idx = 0;
	tex_idx = info.RenderTile[0];
	if (tex_idx != -1)
	{
		Ptr<CTexture> pTex = m_TileAsset->GetTileTex((TILE_TEX)tex_idx);
		pTile1->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	}

	tex_idx = info.RenderTile[1];
	if (tex_idx != -1)
	{
		Ptr<CTexture> pTex = m_TileAsset->GetTileTex((TILE_TEX)tex_idx);
		pTile2->MeshRender()->GetMaterial()->SetTexParam(TEX_PARAM::TEX_0, pTex);
	}
}

void MTileEditor::CreateTileObject()
{
	CGameObject* pObj = CLevelMgr::GetInst()->FindObjectByName(L"EditTile");

	if (pObj)
		DestroyObject(pObj);

	// 타일 렌더링할 오브젝트 생성
	m_EditObject = new CGameObject;
	m_EditObject->SetName(L"EditTile");

	
	
	(m_EditObject, (int)LAYER_INDEX::PLATFORM, true);

	m_vecTileData.resize(m_TileCount.x * m_TileCount.y, false);
	m_vecTileObj.resize(m_TileCount.x * m_TileCount.y);

	// 타일들을 렌더링할 오브젝트 생성
	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl");
	for (int i = 0; i < m_vecTileData.size(); ++i)
	{
		CGameObject* pTile = new CGameObject;
		pTile->SetActive(false);
		//pTile->SetLayerIdx((int)LAYER_INDEX::PLATFORM);
		AddChild(m_EditObject, pTile);

		wstring name = L"Tile_" + std::to_wstring(i);
		pTile->SetName(name);

		// 타일을 실제로 렌더링할 자식 오브젝트
		for (int j = 0; j < 2; ++j)
		{
			CGameObject* pModel = new CGameObject;
			//pTile->AddChild(pModel);
			AddChild(pTile, pModel);
			//pTile->SetLayerIdx((int)LAYER_INDEX::PLATFORM);

			wstring name = L"Tex_" + std::to_wstring(j);
			pModel->SetName(name);

			pModel->AddComponent(new CMeshRender);
			pModel->MeshRender()->SetMesh(pMesh);
			pModel->MeshRender()->SetMaterial(pMtrl);
			pModel->MeshRender()->CreateDynamicMaterial();
		}

		m_vecTileObj[i].pObj = pTile;
	}
}

void MTileEditor::RenewTileObject()
{
	m_vecRenderData.clear();

	for (int i = 0; i < m_vecTileData.size(); ++i)
	{
		UpdateTileObjectData(i);
	}
}

void MTileEditor::ExportTileObject()
{
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"TilePlatform");

	MTileMap* pTileMap = new MTileMap;
	pObject->AddComponent(pTileMap);
	pTileMap->SetTileAsset(m_TileAsset);
	pTileMap->SetTileData(m_vecRenderData);
	CreateObject(pObject, (int)LAYER_INDEX::PLATFORM, false);
}


void MTileEditor::AddTile(int _Col, int _Row)
{
	m_vecTileData[GetTileIndex(_Col, _Row)] = true;

	bool isEvenX = IsEven(_Col);
	bool isEvenY = IsEven(_Row);
	bool isLeftX = _Col == 0;
	bool isRightX = _Col == (int)m_TileCount.x - 1;
	bool isTopY = _Row == 0;
	bool isBottomY = (int)m_TileCount.y - 1;

	// 면
	if (isEvenX && isEvenY)
	{
		// 대각선이 아니면
		if (-1 == IsSlope(_Col, _Row))
		{
			if (IsValid(_Col - 1, _Row))        m_vecTileData[GetTileIndex(_Col - 1, _Row)] = true;
			if (IsValid(_Col - 1, _Row - 1))    m_vecTileData[GetTileIndex(_Col - 1, _Row - 1)] = true;
			if (IsValid(_Col - 1, _Row + 1))    m_vecTileData[GetTileIndex(_Col - 1, _Row + 1)] = true;
			if (IsValid(_Col, _Row - 1))        m_vecTileData[GetTileIndex(_Col, _Row - 1)] = true;
			if (IsValid(_Col, _Row + 1))        m_vecTileData[GetTileIndex(_Col, _Row + 1)] = true;
			if (IsValid(_Col + 1, _Row))        m_vecTileData[GetTileIndex(_Col + 1, _Row)] = true;
			if (IsValid(_Col + 1, _Row - 1))    m_vecTileData[GetTileIndex(_Col + 1, _Row - 1)] = true;
			if (IsValid(_Col + 1, _Row + 1))    m_vecTileData[GetTileIndex(_Col + 1, _Row + 1)] = true;
		}
	}

	// 가로선
	else if (isEvenX)
	{
		if (IsValid(_Col - 1, _Row))        m_vecTileData[GetTileIndex(_Col - 1, _Row)] = true;
		if (IsValid(_Col + 1, _Row))        m_vecTileData[GetTileIndex(_Col + 1, _Row)] = true;
	}

	// 세로선
	else if (isEvenY)
	{
		if (IsValid(_Col, _Row - 1))        m_vecTileData[GetTileIndex(_Col, _Row - 1)] = true;
		if (IsValid(_Col, _Row + 1))        m_vecTileData[GetTileIndex(_Col, _Row + 1)] = true;
	}


}

void MTileEditor::EraseTile(int _Col, int _Row)
{
	m_vecTileData[GetTileIndex(_Col, _Row)] = false;
}


void MTileEditor::DrawGrid()
{
	Vector2 vGridLT = Vector2(m_StartPos.x - m_TileSize.x / 2.f, m_StartPos.y + m_TileSize.y / 2.f);
	Vector2 vGridRB = Vector2(vGridLT.x + m_TileSize.x * ((int)m_TileCount.x / 2 + 1), vGridLT.y - m_TileSize.y * ((int)m_TileCount.y / 2 + 1));

	// 수직 그리드 라인 그리기
	Vector2 vLength = Vector2(m_TileCount.x + 1, m_TileCount.y + 1) * m_TileSize / 2.f;
	Vector2 vEndPos = Vector2(vGridLT.x + vLength.x, vGridLT.y - vLength.y);
	for (int x = 0; x < (int)m_TileCount.x / 2 + 2; ++x)
	{
		float posX = vGridLT.x + (x * m_TileSize.x);
		DrawDebugLine(Vector4(1.f, 1.f, 1.f, 0.5f),    // 흰색, 50% 투명도
			Vector3(posX, vGridLT.y, 0.f),   // 시작점
			Vector3(posX, vGridRB.y, 0.f)); // 끝점
	}

	// 수평 그리드 라인 그리기
	for (int y = 0; y < (int)m_TileCount.y / 2 + 2; ++y)
	{
		float posY = vGridLT.y - (y * m_TileSize.y);
		DrawDebugLine(Vector4(1.f, 1.f, 1.f, 0.5f),    // 흰색, 50% 투명도
			Vector3(vGridLT.x, posY, 0.f),   // 시작점
			Vector3(vGridRB.x, posY, 0.f)); // 끝점
	}
}

void MTileEditor::DrawTileData()
{
	for (int i = 0; i < m_vecTileData.size(); ++i)
	{
		if (!m_vecTileData[i])
			continue;

		Vector2 tilePos = GetTilePos(i % (int)m_TileCount.x, i / (int)m_TileCount.x);

		DrawDebugRect(Vector4(0.f, 1.f, 1.f, 0.5f),
			Vector3(tilePos.x, tilePos.y, 0.f),
			Vector2(10.f, 10.f),
			Vector3(0.f, 0.f, 0.f),
			false,
			0.f,
			true);
	}
}
