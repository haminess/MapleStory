#include "pch.h"
#include "TextureUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTexture.h>
#include <Engine/CMeshRender.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTransform.h>

TextureUI::TextureUI()
    : AssetUI("Texture", ASSET_TYPE::TEXTURE)
    , m_NewObject(nullptr)
{
}

TextureUI::~TextureUI()
{
}

void TextureUI::Render_Update()
{
    AssetTitle();

    Ptr<CTexture> pAsset = dynamic_cast<CTexture*>(GetAsset().Get());
    assert(pAsset.Get());



    ImGui::Text("Name");
    ImGui::SameLine(100);

    string strKey = string(pAsset->GetKey().begin(), pAsset->GetKey().end());

    ImGui::InputText("##TexName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_ReadOnly);

    int width = pAsset->GetWidth();
    int height = pAsset->GetHeight();

    ImGui::Text("Width");
    ImGui::SameLine(100);
    ImGui::InputInt("##Width", &width, 0.f, 0.f, ImGuiInputTextFlags_ReadOnly);

    ImGui::Text("Height");
    ImGui::SameLine(100);
    ImGui::InputInt("##Height", &height, 0.f, 0.f, ImGuiInputTextFlags_ReadOnly);

    // 이미지 프리뷰
    ImVec2 uv_min = ImVec2(0.0f, 0.0f);
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);
    ImVec4 tint_col = ImVec4(1.f, 1.f, 1.f, 1.f);
    ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
    ImGui::Image(pAsset->GetSRV().Get(), ImVec2(300.f, 300.f * height / width), uv_min, uv_max, tint_col, border_col);


    // 텍스쳐 배치
    if (m_NewObject)
    {
        Vector2 vPos = CKeyMgr::GetInst()->GetMouseWorldPos();
        m_NewObject->Transform()->SetWorldPos(Vector3(vPos.x, vPos.y, 0.f));

        if(KEY_RELEASED(KEY::LBTN))
            m_NewObject = nullptr;
    }

    if (ImGui::Button("Instantiate"))
    {
        m_NewObject = new CGameObject;
		m_NewObject->SetName(L"Texture Object");

        m_NewObject->AddComponent(new CMeshRender);
		m_NewObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
        m_NewObject->MeshRender()->CreateDynamicMaterial();

		m_NewObject->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pAsset);

        Vector2 vSize = pAsset->GetResolution();
        m_NewObject->Transform()->SetRelativeScale(vSize.x, vSize.y, 0.f);

        CreateObject(m_NewObject, (int)LAYER_INDEX::DEFAULT, false);
    }

}