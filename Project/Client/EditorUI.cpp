#include "pch.h"
#include "EditorUI.h"
#include <Engine/CKeyMgr.h>

void EditorUI::ControlView(Vector2& _ViewPos, float& _ViewScale)
{
	// 화면 확대
	float wheelDelta = ImGui::GetIO().MouseWheel;
	if (wheelDelta > 0.f)
	{
		_ViewScale += 0.1f;
	}
	if (wheelDelta < 0.f)
	{
		_ViewScale -= 0.1f;
	}

	if (10.f < _ViewScale)
		_ViewScale = 10.f;
	if (_ViewScale < 0.1f)
		_ViewScale = 0.1f;

	// 화면 이동
	if (KEY_PRESSED(KEY::RBTN))
	{
		_ViewPos += CKeyMgr::GetInst()->GetMouseDir();
	}
}

EditorUI::EditorUI(const string& _ID)
	: m_Active(true)
	, m_ID(_ID)
	, m_ParentUI(nullptr)
	, m_Modal(false)
	, m_Move(true)
{
}

EditorUI::~EditorUI()
{
	DeleteVec(m_vecChildUI);
}



void EditorUI::Tick()
{
	if (!m_Active)
		return;
}

void EditorUI::Render()
{
	if (!m_Active)
		return;

	// 최상위 부모 UI
	if (nullptr == m_ParentUI)
	{
		// Modalless
		if (false == m_Modal)
		{
			Render_Modalless();
		}

		// Modal
		else
		{
			Render_Modal();
		}
	}

	// Child UI 인 경우
	else
	{
		Render_Child();
	}
}

void EditorUI::Render_Modalless()
{
	string strID = m_Name + m_ID;

	bool Active = m_Active;

	m_WndFlag |= ImGuiWindowFlags_HorizontalScrollbar;

	if (m_UseMenuBar)
		m_WndFlag = ImGuiWindowFlags_MenuBar;
	if (!m_Move)
		m_WndFlag |= ImGuiWindowFlags_NoMove;

	ImGui::Begin(strID.c_str(), &Active, m_WndFlag);

	if (!ImGui::IsWindowFocused)
		return;

	// ImGui 의 X 버튼을 누른 경우
	if (Active != m_Active)
		SetActive(Active);

	Render_Update();

	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		if (!m_vecChildUI[i]->IsActive())
		{
			continue;
		}

		m_vecChildUI[i]->Render();

		for (int j = i + 1; j < m_vecChildUI.size(); ++j)
		{
			if (m_vecChildUI[j]->IsActive())
			{
				ImGui::Separator();
				break;
			}
		}
	}
	

	ImGui::End();
}


void EditorUI::Render_Modal()
{
	string strID = m_Name + m_ID;

	bool Active = m_Active;

	ImGui::OpenPopup(strID.c_str());

	if (ImGui::BeginPopupModal(strID.c_str(), &Active, m_WndFlag))
	{
		Render_Update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			if (!m_vecChildUI[i]->IsActive())
			{
				continue;
			}

			m_vecChildUI[i]->Render();

			for (int j = i + 1; j < m_vecChildUI.size(); ++j)
			{
				if (m_vecChildUI[j]->IsActive())
				{
					ImGui::Separator();
					break;
				}
			}
		}

		ImGui::EndPopup();
	}
	else
	{
		SetActive(Active);
	}
}

void EditorUI::Render_Child()
{
	string strID = m_Name + m_ID;

	ImGui::BeginChild(strID.c_str(), m_ChildSize, ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysUseWindowPadding);

	Render_Update();

	for (size_t i = 0; i < m_vecChildUI.size(); ++i)
	{
		m_vecChildUI[i]->Render();

		for (int j = i + 1; j < m_vecChildUI.size(); ++j)
		{
			if (m_vecChildUI[j]->IsActive())
			{
				ImGui::Separator();
				break;
			}
		}
	}

	ImGui::EndChild();
}


void EditorUI::SetActive(bool _bActive)
{
	if (m_Active == _bActive)
		return;

	m_Active = _bActive;

	if (m_Active)
		Activate();
	else
		Deactivate();
}