#include "pch.h"
#include "FontUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CFont.h>
#include <Engine/CMeshRender.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTransform.h>

FontUI::FontUI()
    : AssetUI("Font", ASSET_TYPE::FONT)
{
}

FontUI::~FontUI()
{
}

void FontUI::Render_Update()
{
    AssetTitle();

    Ptr<CFont> pAsset = dynamic_cast<CFont*>(GetAsset().Get());
    assert(pAsset.Get());



    ImGui::Text("Name");
    ImGui::SameLine(100);

    string strKey = string(pAsset->GetKey().begin(), pAsset->GetKey().end());

    ImGui::InputText("##FontName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_ReadOnly);
}