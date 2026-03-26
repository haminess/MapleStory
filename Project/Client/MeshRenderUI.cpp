#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>
#include <Engine/CMesh.h>
#include <Engine/CMaterial.h>
#include <Engine/CTexture.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

MeshRenderUI::MeshRenderUI()
	: ComponentUI("MeshRenderUI", COMPONENT_TYPE::MESHRENDER)
{
}

MeshRenderUI::~MeshRenderUI()
{
}

void MeshRenderUI::Render_Update()
{
	ComponentTitle("MeshRender");

	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();

	Ptr<CMesh> pMesh = pMeshRender->GetMesh();

	// [메쉬 정보 출력]
	string MeshName;
	if (nullptr == pMesh)
		MeshName = "None";
	else
	{
		MeshName = string(pMesh->GetKey().begin(), pMesh->GetKey().end());
	}

	ImGui::Text("Mesh");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150);
	ImGui::InputText("##MeshName", (char*)MeshName.c_str(), MeshName.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		// 1차 필터링: Payload ID
		if (ImGui::AcceptDragDropPayload("ContentUI"))
		{
			// 마우스 클릭 뗐을 때 들어옴
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();

			// Data는 TreeNode 주소를 담은 힙메모리 주소
			// 포인터 구조: pPayload -> Data -> (내가 넣은 데이터, 여기서는 TreeNode 포인터)
			TreeNode* pNode = (*(TreeNode**)(pPayload->Data));

			// 아래 구문이 안되는 이유, void 포인터는 값 해석을 할 수 없다
			//TreeNode* pNode = (TreeNode*)(*(pPayload->Data));	

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();

			// 2차 필터링: 원하는 데이터 타입
			if (pAsset->GetAssetType() == ASSET_TYPE::MESH)
			{

			}
		}

		ImGui::EndDragDropTarget();
	}
	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", ImVec2(18.f, 18.f)))
	{
		// ListUI 를 활성화 시키기
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->SetName("Mesh");
		pListUI->SetActive(true);

		// ListUI 에 넣어줄 문자열 정보 가져오기
		pListUI->AddItem("None");

		vector<wstring> vecAssetNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MESH, vecAssetNames);
		pListUI->AddItem(vecAssetNames);

		// 더블 클릭 시 호출시킬 함수 등록
		pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&MeshRenderUI::SelectMesh);
	}


	// [메테리얼 정보 출력]
	Ptr<CMaterial> pMtrl = pMeshRender->GetMaterial();
	string MtrlName;
	if (nullptr == pMtrl)
	{
		MtrlName = "None";
	}
	else
	{
		MtrlName = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	}

	ImGui::Text("Material");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150);
	ImGui::InputText("##MtrlName", (char*)MtrlName.c_str(), MtrlName.length(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("ContentUI"))
		{
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
			TreeNode* pNode = *((TreeNode**)pPayload->Data);
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (pAsset->GetAssetType() == ASSET_TYPE::MATERIAL)
			{
				pMeshRender->SetMaterial((CMaterial*)pAsset.Get());
				ImGui::SetWindowFocus(nullptr);
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##MtrlBtn", ImVec2(18.f, 18.f)))
	{
		// ListUI 를 활성화 시키기
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->SetName("Material");
		pListUI->SetActive(true);

		// ListUI 에 넣어줄 문자열 정보 가져오기
		pListUI->AddItem("None");

		vector<wstring> vecAssetNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MATERIAL, vecAssetNames);
		pListUI->AddItem(vecAssetNames);

		// 더블 클릭 시 호출시킬 함수 등록
		pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&MeshRenderUI::SelectMaterial);
	}

	if (nullptr == pMtrl)
		return;

	// [텍스처 정보 출력]
	string TexName;
	Ptr<CTexture> pTex = pMtrl->GetTexture(m_TexParam);
	if (nullptr == pTex)
		TexName = "None";
	else
	{
		TexName = string(pTex->GetKey().begin(), pTex->GetKey().end());
	}

	ImGui::Text("");
	ImGui::SameLine(ImGui::GetWindowWidth() - 100.f);
	if (nullptr == pMeshRender->GetDynamicMaterial())
	{
		if (ImGui::Button("Create Dynamic Material"))
		{
			pMeshRender->CreateDynamicMaterial();
		}
	}

	ImGui::Text("");
	ImGui::Text("Texture");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150);

	TexName = std::to_string((int)m_TexParam) + " : " + string(TexName.begin(), TexName.end());

	if (ImGui::BeginCombo("##TexParam", TexName.c_str(), 0))
	{
		string itemName;
		for (int i = 0; i < TEX_PARAM::TEX_END; i++)
		{
			pTex = pMtrl->GetTexture(i);
			if (nullptr == pTex)
				TexName = "None";
			else
				TexName = string(pTex->GetKey().begin(), pTex->GetKey().end());
			itemName = std::to_string(i) + " : " + TexName;
			if (ImGui::Selectable(itemName.c_str()))
			{
				m_TexParam = (TEX_PARAM)i;
			}
		}
		ImGui::EndCombo();
	}

	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("ContentUI"))
		{
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
			TreeNode* pNode = *((TreeNode**)pPayload->Data);
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (pAsset->GetAssetType() == ASSET_TYPE::TEXTURE)
			{
				pMeshRender->GetMaterial()->SetTexParam(m_TexParam, ((CTexture*)pAsset.Get()));
				ImGui::SetWindowFocus(nullptr);
			}
		}

		ImGui::EndDragDropTarget();
	}

	float* alpha = (float*)pMtrl->GetScalarParam(SCALAR_PARAM::FLOAT_0);
	if (ImGui::InputFloat("Alpha", alpha))
	{
		pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, alpha);
	}


	ImGui::SameLine();
	if (ImGui::Button("##TexBtn", ImVec2(18.f, 18.f)))
	{
		// ListUI 를 활성화 시키기
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->SetName("Texture");
		pListUI->SetActive(true);

		// ListUI 에 넣어줄 문자열 정보 가져오기
		pListUI->AddItem("None");

		vector<wstring> vecAssetNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecAssetNames);
		pListUI->AddItem(vecAssetNames);

		// 더블 클릭 시 호출시킬 함수 등록
		pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&MeshRenderUI::SelectTexture);
	}


	ImGui::Text("");
	ImGui::SameLine(ImGui::GetWindowWidth() - 150.f); // 오른쪽에 버튼 배치
	if (ImGui::Button("Set Native Size"))
	{
		if (pTex != nullptr)
		{
			Vector2 vSize = pTex->GetResolution();
			Vector3 vScale = pMeshRender->GetOwner()->Transform()->GetWorldScale();
			pTex = pMtrl->GetTexture(0);
			pMeshRender->GetOwner()->Transform()->UseIndependentScale(true);
			pMeshRender->GetOwner()->Transform()->SetRelativeScale(vSize.x, vSize.y, vScale.z);
		}
	}

}

