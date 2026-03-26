#include "pch.h"
#include "AnimEditorUI.h"
#include "func_ui.h"
#include "ListUI.h"
#include "CImGuiMgr.h"
#include "TreeUI.h"
#include <Engine/CTexture.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CPathMgr.h>
#include <Engine/CSprite.h>
#include <Engine/CTexture.h>
#include <Engine/CFlipbook.h>
#include <Engine/CFlipbookPlayer.h>
#include <Engine/CEngine.h>


AnimEditorUI::AnimEditorUI()
	: EditorUI("AnimationEditor")
	, m_ScaleFactor(100.f)
	, m_PreviewPlayer(nullptr)
	, m_IsNew(false)
{
	m_PreviewPlayer = new CFlipbookPlayer;
	m_CurAnim = CAssetMgr::GetInst()->FindAsset<CFlipbook>(L"LinkWalkDownAnim");
	m_PreviewPlayer->AddFlipbook(0, m_CurAnim);
	m_PreviewPlayer->Play(0, 10, true);
}

AnimEditorUI::~AnimEditorUI()
{
	DELETE(m_PreviewPlayer);
}

void AnimEditorUI::Render()
{
	m_PreviewPlayer->FinalTick();

	//ImGui::SetNextWindowPos(ImVec2(100.f, 100.f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(1400, 600), ImGuiCond_FirstUseEver);
	EditorUI::Render();
}


void AnimEditorUI::Render_Update()
{
	// 좌우 분할 레이아웃
	ImGui::Columns(2);
	ImVec2 vecWinSize = ImGui::GetWindowSize();


	ImGui::SetColumnWidth(0, vecWinSize.x * 0.6f); // 왼쪽 70%

	Preview();

	ImGui::NextColumn();

	Detail_Anim();
	Detail_Frame();

}

