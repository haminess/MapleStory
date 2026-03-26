#include "pch.h"
#include "PresetList.h"
#include "PresetMGr.h"
#include <Engine/CTexture.h>


PresetList::PresetList(string _ID)
	: EditorUI(_ID)
	, m_Owner(nullptr)
	, m_Focused(false)
{
}

PresetList::~PresetList()
{
}

void PresetList::Render_Update()
{
	if (!m_Focused && ImGui::IsWindowFocused())
	{
		m_Owner->ChangeFocusEditor(this);
	}

	// 에디터 동작
	if (m_Focused)
		Render_Editor();

	Render_List();
}

void PresetList::Render_List()
{
	ImVec2 windowMax = ImGui::GetWindowContentRegionMax();
	float spacing = ImGui::GetStyle().ItemSpacing.x;

	// 한 줄에 들어갈 수 있는 최대 그룹 개수 계산
	int groupsPerRow = static_cast<int>((windowMax.x + spacing) / (GROUP_SIZE + spacing));
	if (groupsPerRow < 1) groupsPerRow = 1;

	for (int i = 0; i < m_vecPreset.size(); ++i)
	{
		// 한 줄의 시작이 아닐 경우에만 SameLine 호출
		if (i % groupsPerRow != 0)
		{
			ImGui::SameLine();
		}

		Render_Preset(i);
	}
}

void PresetList::Render_Preset(int _Idx)
{
	Ptr<CTexture> pTex = m_vecPreset[_Idx].pBtnTex;
	string name = m_vecPreset[_Idx].Name;

	ImGui::BeginChild(("PresetGroup_" + std::to_string(_Idx)).c_str(), ImVec2(GROUP_SIZE, GROUP_SIZE), false);
	{
		// 버튼 중앙 정렬
		float btnPosX = (GROUP_SIZE - BTN_SIZE) * 0.5f;
		float btnPosY = (GROUP_SIZE - BTN_SIZE - ImGui::GetTextLineHeight()) * 0.5f;
		ImGui::SetCursorPos(ImVec2(btnPosX, btnPosY));

		// 버튼 렌더링
		void* tex = pTex == nullptr ? nullptr : (void*)pTex->GetSRV().Get();
		string tile_id = "##preset_" + std::to_string(_Idx);
		if (ImGui::ImageButton(tile_id.c_str(), tex, ImVec2(BTN_SIZE, BTN_SIZE)))
		{
			SelectPreset(_Idx);
		}

		// 텍스트 중앙 정렬
		float textWidth = ImGui::CalcTextSize(name.c_str()).x;
		float textPosX = (GROUP_SIZE - textWidth) * 0.5f;
		ImGui::SetCursorPosX(textPosX);
		ImGui::Text(name.c_str());
	}
	ImGui::EndChild();
}

void PresetList::AddPreset(string _PresetName, Ptr<CAsset> _Asset, Ptr<CTexture> _Tex)
{
	tPresetInfo info = {};
	info.Name = _PresetName;
	info.pAsset = _Asset;
	info.pBtnTex = _Tex;

	m_vecPreset.push_back(info);
}

void PresetList::AddAllPreset()
{
}
