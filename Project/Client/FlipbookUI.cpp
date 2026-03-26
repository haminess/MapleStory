#include "pch.h"
#include "FlipbookUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CFlipbook.h>
#include <Engine/CFlipbookPlayer.h>
#include <Engine/CTransform.h>
#include <Engine/CMeshRender.h>
#include <Engine/CKeyMgr.h>

FlipbookUI::FlipbookUI()
	: AssetUI("Flipbook", ASSET_TYPE::FLIPBOOK)
    , m_NewObject(nullptr)
{
}

FlipbookUI::~FlipbookUI()
{
}

void FlipbookUI::Render_Update()
{
	AssetUI::Render_Update();

    Ptr<CFlipbook> pAsset = dynamic_cast<CFlipbook*>(GetAsset().Get());


    // 텍스쳐 배치
    if (m_NewObject)
    {
        Vector2 vPos = CKeyMgr::GetInst()->GetMouseWorldPos();
        m_NewObject->Transform()->SetWorldPos(Vector3(vPos.x, vPos.y, 0.f));

        if (KEY_RELEASED(KEY::LBTN))
            m_NewObject = nullptr;
    }
    if (ImGui::Button("Instantiate"))
    {
        // 생성
        m_NewObject = new CGameObject;
        m_NewObject->SetName(L"Anim Object");

        // 컴포넌트 추가
        m_NewObject->AddComponent(new CMeshRender);
        m_NewObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

        m_NewObject->AddComponent(new CFlipbookPlayer);
        m_NewObject->FlipbookPlayer()->AddFlipbook(0, pAsset);
        m_NewObject->FlipbookPlayer()->Play(0, 10, true);

        // 해상도 맞추기
        Vector2 vSize = m_NewObject->FlipbookPlayer()->GetCurrentSprite()->GetSlicePixel();
        m_NewObject->Transform()->SetRelativeScale(vSize.x, vSize.y, 0.f);

        CreateObject(m_NewObject, (int)LAYER_INDEX::DEFAULT, false);
    }

    if (KEY_TAP(KEY::RBTN))
    {
        Vector2 vPos = CKeyMgr::GetInst()->GetMouseWorldPos();
        CRenderMgr::GetInst()->PlayAnimEff(pAsset, nullptr, Vector3(vPos.x, vPos.y, 0.f));
    }
}