void AnimEditorUI::Preview()
{
	// 왼쪽 영역 (텍스처 뷰어)
	{
		ImGui::BeginChild("Texture Viewer", ImVec2(0, 0), true,
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		ImVec2 winMin = ImGui::GetWindowPos();
		ImVec2 winMax = ImVec2(winMin.x + ImGui::GetWindowSize().x, winMin.y + ImGui::GetWindowSize().y);

		// 화면 확대
		if (ImGui::IsMouseHoveringRect(winMin, winMax))
		{
			float wheelDelta = ImGui::GetIO().MouseWheel;
			if (wheelDelta > 0.f)
			{
				m_ScaleFactor += DT * 10000.f;
			}
			if (wheelDelta < 0.f)
			{
				m_ScaleFactor -= DT * 10000.f;
			}
		}

		// 화면 이동
		if (KEY_PRESSED(KEY::RBTN))
		{
			m_PreviewPos += CKeyMgr::GetInst()->GetMouseDir();
		}


		// 애니메이션 출력
		Ptr<CSprite> pCurSprite = m_PreviewPlayer->GetCurrentSprite();
		ImTextureID tex = nullptr;
		if (nullptr != pCurSprite)
		{
			tex = pCurSprite->GetAtlasTexture()->GetSRV().Get();

			Vector2 uvLeftTop = pCurSprite->GetLeftTop();
			Vector2 uvSlice = pCurSprite->GetSlice();
			ImVec2 uv_min = ImVec2(uvLeftTop.x, uvLeftTop.y);
			ImVec2 uv_max = ImVec2(uvLeftTop.x + uvSlice.x, uvLeftTop.y + uvSlice.y);
			float aspect_ratio = 1.f;
			ImGui::SetCursorPos(ImVec2(m_PreviewPos.x, m_PreviewPos.y));

			// 윈도우상 아틀라스 시작 위치 구하기
			Vector2 vAtlasPos = Vector2(winMin.x + m_PreviewPos.x, winMin.y + m_PreviewPos.y);
			// 윈도우상 아틀라스 크기 구하기
			Vector2 vAtlasScale = Vector2(m_ScaleFactor, m_ScaleFactor / aspect_ratio);
			// 마우스 좌표 가져오기
			Vector2 mousePos = Vector2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);

			ImGui::Image(tex, ImVec2(vAtlasScale.x, vAtlasScale.y), uv_min, uv_max, ImVec4(1.f, 1.f, 1.f, 1.f), ImGui::GetStyleColorVec4(ImGuiCol_Border));
		}


		// 애니메이션 재생 버튼
		float buttonWidth = 100.f;
		float buttonHeight = 30.f;
		float buttonSpacing = 10.f;  // 버튼 사이의 간격

		// 윈도우 중앙 계산
		float windowWidth = ImGui::GetWindowSize().x;
		float windowHeight = ImGui::GetWindowSize().y;
		float totalButtonWidth = (buttonWidth * 2) + buttonSpacing;

		// 버튼의 Y 위치 (하단에서 약간 위)
		float buttonsY = windowHeight - buttonHeight - 20.f;

		// Play 버튼 위치 설정
		ImGui::SetCursorPos(ImVec2(
			(windowWidth - totalButtonWidth) * 0.5f,
			buttonsY
		));
		if (ImGui::Button("Play", ImVec2(buttonWidth, buttonHeight))) {
			// Play 버튼 클릭 시 동작
			m_PreviewPlayer->Play(0, 5, true);
		}

		// Stop 버튼 위치 설정
		ImGui::SetCursorPos(ImVec2(
			(windowWidth - totalButtonWidth) * 0.5f + buttonWidth + buttonSpacing,
			buttonsY
		));
		if (ImGui::Button("Stop", ImVec2(buttonWidth, buttonHeight))) {
			// Stop 버튼 클릭 시 동작
			m_PreviewPlayer->Pause();
		}


		ImGui::EndChild();
	}

	// 드래그받은 텍스쳐, 스프라이트 프레임 추가
	if (ImGui::BeginDragDropTarget())
	{
		if (ImGui::AcceptDragDropPayload("ContentUI"))   // 키값이 contentUi인 PayLoad를 받았을때
		{
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
			TreeNode* pNode = *((TreeNode**)pPayload->Data);
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (pAsset->GetAssetType() == ASSET_TYPE::TEXTURE)
			{
				Ptr<CSprite> pSprite = ((CTexture*)pAsset.Get())->GetOriginSprite();
				m_CurAnim->AddSprite(pSprite);
				ImGui::SetWindowFocus(nullptr);
			}
			if (pAsset->GetAssetType() == ASSET_TYPE::SPRITE)
			{
				Ptr<CSprite> pSprite = (CSprite*)pAsset.Get();
				m_CurAnim->AddSprite(pSprite);
				ImGui::SetWindowFocus(nullptr);
			}
		}
		// MultiSelected Payload
		if (ImGui::AcceptDragDropPayload("ContentUI_Multi"))
		{
			//PayLoad로 Vector를 등록해서 가져오는 방식
			const ImGuiPayload* pPayLoad = ImGui::GetDragDropPayload();

			//// PayLoad에서 노드 벡터 받아와서 형변환 (사이즈는 같은데, 벡터 안의 데이터를 읽어들일 수 없음) > 해결
			vector<TreeNode*>* pVecNode = *((vector<TreeNode*>**)(pPayLoad->Data));
			for (int i = 0; i < pVecNode->size(); ++i)
			{
				TreeNode* pNode = pVecNode->at(i);  // 노드를 벡터 순서대로 받아와서 처리

				Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
				if (pAsset->GetAssetType() == ASSET_TYPE::TEXTURE)
				{
					Ptr<CSprite> pSprite = ((CTexture*)pAsset.Get())->GetOriginSprite();
					m_CurAnim->AddSprite(pSprite);
					ImGui::SetWindowFocus(nullptr);
				}
				if (pAsset->GetAssetType() == ASSET_TYPE::SPRITE)
				{
					Ptr<CSprite> pSprite = (CSprite*)pAsset.Get();
					m_CurAnim->AddSprite(pSprite);
					ImGui::SetWindowFocus(nullptr);
				}
			}
			ImGui::SetWindowFocus(nullptr);   // UI 포커스 날리기
		}
		ImGui::EndDragDropTarget();
	}


}

