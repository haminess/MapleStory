#include "pch.h"
#include "Outliner.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CTaskMgr.h>

#include "CImGuiMgr.h"
#include "Inspector.h"
#include "TreeUI.h"


Outliner::Outliner()
	: EditorUI("Outliner")
{
	m_Tree = (TreeUI*)AddChildUI(new TreeUI);
	m_Tree->SetName("Outliner");
	m_Tree->ShowRoot(false);
	m_Tree->SelfDragDrop(true);

	m_Tree->AddDynamicSelect(this, (EUI_DELEGATE_1)&Outliner::SelectGameObject);
	m_Tree->AddDynamicSelfDragDrop(this, (EUI_DELEGATE_2)&Outliner::DragDrop);
}

Outliner::~Outliner()
{
}

void Outliner::Tick()
{
}

void Outliner::Render_Update()
{

	// Simple selection popup (if you want to show the current selection inside the Button itself,
	// you may want to build a string using the "###" operator to preserve a constant ID with a variable label)


	if (ImGui::IsItemHovered() && KEY_TAP(KEY::RBTN))
		ImGui::OpenPopup("Context");
	if (ImGui::BeginPopup("Context"))
	{
		if (ImGui::Selectable("Create EmptyObject"))
		{
			CGameObject* pNew = new CGameObject;
			pNew->SetName(L"GameObject (New)");

			CreateObject(pNew, 0, false);
		}

		ImGui::Separator();
		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::BeginMenu("2D Object"))
			{
				ImGui::MenuItem("Square");
				ImGui::MenuItem("Circle");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}



	if (CTaskMgr::GetInst()->IsLevelChanged())
	{
		RenewGameObject();
	}
}

void Outliner::Context(TreeNode* _TreeNode)
{
	CGameObject* pObj = (CGameObject*)_TreeNode->GetData();
	if (ImGui::Selectable("Rename"))
	{
		_TreeNode->BeginRename();
	}
	if (ImGui::Selectable("Delete"))
	{
		if(pObj)
			DestroyObject(pObj);
	}
	if (ImGui::Selectable("Duplicate"))
	{
		if (pObj)
		{
			CGameObject* pClone = pObj->Clone();
			CreateObject(pClone, pClone->GetLayerIdx(), false);
			pClone->SetName(pObj->GetName() + L" (Clone)");
		}
	}

	ImGui::Separator();

	if (ImGui::Selectable("Create Prefab"))
	{
		CGameObject* pProto = pObj->Clone();
		Ptr<CPrefab> pPrefab = new CPrefab;
		pPrefab->SetProtoObject(pProto);
		pPrefab->Save(CPathMgr::GetInst()->GetContentPath() + pPrefab->GetRelativePath());

		CAssetMgr::GetInst()->DeleteAsset<CPrefab>(pPrefab->GetRelativePath());
		CAssetMgr::GetInst()->AddAsset<CPrefab>(pPrefab->GetRelativePath(), pPrefab);
	}
}

void Outliner::RenewGameObject()
{
	m_Tree->Clear();
	TreeNode* pRootNode = m_Tree->AddItem(nullptr, "RootNode", 0);

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (pCurLevel)
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			CLayer* pLayer = pCurLevel->GetLayer(i);

			const vector<CGameObject*>& vecParents = pLayer->GetParentObjects();
			for (size_t j = 0; j < vecParents.size(); ++j)
			{
				AddGameObject(pRootNode, vecParents[j]);
			}
		}

		// 오브젝트 삭제한 경우 예외처리* 
		// -> 오브젝트 삭제하면 바로 SetTargetObject(nullptr)하기
		Inspector* pIns = CImGuiMgr::GetInst()->GetInspector();
		pIns->SetTargetObject(pIns->GetTargetObject());
	}
	else
	{
		m_Tree->AddItem(pRootNode, "None", 0);
	}

}

void Outliner::AddGameObject(TreeNode* _ParentNode, CGameObject* _Object)
{
	wstring ObjName = _Object->GetName();
	string Name = string(ObjName.begin(), ObjName.end());

	TreeNode* pCurNode = m_Tree->AddItem(_ParentNode, Name, (DWORD_PTR)_Object);

	const vector<CGameObject*>& vecChild = _Object->GetChild();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObject(pCurNode, vecChild[i]);
	}
}

void Outliner::SelectGameObject(DWORD_PTR _TreeNode)
{
	TreeNode* pNode = (TreeNode*)_TreeNode;
	CGameObject* pTarget = (CGameObject*)pNode->GetData();

	Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspector->SetTargetObject(pTarget);
}

void Outliner::DragDrop(DWORD_PTR _DragNode, DWORD_PTR _DropNode)
{
	TreeNode* pDragged = (TreeNode*)_DragNode;
	TreeNode* pDropped = (TreeNode*)_DropNode;

	CGameObject* pDragObj = (CGameObject*)pDragged->GetData();
	CGameObject* pDropObj = nullptr;

	if (pDropped)
	{
		pDropObj = (CGameObject*)pDropped->GetData();

		// 드래그 된 오브젝트가 목적지 오브젝트의 조상 중 하나라면, 그 상황을 방지한다.
		if (pDropObj->IsAncestor(pDragObj))
			return;
	}

	AddChild(pDropObj, pDragObj);
}
