
#include "pch.h"
#include "FlipbookPlayerUI.h"

#include <Engine/global.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CFlipbookPlayer.h>
#include <Engine/CFlipbook.h>
#include <Engine/CSprite.h>
#include <Engine/CTransform.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "TreeUI.h"
#include "AnimEditorUI.h"

FlipbookPlayerUI::FlipbookPlayerUI()
	: ComponentUI("FlipbookPlayerUI", COMPONENT_TYPE::FLIPBOOKPLAYER)
	, m_CurFrameIdx(0)
	, m_Selected(0)
{
}

FlipbookPlayerUI::~FlipbookPlayerUI()
{
}

void FlipbookPlayerUI::Render_Update()
{
	ComponentTitle("FlipbookPlayer");

	CFlipbookPlayer* pFlipPlayer = GetTargetObject()->FlipbookPlayer();
	vector<Ptr<CFlipbook>>& pList = pFlipPlayer->GetFlipbookList();

	// Flipbook 정보 출력
	ImGui::Text("Flipbook List");
	ImGui::SameLine(100);
	if (ImGui::Button("##FlipbookBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->SetName("Flipbook");
		pListUI->SetActive(true);

		pListUI->AddItem("None");

		vector<wstring> vecAssetNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::FLIPBOOK, vecAssetNames);
		pListUI->AddItem(vecAssetNames);

		pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&FlipbookPlayerUI::SelectFlipbook);
	}


	if (ImGui::BeginListBox("##FlipbookList", ImVec2(-1, 0)))
	{
		for (int i = 0; i < pList.size(); ++i)
		{
			wstring wname = std::to_wstring(i) + L" : ";
			if (nullptr == pList[i])
				wname += L"None";
			else
				wname += pList[i]->GetName();

			string name = string(wname.begin(), wname.end());

			bool isSelected = false;
			if (m_Selected == i)
				isSelected = true;

			if (ImGui::Selectable(name.c_str(), &isSelected))
			{
				m_Selected = i;
			}

			// 드래그 드롭 기능
			if (ImGui::BeginDragDropTarget())
			{
				if (ImGui::AcceptDragDropPayload("ContentUI"))
				{
					const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
					TreeNode* pNode = *((TreeNode**)pPayload->Data);
					Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();

					if (pAsset->GetAssetType() == ASSET_TYPE::FLIPBOOK)
					{
						pFlipPlayer->AddFlipbook(i, (CFlipbook*)pAsset.Get());
						ImGui::SetWindowFocus(nullptr);
					}
				}
				ImGui::EndDragDropTarget();
			}
		}

		// 플립북 추가하기
		string add = std::to_string(pList.size()) + " : Add Flipbook";

		if (ImGui::Selectable(add.c_str()))
		{
			m_Selected = pList.size();

			ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
			pListUI->SetName("Flipbook");
			pListUI->SetActive(true);

			pListUI->AddItem("None");

			vector<wstring> vecAssetNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::FLIPBOOK, vecAssetNames);
			pListUI->AddItem(vecAssetNames);

			pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&FlipbookPlayerUI::SelectFlipbook);
		}

		// 드래그 드롭 기능
		if (ImGui::BeginDragDropTarget())
		{
			if (ImGui::AcceptDragDropPayload("ContentUI"))
			{
				const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
				TreeNode* pNode = *((TreeNode**)pPayload->Data);
				Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();

				if (pAsset->GetAssetType() == ASSET_TYPE::FLIPBOOK)
				{
					m_Selected = pList.size();

					pFlipPlayer->AddFlipbook(m_Selected, (CFlipbook*)pAsset.Get());
					ImGui::SetWindowFocus(nullptr);
				}
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::EndListBox();

		// 추가된 Flipbook 없으면 Flipbook 관련 세부사항 x
		if (pList.size() == 0)
		{
			ImGui::Text("No Flipbook Vaild");
			return;
		}

		static bool bAnimSaved = true;
		static bool bAnimLoop = true;
		// Level Pause 상태에서는 실행 안됨
		// Play 버튼
		if (ImGui::Button("Play", ImVec2(60, 25)))
		{
			pFlipPlayer->Play(m_Selected, 10, bAnimLoop);
		}

		ImGui::SameLine();

		// Pause 버튼 
		if (ImGui::Button("Pause", ImVec2(60, 25)))
		{
			pFlipPlayer->Pause();
		}
		ImGui::SameLine();

		// Pause 버튼 
		if (ImGui::Button(bAnimSaved ? "Saved" : "Save", ImVec2(60, 25)))
		{
			bAnimSaved = true;

			Ptr<CFlipbook> pFlipbook = pFlipPlayer->GetFlipbook(m_Selected);
			pFlipbook->Save(CPathMgr::GetInst()->GetContentPath() + pFlipbook->GetRelativePath());
		}

		ImGui::SameLine();
		// Open Editor 버튼 
		if (ImGui::Button("Edit..", ImVec2(60, 25)))
		{
			AnimEditorUI* pEditor = (AnimEditorUI*)CImGuiMgr::GetInst()->FindUI("AnimationEditor");
			pEditor->SetCurAnim(pFlipPlayer->GetCurFlipbook());
			pEditor->SetActive(true);
		}


		// === Flipbook Info ===
		Ptr<CFlipbook> pFlipbook = pFlipPlayer->GetFlipbook(m_Selected);
		if (nullptr != pFlipbook)
		{
			// Loop
			bAnimLoop = pFlipbook->GetLoop();

			ImGui::Text("Loop");
			ImGui::SameLine(100);
			ImGui::SetNextItemWidth(150);
			if (ImGui::Checkbox("##Loop", &bAnimLoop))
			{
				pFlipbook->SetLoop(bAnimLoop);
				pFlipbook->Save(CPathMgr::GetInst()->GetContentPath() + pFlipbook->GetRelativePath());
			}

			// Offset
			float Offset[2] = { pFlipbook->GetAnimOffset().x, pFlipbook->GetAnimOffset().y };
			ImGui::Text("Offset");
			ImGui::SameLine(100);
			ImGui::SetNextItemWidth(150);
			if (ImGui::DragFloat2("##Offset", Offset))
			{
				bAnimSaved = false;
				pFlipbook->SetAnimOffset(Vector2(Offset[0], Offset[1]));
				//pFlipbook->Save(CPathMgr::GetInst()->GetContentPath() + pFlipbook->GetRelativePath());
			}
		}
	}
}

void FlipbookPlayerUI::SelectFlipbook(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	// flipbook list
	vector<Ptr<CFlipbook>>& pList = GetTargetObject()->FlipbookPlayer()->GetFlipbookList();

	if (*pStr == "None")
	{
		pList[m_Selected] = nullptr;
		return;
	}

	Ptr<CFlipbook> pFlipbook = CAssetMgr::GetInst()->FindAsset<CFlipbook>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pFlipbook)
		return;

	GetTargetObject()->FlipbookPlayer()->AddFlipbook(m_Selected, pFlipbook);
}