void AnimEditorUI::Detail_Anim()
{
	const char* pivots[] = { "Center", "Left" , "Right" , "Top" , "TopLeft" , "TopRight" , "Bottom" , "BottomLeft" , "BottomRight", "Custom" };

	SHOW_EDITOR_DETAIL(Animation);
	ImGui::BeginChild("Anim Info", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY);
	ImVec2 backSize = ImGui::GetWindowSize();
	int itemWidth = (backSize.x - 150) / 2;

	if (nullptr != m_CurAnim)
	{
		// name
		wstring wname = m_CurAnim->GetName();
		string name = string(wname.begin(), wname.end());

		char buffer[256] = {};
		strcpy_s(buffer, name.c_str());

		if (ImGui::InputText("##AnimName", buffer, sizeof(buffer)))
		{
			name = buffer;
			wname = wstring(name.begin(), name.end());
			m_CurAnim->SetName(wname);
		}

		// frame count
		string animInfo = "Frame Count : " + std::to_string(m_CurAnim->GetMaxSprite());
		ImGui::Text(animInfo.c_str());
		animInfo = "Duration : " + std::to_string(m_CurAnim->GetMaxSprite() * m_PreviewPlayer->GetFPS()) + " sec";
		ImGui::Text(animInfo.c_str());

		ImGui::Text("");

		bool bLoop = m_CurAnim->GetLoop();
		if (DrawCheckUI("Loop", &bLoop))
		{
			m_CurAnim->SetLoop(bLoop);
		}

		// 폴더로 스프라이트 추가
		static string folderPath = "";
		DrawInputTextUI("Add Folder", folderPath);

		ImGui::SameLine();
		if (ImGui::Button("##MeshBtn", ImVec2(18.f, 18.f)))
		{
			AddSpriteInFolder(wstring(folderPath.begin(), folderPath.end()));
		}

		// button
		float windowWidth = ImGui::GetContentRegionAvail().x;
		float buttonWidth = (windowWidth - ImGui::GetStyle().ItemSpacing.x * 2) / 3.f;
		if (ImGui::Button("Create##Sprite", ImVec2(buttonWidth, 25)))
		{
			CreateFlipbook();
		}

		ImGui::SameLine();
		if (ImGui::Button("Load", ImVec2(buttonWidth, 25)))
		{
			LoadFlipbook();
		}

		ImGui::SameLine();
		if (ImGui::Button("Save", ImVec2(buttonWidth, 25)))
		{
			Save();
		}


		ImGui::Separator();

		ImGui::EndChild();

	}
}

