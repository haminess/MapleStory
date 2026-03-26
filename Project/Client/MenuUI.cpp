#include "pch.h"
#include "MenuUI.h"

#include <Engine/CPathMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include "CLevelSaveLoad.h"
#include "TestLevel.h"

#include "CImGuiMgr.h"
#include "Inspector.h"

MenuUI::MenuUI()
    : EditorUI("MenuUI")
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::Render()
{
    if (!IsActive())
        return;

    if (ImGui::BeginMainMenuBar())
    {
        File();

        Level();

        GameObject();

        Window();

        Editor();

        Asset();

        ImGui::EndMainMenuBar();
    }
}

void MenuUI::File()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Create New Level"))
        {
            CLevel* pNewLevel = TestLevel::CreateEmptyLevel();
            ChangeLevel(pNewLevel, LEVEL_STATE::STOP);

            // 레벨이 로드될때 Inspector 에서 보여주던 정보를 전부 제거한다. (삭제된 객체를 가리키고 있을 수 있기 때문)
            Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
            pInspector->SetTargetObject(nullptr);
        }
        if (ImGui::MenuItem("Level Save"))
        {
            wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
            strFilePath += L"Level\\Test.lv";

            CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
            CLevelSaveLoad::SaveLevel(strFilePath, CurLevel);
        }

        if (ImGui::MenuItem("Level Load"))
        {
            wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
            strFilePath += L"Level\\Test.lv";

            CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(strFilePath);
            ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

            // 레벨이 로드될때 Inspector 에서 보여주던 정보를 전부 제거한다. (삭제된 객체를 가리키고 있을 수 있기 때문)
            Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
            pInspector->SetTargetObject(nullptr);
        }

        ImGui::EndMenu();
    }
}

void MenuUI::Level()
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    LEVEL_STATE State = LEVEL_STATE::NONE;

    if (nullptr != pCurLevel)
        State = pCurLevel->GetState();

    bool IsNotPlay = State != LEVEL_STATE::PLAY;
    bool IsPlay = !IsNotPlay;
    bool IsPause = State == LEVEL_STATE::PAUSE;

    if (ImGui::BeginMenu("Level"))
    {
        if (ImGui::MenuItem("Play", nullptr, nullptr, IsNotPlay))
        {
            ChangeLevelState(LEVEL_STATE::PLAY);
        }

        if (ImGui::MenuItem("Pause", nullptr, nullptr, IsPlay))
        {
            ChangeLevelState(LEVEL_STATE::PAUSE);
        }

        if (ImGui::MenuItem("Stop", nullptr, nullptr, IsPlay || IsPause))
        {
            // 레벨이 정지될때 Inspector 에서 보여주던 정보를 전부 제거한다. (삭제된 객체를 가리키고 있을 수 있기 때문)
            Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
            pInspector->SetTargetObject(nullptr);

            wstring LevelName = CLevelMgr::GetInst()->GetCurrentLevel()->GetName();
            wstring LevelPath = CPathMgr::GetInst()->GetContentPath() + L"Level\\" + LevelName + L".lv";

            CLevel* pLevel = CLevelSaveLoad::LoadLevel(LevelPath);
            ChangeLevel(pLevel, LEVEL_STATE::STOP);
        }

        ImGui::EndMenu();
    }

}

void MenuUI::GameObject()
{
    if (ImGui::BeginMenu("GameObject"))
    {
        if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
        ImGui::Separator();
        if (ImGui::MenuItem("Cut", "CTRL+X")) {}
        if (ImGui::MenuItem("Copy", "CTRL+C")) {}
        if (ImGui::MenuItem("Paste", "CTRL+V")) {}
        ImGui::EndMenu();
    }
}

void MenuUI::Editor()
{
    if (ImGui::BeginMenu("Editor"))
    {
        if (ImGui::MenuItem("Sprite Editorr (Me)"))
        {
            CImGuiMgr::GetInst()->SetActive("SpriteEditor", true);
        }
        if (ImGui::MenuItem("Animation Editor"))
        {
            CImGuiMgr::GetInst()->SetActive("AnimationEditor", true);
        }
        if (ImGui::MenuItem("Sprite Editor (Teacher)"))
        {
            CImGuiMgr::GetInst()->SetActive("SpriteEditor_T", true);
        }
        if (ImGui::MenuItem("Map Editor"))
        {
            CImGuiMgr::GetInst()->SetActive("Map Editor", true);
        }

        ImGui::EndMenu();
    }
}

void MenuUI::Asset()
{
    if (ImGui::BeginMenu("Asset"))
    {
        if (ImGui::MenuItem("Create Empty Material", nullptr, nullptr))
        {
            Ptr<CMaterial> pMtrl = new CMaterial;
            wstring MtrlKey = GetNextMaterialName();
            pMtrl->SetRelativePath(MtrlKey);
            CAssetMgr::GetInst()->AddAsset(MtrlKey, pMtrl);
        }

        ImGui::EndMenu();
    }
}

void MenuUI::Window()
{
    if (ImGui::BeginMenu("Window"))
    {
        if (ImGui::MenuItem("Outliner"))
        {
            CImGuiMgr::GetInst()->SetActive("Outliner", true);
        }
        if (ImGui::MenuItem("Content"))
        {
            CImGuiMgr::GetInst()->SetActive("Content", true);
        }
        if (ImGui::MenuItem("Level"))
        {
            CImGuiMgr::GetInst()->SetActive("Level", true);
        }
        if (ImGui::MenuItem("Details"))
        {
            CImGuiMgr::GetInst()->SetActive("Inspector", true);
        }

        ImGui::EndMenu();
    }
}

wstring MenuUI::GetNextMaterialName()
{
    wchar_t Bufff[255] = {};

    UINT i = 0;
    while (true)
    {
        wsprintf(Bufff, L"Material\\Default Material %d.mtrl", i++);

        if (nullptr == CAssetMgr::GetInst()->FindAsset<CMaterial>(Bufff))
            break;
    }

    return Bufff;
}

