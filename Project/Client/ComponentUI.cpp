#include "pch.h"
#include "ComponentUI.h"
#include "ListUI.h"
#include "CImGuiMgr.h"
#include "Inspector.h"
#include <Scripts/CScriptMgr.h>
#include <Engine/CComponent.h>

ComponentUI::ComponentUI(const string& _ID, COMPONENT_TYPE _Type)
    : EditorUI(_ID)
    , m_TargetObject(nullptr)
    , m_Type(_Type)
{
}

ComponentUI::ComponentUI(const string& _ID)
    : EditorUI(_ID)
    , m_TargetObject(nullptr)
    , m_Type(COMPONENT_TYPE::END)
{
}

ComponentUI::~ComponentUI()
{
}

void ComponentUI::SetTargetObject(CGameObject* _Target)
{
    m_TargetObject = _Target;

    if (nullptr == m_TargetObject)
    {
        SetActive(false);
    }
    else
    {
        // 본인 컴포넌트 없으면 비활성화
        CComponent* pComponent = m_TargetObject->GetComponent(m_Type);
        if (nullptr == pComponent)
        {
            SetActive(false);
        }
        else
        {
            SetActive(true);
        }
    }

}

void ComponentUI::ComponentTitle(const string& _title)
{
    ImGui::PushID(0);

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.6f, 0.6f));

    if (ImGui::Button(_title.c_str()))
    {
		ImGui::OpenPopup("Context");
    }
	ComponentContext();

    ImGui::PopStyleColor(3);
    ImGui::PopID();
}

void ComponentUI::ComponentContext()
{
	if (ImGui::BeginPopup("Context"))
	{
		if (ImGui::Selectable("Add Component"))
		{
            // ListUI 를 활성화 시키기
            ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
            pListUI->SetName("Component");
            pListUI->SetActive(true);

            // ListUI 에 넣어줄 문자열 정보 가져오기
            pListUI->AddItem("None");

            vector<wstring> vecAssetNames;
            for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
            {
                string strCom = (char*)COMPONENT_TYPE_STRING[i];
                vecAssetNames.push_back(wstring(strCom.begin(), strCom.end()));
            }
            pListUI->AddItem(vecAssetNames);

            // 더블 클릭 시 호출시킬 함수 등록
            pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&ComponentUI::SelectComponent);
		}
		if (ImGui::Selectable("Add Script"))
		{
            // ListUI 를 활성화 시키기
            ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
            pListUI->SetName("Texture");
            pListUI->SetActive(true);

            // ListUI 에 넣어줄 문자열 정보 가져오기
            pListUI->AddItem("None");

            vector<wstring> vecAssetNames;
            CScriptMgr::GetScriptInfo(vecAssetNames);
            pListUI->AddItem(vecAssetNames);

            // 더블 클릭 시 호출시킬 함수 등록
            pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&ComponentUI::SelectScript);
		}
		if (ImGui::Selectable("Delete Component"))
		{
            GetTargetObject()->DeleteComponent(m_Type);
            CImGuiMgr::GetInst()->GetInspector()->RenewGameObject();
		}

		ImGui::EndPopup();
	}
}

void ComponentUI::SelectComponent(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
    // 리스트에서 더블클릭한 항목의 이름을 받아온다.
    ListUI* pListUI = (ListUI*)_ListUI;
    string SelectString = *(string*)_SelectString;

    if (SelectString == "None")
    {
        return;
    }

    GetTargetObject()->AddComponent(CComponent::GetComponentType(wstring(SelectString.begin(), SelectString.end())));
    CImGuiMgr::GetInst()->GetInspector()->RenewGameObject();
}

void ComponentUI::SelectScript(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
    // 리스트에서 더블클릭한 항목의 이름을 받아온다.
    ListUI* pListUI = (ListUI*)_ListUI;
    string SelectString = *(string*)_SelectString;
    

    if (SelectString == "None")
    {
        return;
    }

    GetTargetObject()->AddComponent((CComponent*)CScriptMgr::GetScript(wstring(SelectString.begin(), SelectString.end())));
    CImGuiMgr::GetInst()->GetInspector()->RenewGameObject();
}