void AnimEditorUI::Detail_Frame()
{
	// === Frame List ===
	SHOW_EDITOR_DETAIL(Frame);
	ImGui::BeginChild("Frame Edit", ImVec2(0, 0), ImGuiChildFlags_AutoResizeY);
	wstring curFrameName;

	Ptr<CSprite> pCurrentSprite = m_PreviewPlayer->GetCurrentSprite();

	if (ImGui::BeginListBox("##FrameList_animeditor", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (int i = 0; i < m_CurAnim->GetMaxSprite(); i++)
		{
			CSprite* pListSprite = m_CurAnim->GetSprite(i).Get();

			wstring spriteName = L"None";
			if (pListSprite != nullptr)
			{
				spriteName = pListSprite->GetName();
				if (spriteName == L"")
					spriteName = pListSprite->GetKey();

			}

			string items = std::to_string(i) + " : " + string(spriteName.begin(), spriteName.end()) + "##animeditor";

			bool is_selected = false;
			if (m_PreviewPlayer->IsPlaying())
			{
				is_selected = (m_PreviewPlayer->GetCurrentSprite() == pListSprite);
				if (ImGui::Selectable(items.c_str(), is_selected, 0))
				{
					m_PreviewPlayer->SetCurSpriteIdx(i);
					m_PreviewPlayer->Pause();
				}

				if (is_selected)
				{
					m_CurSpriteIdx = i;
					ImGui::SetItemDefaultFocus();
					curFrameName = spriteName;
				}
			}
			else
			{
				auto& selected = GetSelected();
				is_selected = find(selected.begin(), selected.end(), pListSprite) != selected.end();
				if (ImGui::Selectable(items.c_str(), is_selected, 0))
				{
					if (!(KEY_PRESSED(KEY::CTRL)))
					{
						SelectClear();
					}

					if (is_selected)
						DeselectSprite(pListSprite);
					else
						SelectSprite(pListSprite);
				}


			}


		}
		ImGui::EndListBox();

		// === Current Frame Info ===
		// Frame Edit Button
		ImGui::Text("");
		ImGui::SameLine(ImGui::GetColumnWidth() - 70 * 2);
		if (ImGui::Button("Add", ImVec2(60, 0)))
		{
			Ptr<CSprite> pSprite = nullptr;
			// 기존 애니메이션일 경우
			if (m_CurAnim->GetMaxSprite() > 0)
			{
				pSprite = m_CurAnim->GetSprite(m_CurAnim->GetMaxSprite() - 1);
			}
			// 새로운 애니메이션
			else
			{
				m_CurSpriteIdx = 0;
				m_CurAnim->AddSprite(nullptr);
				int a = m_CurAnim->GetMaxSprite();
				LoadSprite();
			}

			m_CurAnim->AddSprite(pSprite);
		}

		ImGui::SameLine(ImGui::GetColumnWidth() - 70);
		if (ImGui::Button("Del", ImVec2(60, 0)) || KEY_TAP(KEY::DEL))
		{
			vector<Ptr<CSprite>>& vecSprites = m_CurAnim->GetSprites();
			if (vecSprites.size() > m_CurSpriteIdx)
				vecSprites.erase(vecSprites.begin() + m_CurSpriteIdx);
		}
	}


	// === Sprite Info ===
	ImGui::Text("");
	ImGui::Text("");
	ImGui::Text("Sprite Info");

	// Name
	if (curFrameName == L"")
		curFrameName = L"None";
	string strCurFrameName = string(curFrameName.begin(), curFrameName.end());
	DrawTextUI("Sprite Name", strCurFrameName);

	// Detail
	auto selected = GetSelected();
	if (!m_PreviewPlayer->IsPlaying() && selected.size() > 0)
	{
		ImGui::Spacing();

		if (selected.size() > 0)
		{

			CSprite* sprite = *selected.begin();

			// Left Top
			ImGui::Text("Left Top");
			ImGui::SameLine(100);
			Vector2 leftTop = sprite->GetLeftTopPixel();
			float leftTopArr[2] = { leftTop.x, leftTop.y };
			if (ImGui::DragFloat2("##LeftTop", leftTopArr))
			{
				for (auto s : selected)
					s->SetLeftTop(Vector2(leftTopArr[0], leftTopArr[1]));
			}

			// Slice Size
			ImGui::Text("Slice Size");
			ImGui::SameLine(100);
			Vector2 slice = sprite->GetSlicePixel();
			float sliceArr[2] = { slice.x, slice.y };
			if (ImGui::DragFloat2("##Slice", sliceArr))
			{
				for (auto s : selected)
					s->SetSlice(Vector2(sliceArr[0], sliceArr[1]));
			}

			// Background
			ImGui::Text("Background");
			ImGui::SameLine(100);
			Vector2 background = sprite->GetBackgroundPixel();
			float backgroundArr[2] = { background.x, background.y };
			if (ImGui::DragFloat2("##Background", backgroundArr))
			{
				for (auto s : selected)
					s->SetBackground(Vector2(backgroundArr[0], backgroundArr[1]));
			}

			// Offset
			ImGui::Text("Offset");
			ImGui::SameLine(100);
			Vector2 offset = sprite->GetOffsetPixel();
			float offsetArr[2] = { offset.x, offset.y };
			if (ImGui::DragFloat2("##Offset", offsetArr))
			{
				for (auto s : selected)
					s->SetOffset(Vector2(offsetArr[0], offsetArr[1]));
			}

			// Sprite Edit Button
			ImGui::Text("");
			ImGui::SameLine(ImGui::GetColumnWidth() - 70 * 2);
			if (ImGui::Button("Load", ImVec2(60, 0)))
			{
				LoadSprite();
			}

			ImGui::SameLine(ImGui::GetColumnWidth() - 70);
			if (ImGui::Button("Save", ImVec2(60, 0)))
			{
				for (auto& sprite : m_CurAnim->GetSprites())
					sprite->Save(CPathMgr::GetInst()->GetContentPath() + sprite->GetRelativePath());
			}
		}
	}

	ImGui::EndChild();
}

void AnimEditorUI::SetCurAnim(Ptr<CFlipbook> _Anim)
{
	m_CurAnim = _Anim;
	m_PreviewPlayer->AddFlipbook(0, m_CurAnim);
	m_PreviewPlayer->Play(0, 0.2f, true);
}

void AnimEditorUI::CreateFlipbook()
{
	m_IsNew = true;

	m_CurAnim = new CFlipbook;
	m_CurAnim->SetName(L"New Animation");

	m_PreviewPlayer->Pause();
	m_PreviewPlayer->AddFlipbook(0, m_CurAnim);
}

void AnimEditorUI::LoadFlipbook()
{
	// ListUI 를 활성화 시키기
	ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
	pListUI->SetName("Flipbook");
	pListUI->SetActive(true);

	// ListUI 에 넣어줄 문자열 정보 가져오기
	pListUI->AddItem("None");

	vector<wstring> vecAssetNames;
	CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::FLIPBOOK, vecAssetNames);
	pListUI->AddItem(vecAssetNames);

	// 더블 클릭 시 호출시킬 함수 등록
	pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&AnimEditorUI::LoadFlipbook);
}

