#include "pch.h"
#include "MObjectEditor.h"
#include "CImGuiMgr.h"
#include "ListUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMaterial.h>
#include <Engine/CTexture.h>
#include <Engine/CFlipbook.h>
#include <Engine/CFlipbookPlayer.h>
#include <Engine/CMeshRender.h>
#include <Engine/CTransform.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CFontMgr.h>

#include <Scripts/CPlatformScript.h>
#include <Scripts/CWallScript.h>
#include <Scripts/CRope.h>

MObjectEditor::MObjectEditor()
	: PresetList("Object")
	, m_IsPlatformMode(false)
{
	auto& prerAsset = CAssetMgr::GetInst()->GetAssets(ASSET_TYPE::PREFAB);
	for (auto& iter = prerAsset.begin(); iter != prerAsset.end(); ++iter)
	{
		Ptr<CAsset> pAsset = iter->second.Get();
		Ptr<CPrefab> pPref = (CPrefab*)iter->second.Get();
		wstring wname = CPathMgr::GetInst()->GetNameFromPath(pPref->GetName(), false);
		string name = string(wname.begin(), wname.end());

		CMeshRender* pRender = pPref->GetPrefabInfo()->MeshRender();
		Ptr<CMaterial> pMtrl = nullptr;
		Ptr<CTexture> pTex = nullptr;
		if (nullptr != pRender) pMtrl = pRender->GetMaterial();
		if (nullptr != pMtrl) pTex = pMtrl->GetTexParam(TEX_0);

		LoadPrefab(pPref);
		AddPreset(name, pAsset, pTex);
	}
}

MObjectEditor::~MObjectEditor()
{
}

void MObjectEditor::LoadPrefab(Ptr<CPrefab> _New)
{
	m_vecPrefabs.push_back(_New);
}

void MObjectEditor::CreateTexturePrefab(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// 리스트에서 더블킬릭한 항목의 이름을 받아온다.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
		return;
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pTex)
		return;

	CGameObject* pProto = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"MObject")->Instantiate();
	pProto->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pTex);
	pProto->SetName(pTex->GetName());
	pProto->Transform()->SetRelativeScale(pTex->GetResolution().x, pTex->GetResolution().y, 1.f);

	// 프리팹 생성
	Ptr<CPrefab> pPrefab = new CPrefab;
	pPrefab->SetProtoObject(pProto);
	pPrefab->SetName(CPathMgr::GetInst()->GetNameFromPath(pTex->GetName(), false));
	pPrefab->Save(CPathMgr::GetInst()->GetContentPath() + pPrefab->GetRelativePath());

	// 이름 설정
	CAssetMgr::GetInst()->AddAsset<CPrefab>(pPrefab->GetRelativePath(), pPrefab);
	LoadPrefab(pPrefab);
	wstring name = pPrefab->GetName();

	// 이미지 설정
	CMeshRender* pRender = pPrefab->GetPrefabInfo()->MeshRender();
	Ptr<CMaterial> pMtrl = nullptr;
	Ptr<CTexture> pPrefabTex = nullptr;
	if (nullptr != pRender) pMtrl = pRender->GetMaterial();
	if (nullptr != pMtrl) pPrefabTex = pMtrl->GetTexParam(TEX_0);
	AddPreset(string(name.begin(), name.end()), pPrefab.Get(), pPrefabTex);
}

void MObjectEditor::AddPrefabFromListUI(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// 리스트에서 더블킬릭한 항목의 이름을 받아온다.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
		return;
	Ptr<CPrefab> pPrefab = CAssetMgr::GetInst()->FindAsset<CPrefab>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pPrefab)
		return;

	// 프리팹 생성
	LoadPrefab(pPrefab);
}

void MObjectEditor::CreatePresetObject(int _LayerIdx)
{
	::CreateObject(m_NewObject, _LayerIdx, false);
	m_NewObject = nullptr;
}

