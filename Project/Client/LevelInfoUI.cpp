#include "pch.h"
#include "LevelInfoUI.h"
#include "CLevelSaveLoad.h"

#include <Engine/CEngine.h>
#include <Engine/CPathMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

LevelInfoUI::LevelInfoUI()
	: EditorUI("LevelInfoUI")
{

}

LevelInfoUI::~LevelInfoUI()
{
}

void LevelInfoUI::Render_Update()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (!pLevel)
	{
		ImGui::Text("No Level");
		return;
	}

	ImGui::Text("Level Name");
	ImGui::SameLine(120);
	// name
	wstring wname = pLevel->GetName();
	string name = string(wname.begin(), wname.end());

	char buffer[256] = {};
	strcpy_s(buffer, name.c_str());

	if (ImGui::InputText("##LevelName", buffer, sizeof(buffer)))
	{
		name = buffer;
		wname = wstring(name.begin(), name.end());
		pLevel->SetName(wname);
	}

	if (ImGui::Button("Save"))
	{
		wstring name = pLevel->GetName();
		wstring path = CPathMgr::GetInst()->GetContentPath() + L"Level\\" + name + L".lv";
		bool IsExist = exists(path);
		if (IsExist)
		{
			if (MessageBox(CEngine::GetInst()->GetMainWnd(), L"이미 존재하는 레벨입니다. 덮어쓰시겠습니까?", L"알림", MB_YESNO) == IDNO)
				return;
		}

		CLevelSaveLoad::SaveLevel(path, pLevel);
	}
}
