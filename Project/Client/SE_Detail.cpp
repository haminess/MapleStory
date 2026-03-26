#include "pch.h"
#include "SE_Detail.h"

#include "SE_AtlasView.h"
#include "TreeUI.h"
#include "ListUI.h"
#include "CImGuiMgr.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CFlipbook.h>

SE_Detail::SE_Detail()
	: SE_Sub("SE_Detail")
{
}

SE_Detail::~SE_Detail()
{
	if (m_SliceThread.joinable())
	{
		m_SliceThread.join();
	}
}


void SE_Detail::Render_Update()
{
	if (GetOwner()->GetAtlas() == nullptr)
	{
		ImGui::Text("No Texture Selected");
		return;
	}


	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 120); // 기본 입력 필드 너비 설정

	Atlas();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	Sprites();


	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();


	ImGui::PopItemWidth();

	Slice();

}



void SE_Detail::Atlas()
{
	SHOW_EDITOR_DETAIL(Atlas);

	Ptr<CTexture> pAtlas = GetOwner()->GetAtlas();

	// Texture Name
	path key = string(pAtlas->GetKey().begin(), pAtlas->GetKey().end());
	string texName = key.filename().string();
	ImGui::Text("Texture Name");
	ImGui::SameLine(120);
	ImGui::InputText("##TexName", (char*)texName.c_str(), texName.length(), ImGuiInputTextFlags_ReadOnly);
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentUI");
		if (payload)
		{
			const ImGuiPayload* pPayload = ImGui::GetDragDropPayload();
			TreeNode* pNode = *((TreeNode**)pPayload->Data);
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();

			if (pAsset->GetAssetType() == ASSET_TYPE::TEXTURE)
			{
				GetOwner()->Load((Ptr<CTexture>)(CTexture*)pNode->GetData());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::Spacing();

	// === 해상도 ===
	ImGui::Text("Resolution");
	ImGui::SameLine(120);

	int width = (int)pAtlas->GetWidth();
	int height = (int)pAtlas->GetHeight();
	int resol[2] = { width, height };
	ImGui::InputInt2("##Width", resol, ImGuiInputTextFlags_ReadOnly);


	float windowWidth = ImGui::GetContentRegionAvail().x;
	float buttonWidth = (windowWidth - ImGui::GetStyle().ItemSpacing.x * 2) / 2.f;
	// === 텍스처 로드 버튼 ===
	if (ImGui::Button("Load Sprite", ImVec2(buttonWidth, 25))) // -1은 available width
	{
		GetOwner()->LoadFromDirectory();
	}
	ImGui::SameLine();
	if (ImGui::Button("Save Sprite", ImVec2(buttonWidth, 25)))
	{
		GetOwner()->Save();
	}
}

void SE_Detail::Sprites()
{
	// === 스프라이트 리스트 ===
	SHOW_EDITOR_DETAIL(Sprite);

	Ptr<CTexture> pAtlas = GetOwner()->GetAtlas();
	path key = string(pAtlas->GetKey().begin(), pAtlas->GetKey().end());
	string texName = key.filename().string();

	list<CSprite>& sprites = GetOwner()->GetSprites();


	if (ImGui::BeginListBox("##SpriteList", ImVec2(-1, 120))) // -1은 available width
	{
		if (sprites.empty())
		{
			ImGui::Selectable("No sprites available", false);
		}
		else
		{
			auto& selected = GetOwner()->GetSelected();
			auto iter = sprites.begin();
			for (size_t i = 0; i < sprites.size(); i++)
			{
				string spriteName = texName + "_" + std::to_string(i);
				bool is_selected = find(selected.begin(), selected.end(), &*iter) != selected.end();

				if (ImGui::Selectable(spriteName.c_str(), is_selected))
				{
					if (!(KEY_PRESSED(KEY::CTRL)))
					{
						GetOwner()->SelectClear();
					}

					if (is_selected)
						GetOwner()->DeselectSprite(&(*iter));
					else
						GetOwner()->SelectSprite(&(*iter));

				}

				++iter;
			}
		}

		ImGui::EndListBox();
	}

	// === 스프라이트 정보 출력 ===
	auto selected = GetOwner()->GetSelected();
	ImGui::Spacing();

	if (selected.size() > 0)
	{
		ImGui::Text("Sprite Info");
		ImGui::Spacing();

		CSprite* sprite = *selected.begin();

		// Left Top
		ImGui::Text("Left Top");
		ImGui::SameLine(120);
		Vector2 leftTop = sprite->GetLeftTopPixel();
		float leftTopArr[2] = { leftTop.x, leftTop.y };
		if (ImGui::DragFloat2("##LeftTop", leftTopArr))
		{
			for (auto s : selected)
				s->SetLeftTop(Vector2(leftTopArr[0], leftTopArr[1]));
		}

		// Slice Size
		ImGui::Text("Slice Size");
		ImGui::SameLine(120);
		Vector2 slice = sprite->GetSlicePixel();
		float sliceArr[2] = { slice.x, slice.y };
		if (ImGui::DragFloat2("##Slice", sliceArr))
		{
			for (auto s : selected)
				s->SetSlice(Vector2(sliceArr[0], sliceArr[1]));
		}

		// Background
		ImGui::Text("Background");
		ImGui::SameLine(120);
		Vector2 background = sprite->GetBackgroundPixel();
		float backgroundArr[2] = { background.x, background.y };
		if (ImGui::DragFloat2("##Background", backgroundArr))
		{
			for (auto s : selected)
				s->SetBackground(Vector2(backgroundArr[0], backgroundArr[1]));
		}

		// Offset
		ImGui::Text("Offset");
		ImGui::SameLine(120);
		Vector2 offset = sprite->GetOffsetPixel();
		float offsetArr[2] = { offset.x, offset.y };
		if (ImGui::DragFloat2("##Offset", offsetArr))
		{
			for (auto s : selected)
				s->SetOffset(Vector2(offsetArr[0], offsetArr[1]));
		}
	}

	// === 저장 기능 ===
	float windowWidth = ImGui::GetContentRegionAvail().x;
	float buttonWidth = (windowWidth - ImGui::GetStyle().ItemSpacing.x * 2) / 3.f;

	if (ImGui::Button("Create##Sprite", ImVec2(buttonWidth, 25)))
	{
		//  기능
		GetOwner()->CreateSprite({}, {});
	}
	ImGui::SameLine();
	if (ImGui::Button("Delete##Sprite", ImVec2(buttonWidth, 25)))
	{
		if (selected.size() > 0)
		{
			for (auto iter : selected)
			{
				GetOwner()->DeleteSprite(iter);
			}
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear##Sprite", ImVec2(buttonWidth, 25)))
	{
		// Clear 기능
		GetOwner()->ClearAllSprite();
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	// === 애니메이션 생성 ===
	SHOW_EDITOR_DETAIL(Animation);
	if (selected.size() > 0)
	{
		DrawTitleUI("Animation");
		static bool bLoop = false;
		DrawCheckUI("Loop", &bLoop);
		if (ImGui::Button("Create Animation", ImVec2(-1, 25))) // -1은 available width
		{
			GetOwner()->Save();
			GetOwner()->CreateFlipbook(bLoop);
		}
	}
	else
	{
		ImGui::Text("No Sprite Selected");
	}
}

void SE_Detail::Slice()
{
	// Slice
	SHOW_EDITOR_DETAIL(Slicing);
	ImVec2 backSize = ImGui::GetWindowSize();
	int itemWidth = (ImGui::GetColumnWidth() - 150) / 2;

	const char* types[] = { "Grid Slice By Cell Size" , "Grid Slice By Cell Count", "Auto Slice" };
	static int typeIndex = 0;

	// slice type
	DrawNameUI("Type");
	ImGui::Combo("##Type", &typeIndex, types, IM_ARRAYSIZE(types));

	// slice by size
	if (typeIndex == 0)
	{
		Vector2 vPixelSize = {}, vOffset = {}, vPadding = {};
		DrawNameInt2UI("Pixel Size", "X", "Y", &m_SliceInput, itemWidth);
		DrawNameInt2UI("Offset", "X", "Y", &vOffset, itemWidth);
		DrawNameInt2UI("Padding", "X", "Y", &vPadding, itemWidth);

		//static int pivotIndex = 0;

		//DrawNameUI("Pivot");
		//ImGui::Combo("##Pivot", &pivotIndex, pivots, IM_ARRAYSIZE(pivots));
		//if (pivotIndex == 9)
		//{
		//	Vector2 vCustomPivot = {};
		//	DrawVector2UI("Custom Pivot", &vCustomPivot);
		//}

		if (ImGui::Button("Slice", ImVec2(-1, 0)))
			GetOwner()->SliceBySize(m_SliceInput);
	}

	// slice by count
	else if (typeIndex == 1)
	{
		Vector2 vCount = {}, vOffset = {}, vPadding = {};
		DrawNameInt2UI("Pixel Count", "X", "Y", &m_SliceInput, itemWidth);
		DrawNameInt2UI("Offset", "X", "Y", &vOffset, itemWidth);
		DrawNameInt2UI("Padding", "X", "Y", &vPadding, itemWidth);

		//static int pivotIndex = 0;

		//DrawNameUI("Pivot");
		//ImGui::Combo("##Pivot", &pivotIndex, pivots, IM_ARRAYSIZE(pivots));
		//if (pivotIndex == 9)
		//{
		//	Vector2 vCustomPivot = {};
		//	DrawVector2UI("Custom Pivot", &vCustomPivot);
		//}

		if (ImGui::Button("Slice", ImVec2(-1, 0)))
			GetOwner()->SliceByCount(m_SliceInput);
	}
	else if (typeIndex == 2)
	{
		if (!GetOwner()->IsAutoSlicing())
		{
			if (ImGui::Button("Auto Slice"))
			{
				//GetAtlasView()->AutoSliceAll();
				m_SliceThread = std::thread(&SpriteEditor::AutoSliceAll, GetOwner());
				m_SliceThread.detach();  // 백그라운드에서 실행
			}
		}
		else
		{
			ImGui::Button("Slicing...");
		}
	}

	ImGui::Spacing();

}

void SE_Detail::SelectTexture(DWORD_PTR _ListUI, DWORD_PTR _SelectString)
{
	// 리스트에서 더블킬릭한 항목의 이름을 받아온다.
	ListUI* pListUI = (ListUI*)_ListUI;
	string* pStr = (string*)_SelectString;

	if (*pStr == "None")
	{
		GetOwner()->Load(nullptr);
		return;
	}

	// 해당 항목 에셋을 찾아서, MeshRenderComponent 가 해당 메시를 참조하게 한다.
	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(wstring(pStr->begin(), pStr->end()));
	if (nullptr == pTex)
		return;

	GetOwner()->Load(pTex);
}