void MObjectEditor::CreatePlatform(Vector2& _MouseCurPos)
{
	// shift를 누르고 있으면 수평/수직으로 생성
	if (KEY_PRESSED(KEY::LSHIFT))
	{
		float dx = _MouseCurPos.x - m_MouseStartPos.x;
		float dy = _MouseCurPos.y - m_MouseStartPos.y;

		if (fabsf(dx) >= fabsf(dy))
			_MouseCurPos.y = m_MouseStartPos.y;
		else
			_MouseCurPos.x = m_MouseStartPos.x;
	}


	if (KEY_PRESSED(KEY::LBTN))
	{
		DrawDebugLine(
			Vector4(1.f, 1.f, 0.f, 1.f),
			m_MouseStartPos, Vector3(_MouseCurPos.x, _MouseCurPos.y, 0.f)
		);
	}

	if (KEY_RELEASED(KEY::LBTN))
	{
		Vector3 vEndPos = Vector3(_MouseCurPos.x, _MouseCurPos.y, 0.f);
		if ((m_MouseStartPos - vEndPos).Length() < 5.f)
			return;

		// 플랫폼 생성
		m_NewObject = new CGameObject;
		m_NewObject->AddComponent(new CPlatformScript);
		m_NewObject->GetScript<CPlatformScript>()->SetPlatformPos(
			m_MouseStartPos, vEndPos
		);

		CreatePresetObject(23);
	}
}

void MObjectEditor::CreateRope(Vector2& _MouseCurPos)
{
	Vector3 vEndPos = Vector3(_MouseCurPos.x, _MouseCurPos.y, 0.f);

	Vector2 vMinPos(min(m_MouseStartPos.x, vEndPos.x), min(m_MouseStartPos.y, vEndPos.y));
	Vector2 vMaxPos(max(m_MouseStartPos.x, vEndPos.x), max(m_MouseStartPos.y, vEndPos.y));

	Vector3 vPos((vMinPos.x + vMaxPos.x) * 0.5f, (vMinPos.y + vMaxPos.y) * 0.5f, 0.f);
	Vector3 vScale(vMaxPos.x - vMinPos.x, vMaxPos.y - vMinPos.y, 1.f);

	if (KEY_PRESSED(KEY::LBTN))
	{
		DrawDebugRect(
			Vector4(1.f, 1.f, 0.f, 1.f),
			vPos, Vector2(vScale.x, vScale.y),
			Vector3(0.f, 0.f, 0.f)
		);
	}

	if (KEY_RELEASED(KEY::LBTN))
	{
		if ((m_MouseStartPos - vEndPos).Length() < 5.f)
			return;

		// 벽 생성
		m_NewObject = new CGameObject;
		m_NewObject->AddComponent(new CCollider2D);
		m_NewObject->AddComponent(new CRope);
		m_NewObject->Transform()->SetWorldPos(vPos);
		m_NewObject->Transform()->SetRelativeScale(vScale);
		m_NewObject->Collider2D()->SetIndependentScale(false);
		m_NewObject->Collider2D()->SetScale(Vector2(1.f, 1.f));

		CreatePresetObject(23);
	}
}

void MObjectEditor::CreateWall(Vector2& _MouseCurPos)
{
	Vector3 vEndPos = Vector3(_MouseCurPos.x, _MouseCurPos.y, 0.f);

	Vector2 vMinPos(min(m_MouseStartPos.x, vEndPos.x), min(m_MouseStartPos.y, vEndPos.y));
	Vector2 vMaxPos(max(m_MouseStartPos.x, vEndPos.x), max(m_MouseStartPos.y, vEndPos.y));

	Vector3 vPos((vMinPos.x + vMaxPos.x) * 0.5f, (vMinPos.y + vMaxPos.y) * 0.5f, 0.f);
	Vector3 vScale(vMaxPos.x - vMinPos.x, vMaxPos.y - vMinPos.y, 1.f);

	if (KEY_PRESSED(KEY::LBTN))
	{
		DrawDebugRect(
			Vector4(1.f, 1.f, 0.f, 1.f),
			vPos, Vector2(vScale.x, vScale.y),
			Vector3(0.f, 0.f, 0.f)
		);
	}

	if (KEY_RELEASED(KEY::LBTN))
	{
		if ((m_MouseStartPos - vEndPos).Length() < 5.f)
			return;

		// 밧줄 생성
		m_NewObject = new CGameObject;
		m_NewObject->AddComponent(new CCollider2D);
		m_NewObject->AddComponent(new CWallScript);
		m_NewObject->Transform()->SetWorldPos(vPos);
		m_NewObject->Transform()->SetRelativeScale(vScale);
		m_NewObject->Collider2D()->SetIndependentScale(false);
		m_NewObject->Collider2D()->SetScale(Vector2(1.f, 1.f));

		CreatePresetObject(23);
	}
}

void MObjectEditor::SelectPreset(int _Idx)
{
	// 오브젝트 맵 설치
	m_NewObject = m_vecPrefabs[_Idx]->Instantiate();
	::CreateObject(m_NewObject, m_NewObject->GetLayerIdx(), false);
}

