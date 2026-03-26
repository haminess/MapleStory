
#include "pch.h"
#include "StateMachineUI.h"

#include <Engine/global.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CStateMachine.h>
#include <Engine/CTransform.h>

#include <Scripts/CScriptMgr.h>

#include "CImGuiMgr.h"
#include "ListUI.h"
#include "TreeUI.h"
#include "AnimEditorUI.h"

StateMachineUI::StateMachineUI()
	: ComponentUI("StateMachineUI", COMPONENT_TYPE::STATEMACHINE)
{
}

StateMachineUI::~StateMachineUI()
{
}

void StateMachineUI::Render_Update()
{
	ComponentTitle("StateMachine");

	CStateMachine* pSM = GetTargetObject()->StateMachine();
	const auto& List = pSM->GetStates();

	// Flipbook 정보 출력
	ImGui::Text("State List");
	if (ImGui::BeginListBox("##StateList", ImVec2(-1, 0)))
	{
		for (auto& State : List)
		{
			bool isSelected = false;
			if(State.second == pSM->GetCurrentState())
				isSelected = true;

			wstring wname = State.first;
			string name = string(wname.begin(), wname.end());
			if (ImGui::Selectable(name.c_str(), &isSelected))
			{
				pSM->ChangeState(State.first);
			}
		}

		// 플립북 추가하기
		if (ImGui::Selectable("Add State"))
		{
			ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
			pListUI->SetName("Flipbook");
			pListUI->SetActive(true);

			pListUI->AddItem("None");

			vector<wstring> vecStateName;
			CScriptMgr::GetScriptInfo(vecStateName);
			pListUI->AddItem(vecStateName);

			pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&StateMachineUI::SelectState);
		}

		ImGui::EndListBox();
	}
}

void StateMachineUI::SelectState(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;
	string sScriptName = *pStr;
	wstring wScriptName = wstring(sScriptName.begin(), sScriptName.end());


	if (*pStr == "None")
		return;

	CState* pState = (CState*)CScriptMgr::GetScript(wScriptName);
	if (pState)
		GetTargetObject()->StateMachine()->AddState(wScriptName, pState);
}