void AnimEditorUI::LoadSprite()
{
	// ListUI 를 활성화 시키기
	ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
	pListUI->SetName("Sprite");
	pListUI->SetActive(true);

	// ListUI 에 넣어줄 문자열 정보 가져오기
	pListUI->AddItem("None");

	vector<wstring> vecAssetNames;
	CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::SPRITE, vecAssetNames);
	pListUI->AddItem(vecAssetNames);

	// 더블 클릭 시 호출시킬 함수 등록
	pListUI->AddDynamicDoubleClicked(this, (EUI_DELEGATE_2)&AnimEditorUI::SelectSprite);
}

void AnimEditorUI::LoadFlipbook(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	m_IsNew = false;

	// 리스트에서 더블클릭한 항목의 이름을 받아온다.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
	{
		m_CurAnim = nullptr;
		return;
	}

	// 해당 항목 에셋을 찾아서, MeshRenderComponent 가 해당 메시를 참조하게 한다.
	m_CurAnim = CAssetMgr::GetInst()->FindAsset<CFlipbook>(wstring(pStr->begin(), pStr->end()));
	m_PreviewPlayer->AddFlipbook(0, m_CurAnim);
	m_PreviewPlayer->Pause();
}

void AnimEditorUI::SelectSprite(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// 리스트에서 더블클릭한 항목의 이름을 받아온다.
	ListUI* pListUI = (ListUI*)_ListUI;
	string str = *(string*)_SelectString;
	wstring wstr = wstring(str.begin(), str.end());

	if (str == "None")
	{
		m_CurAnim = nullptr;
		return;
	}

	// 해당 항목 에셋을 찾아서, MeshRenderComponent 가 해당 메시를 참조하게 한다.
	Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(wstr);
	m_CurAnim->GetSprite(m_CurSpriteIdx) = pSprite;
}

void AnimEditorUI::Save()
{
	if (nullptr == m_CurAnim)
		return;

	wstring name = m_CurAnim->GetName();

	wstring key;
	key = L"Flipbook\\" + name + L".flip";


	if (nullptr != CAssetMgr::GetInst()->FindAsset<CFlipbook>(key))
	{
		if (!MessageBox(CEngine::GetInst()->GetMainWnd(),
			L"애니메이션이 이미 존재합니다.\n 덮어쓰시겠습니까?", L"알림", MB_YESNO))
			return;
	}

	m_CurAnim->Save(CPathMgr::GetInst()->GetContentPath() + key);
}


void AnimEditorUI::AddSpriteInFolder(const wstring& _FolderPath)
{
	// 폴더 파일리스트 불러오기
	wstring folderPath = CPathMgr::GetInst()->GetContentPath() + _FolderPath;

	std::vector<std::wstring> result;
	for (const auto& entry : directory_iterator(folderPath))
	{
		if (entry.is_regular_file())
		{
			result.push_back(entry.path().filename().wstring());

		}
	}

	// 숫자순 정렬
	sort(result.begin(), result.end(),
		[](const wstring& a, const wstring& b)
		{
			return GetFirstNumberBeforeUnderscore(a) < GetFirstNumberBeforeUnderscore(b);
		});

	// 폴더 내 이미지를 애니메이션에 추가
	for (const wstring& fileName : result)
	{
		Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(_FolderPath + L"\\" + fileName);

		if (pTex == nullptr)
		{
			MessageBox(CEngine::GetInst()->GetMainWnd(), L"주소가 유효하지 않습니다.", L"알림", MB_OK);
			return;
		}

		Ptr<CSprite> pSprite = pTex->GetOriginSprite();
		m_CurAnim->AddSprite(pSprite);
		ImGui::SetWindowFocus(nullptr);
	}

	wstring fileName = CPathMgr::GetInst()->GetNameFromPath(_FolderPath, false);
	m_CurAnim->SetName(fileName);
}