void MObjectEditor::Render_Editor()
{
	// 설치중인 오브젝트 렌더링
	if (m_NewObject)
	{
		Vector2 vPos = CKeyMgr::GetInst()->GetMouseWorldPos();
		m_NewObject->Transform()->SetWorldPos(Vector3(vPos.x, vPos.y, 0.f));
	}

	if (m_NewObject && KEY_RELEASED(KEY::LBTN))
	{
		m_NewObject = nullptr;
	}


	// 생성 패널
	static bool bCreatePanel = true;
	if (DrawTitleUI("Create"))
		bCreatePanel = !bCreatePanel;

	if (bCreatePanel)
	{
		if (ImGui::Button("Load Prefab Data"))
		{
			// ListUI 를 활성화 시키기
			ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
			pListUI->SetName("Prefab");
			pListUI->SetActive(true);

			// ListUI 에 넣어줄 문자열 정보 가져오기
			pListUI->AddItem("None");

			vector<wstring> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::PREFAB, vecTexNames);
			pListUI->AddItem(vecTexNames);

			// 더블 클릭 시 호출시킬 함수 등록
			pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&MObjectEditor::AddPrefabFromListUI);
		}
		if (ImGui::Button("Texture Object"))
		{
			// ListUI 를 활성화 시키기
			ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
			pListUI->SetName("Texture");
			pListUI->SetActive(true);

			// ListUI 에 넣어줄 문자열 정보 가져오기
			pListUI->AddItem("None");

			vector<wstring> vecTexNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
			pListUI->AddItem(vecTexNames);

			// 더블 클릭 시 호출시킬 함수 등록
			pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&MObjectEditor::CreateTexturePrefab);

			//OpenAssetListUI(ASSET_TYPE::TEXTURE, (EditorUI*)this, (EUI_DELEGATE_2)&MObjectEditor::CreateTexturePrefab);

		}

		float spacing = ImGui::GetStyle().ItemSpacing.x;
		float avail = ImGui::GetContentRegionAvail().x;
		float buttonWidth = (avail - spacing * 2.0f) / 3.0f;
		ImVec2 size(buttonWidth, 0);

		string strMode;
		strMode = m_EditMode == EObjectEditorMode::PLATFORM ? "Stop" : "Platform";
		if (ImGui::Button(strMode.c_str(), size))
		{
			if (m_EditMode == EObjectEditorMode::PLATFORM)
				m_EditMode = EObjectEditorMode::NONE;
			else
				m_EditMode = EObjectEditorMode::PLATFORM;
		}
		ImGui::SameLine();

		strMode = m_EditMode == EObjectEditorMode::WALL ? "Stop" : "Wall";
		if (ImGui::Button(strMode.c_str(), size))
		{
			if (m_EditMode == EObjectEditorMode::WALL)
				m_EditMode = EObjectEditorMode::NONE;
			else
				m_EditMode = EObjectEditorMode::WALL;
		}
		ImGui::SameLine();

		strMode = m_EditMode == EObjectEditorMode::ROPE ? "Stop" : "Rope";
		if (ImGui::Button(strMode.c_str(), size))
		{
			if (m_EditMode == EObjectEditorMode::ROPE)
				m_EditMode = EObjectEditorMode::NONE;
			else
				m_EditMode = EObjectEditorMode::ROPE;
		}

		ImGui::Separator();
	}



	static bool bDrawing = false;
	static Vector2 vMouseCurPos;
	if (KEY_TAP(KEY::LBTN))
	{
		vMouseCurPos = CKeyMgr::GetInst()->GetMouseWorldPos();
		m_MouseStartPos = Vector3(vMouseCurPos.x, vMouseCurPos.y, 0.f);

		bDrawing = true;
	}

	if (m_EditMode == EObjectEditorMode::NONE || ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
		bDrawing = false;

	if (!bDrawing)
		return;

	vMouseCurPos = CKeyMgr::GetInst()->GetMouseWorldPos();
	switch (m_EditMode)
	{
	case EObjectEditorMode::PLATFORM:
		CreatePlatform(vMouseCurPos);
		break;
	case EObjectEditorMode::WALL:
		CreateWall(vMouseCurPos);
		break;
	case EObjectEditorMode::ROPE:
		CreateRope(vMouseCurPos);
		break;
	}

	if (KEY_RELEASED(KEY::LBTN))
		bDrawing = false;
}

