#include "pch.h"
#include "LevelUI.h"
#include "TestLevel.h"
#include "CLevelSaveLoad.h"
#include "CImGuiMgr.h"
#include "TreeUI.h"
#include "Inspector.h"

#include <Engine/CEngine.h>
#include <Engine/CPathMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>


LevelUI::LevelUI()
	: EditorUI("Level")
{
	m_LevelTree = (TreeUI*)AddChildUI(new TreeUI);
	m_LevelTree->SetName("LevelUI");
	m_LevelTree->ShowRoot(false);
	m_LevelTree->AddDynamicSelect(this, (EUI_DELEGATE_1)&LevelUI::SelectLevel);
	m_LevelTree->SetChildSize(ImVec2(0.f, -1.f));

}

LevelUI::~LevelUI()
{
}

void LevelUI::Context(TreeNode* _TreeNode)
{
	// 전체 레벨
	if (ImGui::Selectable("Create New Level"))
	{
		CLevel* pNewLevel = TestLevel::CreateEmptyLevel();
		pNewLevel->SetName(L"New Level");
		ChangeLevel(pNewLevel, LEVEL_STATE::STOP);

		Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
		pInspector->SetTargetObject(nullptr);
	}
	if (ImGui::Selectable("Save All Levels"))
	{
		SaveAllLevels();
	}
	if (ImGui::Selectable("Save Current Level"))
	{
		CLevel* CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

		wstring name = CurLevel->GetName();
		wstring path = CPathMgr::GetInst()->GetContentPath() + L"Level\\" + name + L".lv";
		bool IsExist = exists(path);
		if (IsExist)
		{
			if (MessageBox(CEngine::GetInst()->GetMainWnd(), L"이미 존재하는 레벨입니다. 덮어쓰시겠습니까?", L"알림", MB_YESNO) == IDNO)
				return;
		}

		CLevelSaveLoad::SaveLevel(path, CurLevel);
	}


	// 해당 레벨
	ImGui::Separator();
	if (ImGui::Selectable("Open"))
	{
		TreeNode* pNode = (TreeNode*)_TreeNode;
		string name = pNode->GetName();

		wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
		strFilePath += L"Level\\" + wstring(name.begin(), name.end()) + L".lv";

		wstring msg = wstring(name.begin(), name.end()) + L" 레벨을 로드하겠냐능?";
		if (MessageBox(CEngine::GetInst()->GetMainWnd(), msg.c_str(), L"알림", MB_YESNO) == IDYES)
		{
			CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(strFilePath);

			if (pLoadedLevel)
				ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);
			else
				MessageBox(CEngine::GetInst()->GetMainWnd(), L"파일이 존재하지 않습니다.", L"알림", MB_OK);

			// 레벨이 로드될때 Inspector 에서 보여주던 정보를 전부 제거한다. (삭제된 객체를 가리키고 있을 수 있기 때문)
			Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
			pInspector->SetTargetObject(nullptr);
		}

	}
}

void LevelUI::RenewLevel()
{
	// 레벨 노드 추가
	m_LevelTree->Clear();
	TreeNode* pRootNode = m_LevelTree->AddItem(nullptr, "RootNode", 0);
	TreeNode* pLevelRoot = m_LevelTree->AddItem(pRootNode, "LEVEL", 0, true);
	for (int i = 0; i < m_vecLevelPath.size(); ++i)
	{
		wstring wname = CPathMgr::GetInst()->GetNameFromPath(m_vecLevelPath[i]);
		string name = string(wname.begin(), wname.end());
		m_LevelTree->AddItem(pLevelRoot, name, 0, false);
	}

}

void LevelUI::SelectLevel(DWORD_PTR _TreeNode)
{
	TreeNode* pNode = (TreeNode*)_TreeNode;

	Inspector* pInspector = (Inspector*)CImGuiMgr::GetInst()->FindUI("Inspector");
	string name = pNode->GetName();
	pInspector->SetTargetLevel();
}

void LevelUI::SaveAllLevels()
{
	if (m_vecLevelPath.empty())
	{
		MessageBox(CEngine::GetInst()->GetMainWnd(), L"저장할 레벨 파일이 없습니다.", L"알림", MB_OK);
		return;
	}

	// 전체 덮어쓰기 여부를 한 번만 물어보기
	if (MessageBox(CEngine::GetInst()->GetMainWnd(),
		L"Level 폴더의 모든 레벨(.lv)을 로드한 뒤 다시 저장(덮어쓰기)합니다.\n계속할까요?",
		L"Resave All Levels",
		MB_YESNO) == IDNO)
	{
		return;
	}

	int successCount = 0;
	int failCount = 0;

	wstring failList;

	for (const wstring& filePath : m_vecLevelPath)
	{
		wstring fullPath = CPathMgr::GetInst()->GetContentPath() + filePath;

		// 파일 존재 확인
		if (!exists(fullPath))
		{
			++failCount;
			failList += L"- (Missing) " + fullPath + L"\n";
			continue;
		}

		// 로드
		CLevel* pLoaded = CLevelSaveLoad::LoadLevel(fullPath);
		if (!pLoaded)
		{
			++failCount;
			failList += L"- (Load Fail) " + filePath + L"\n";
			continue;
		}

		// 저장 (원본 경로에 그대로 저장 = 덮어쓰기)
		CLevelSaveLoad::SaveLevel(fullPath, pLoaded);
		++successCount;

		// ⚠️ 중요: LoadLevel이 new로 만든 레벨이면 여기서 해제 필요.
		// 프로젝트에 SAFE_DELETE 매크로가 있으면 그걸 쓰세요.
		if (pLoaded)
		{
			delete pLoaded;
			pLoaded = nullptr;
		}
	}

	wstring result = L"Resave 완료\n\n성공: " + std::to_wstring(successCount) + L"\n실패: " + std::to_wstring(failCount);
	if (failCount > 0)
	{
		result += L"\n\n[실패 목록]\n" + failList;
	}

	MessageBox(CEngine::GetInst()->GetMainWnd(), result.c_str(), L"알림", MB_OK);
}