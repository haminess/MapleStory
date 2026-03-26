#include "pch.h"
#include "AssetUI.h"


AssetUI::AssetUI(const string& _ID, ASSET_TYPE _Type)
    : EditorUI(_ID)
    , m_Type(_Type)
{
    SetActive(false);
}

AssetUI::~AssetUI()
{
}

void AssetUI::AssetTitle()
{
    ImGui::PushID(0);

    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.2f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.2f, 0.6f, 0.6f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.2f, 0.6f, 0.6f));

    ImGui::Button(ASSET_TYPE_STRING[(UINT)m_Type]);

    ImGui::PopStyleColor(3);
    ImGui::PopID();
}

void AssetUI::Render_Update()
{
    AssetTitle();
}

void AssetUI::Deactivate()
{
    // 스마트 포인터들은 가리키고 있으면 객체 삭제 소멸에 문제가 발생할 수 있기 때문에
    // 반드시 생성과 소멸에 신경써주어야 한다.
    m_TargetAsset = nullptr;
}