void MeshRenderUI::SelectMesh(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// 리스트에서 더블킬릭한 항목의 이름을 받아온다.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
	{
		GetTargetObject()->MeshRender()->SetMesh(nullptr);
		return;
	}

	// 해당 항목 에셋을 찾아서, MeshRenderComponent 가 해당 메시를 참조하게 한다.
	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pMesh)
		return;

	GetTargetObject()->MeshRender()->SetMesh(pMesh);
}

void MeshRenderUI::SelectMaterial(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// 리스트에서 더블킬릭한 항목의 이름을 받아온다.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
	{
		GetTargetObject()->MeshRender()->SetMaterial(nullptr);
		return;
	}

	// 해당 항목 에셋을 찾아서, MeshRenderComponent 가 해당 메시를 참조하게 한다.
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pMtrl)
		return;

	GetTargetObject()->MeshRender()->SetMaterial(pMtrl);
}

void MeshRenderUI::SelectTexture(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// 리스트에서 더블킬릭한 항목의 이름을 받아온다.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
	{
		GetTargetObject()->MeshRender()->SetMaterial(nullptr);
		return;
	}

	// 해당 항목 에셋을 찾아서, MeshRenderComponent 가 해당 메시를 참조하게 한다.
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pTex)
		return;

	GetTargetObject()->MeshRender()->GetMaterial()->SetTexParam(m_TexParam, pTex);
}
