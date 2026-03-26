#include "pch.h"
#include "ListUI.h"

#include "imgui/imgui.h"

ListUI::ListUI()
	: EditorUI("##ListUI")
	, m_SelectedIdx(-1)
	, m_bClicked(false)
	, m_DbclickedInst(nullptr)
	, m_DbclickedFunc2(nullptr)
{
}

ListUI::~ListUI()
{
}


void ListUI::Render_Update()
{
	m_bClicked = false;

	for (size_t i = 0; i < m_vecList.size(); ++i)
	{
		int Flags = ImGuiTreeNodeFlags_Leaf;

		if (m_SelectedIdx == i)
		{
			Flags |= ImGuiTreeNodeFlags_Selected;
		}

		if (ImGui::TreeNodeEx(m_vecList[i].c_str(), Flags))
		{
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			{
				m_SelectedIdx = i;
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				// 더블클릭한 항목 문자열 기록
				m_strSelected = m_vecList[i];

				// 등록된 Delegate 호출
				if (m_DbclickedInst && m_DbclickedFunc1)
				{
					(m_DbclickedInst->*m_DbclickedFunc1)((DWORD_PTR)&m_strSelected);
				}
				// 등록된 Delegate 호출
				if (m_DbclickedInst && m_DbclickedFunc2)
				{
					// Selected 메서드 0: object ui, 1: item num
					(m_DbclickedInst->*m_DbclickedFunc2)((DWORD_PTR)this, (DWORD_PTR)&m_strSelected);
				}


				// 비활성화
				if(IsModal())
					SetActive(false);
			}

			ImGui::TreePop();
		}
	}
}

