#include "pch.h"
#include "PrefabUI.h"

#include <Engine/CPrefab.h>
#include <Engine/CPathMgr.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

PrefabUI::PrefabUI()
	: AssetUI("Prefab", ASSET_TYPE::PREFAB)
	, m_NewObject(nullptr)
{
}

PrefabUI::~PrefabUI()
{
}

void PrefabUI::Render_Update()
{
	AssetTitle();

	Ptr<CPrefab> pAsset = dynamic_cast<CPrefab*>(GetAsset().Get());
	assert(pAsset.Get());

	ImGui::Text("Name");
	ImGui::SameLine(100);

	string strKey = string(pAsset->GetKey().begin(), pAsset->GetKey().end());
	ImGui::InputText("##PrefabName", (char*)strKey.c_str(), strKey.length());


	// 프리팹 생성
	if (m_NewObject)
	{
		Vector2 vPos = CKeyMgr::GetInst()->GetMouseWorldPos();
		m_NewObject->Transform()->SetWorldPos(Vector3(vPos.x, vPos.y, 0.f));

		if (KEY_RELEASED(KEY::LBTN))
			m_NewObject = nullptr;
	}

	if (ImGui::Button("Instantiate"))
	{
		m_NewObject = pAsset->Instantiate();
		CreateObject(m_NewObject, m_NewObject->GetLayerIdx(), false);
	}


	if (ImGui::Button("Save"))
	{
		wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

		// 파일 경로 문자열
		wchar_t szFilePath[255] = {};

		OPENFILENAME Desc = {};

		Desc.lStructSize = sizeof(OPENFILENAME);
		Desc.hwndOwner = nullptr;
		Desc.lpstrFile = szFilePath;
		Desc.nMaxFile = 255;
		Desc.lpstrFilter = L"Prefab\0*.pref\0ALL\0*.*";
		Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		Desc.lpstrInitialDir = strContentPath.c_str();

		if (GetSaveFileName(&Desc))
		{
			if (wcslen(szFilePath) != 0)
			{
				wstring path = wstring(szFilePath) + L".pref";
				pAsset->Save(path);
			}


		}
	}



	ImGui::Separator();
	if (ImGui::Button("Save All Prefabs"))
	{
		CAssetMgr::GetInst()->SaveAllPrefab();
	}
}