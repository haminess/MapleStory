#include "pch.h"
#include "PresetMgr.h"
#include "CImGuiMgr.h"

PresetMgr::PresetMgr()
	: EditorUI("Map Editor")
{
}

PresetMgr::~PresetMgr()
{
}

void PresetMgr::Render_Update()
{
}


void PresetMgr::CreatePresetEditor(PresetList* _Editor)
{
	_Editor->SetActive(false);
	m_vecPresetEditor.push_back(_Editor);
	CImGuiMgr::GetInst()->RegisterEditor(_Editor);
	_Editor->m_Owner = this;
}

void PresetMgr::ChangeFocusEditor(PresetList* _Editor)
{
	m_CurEditor = _Editor;
	for (int i = 0; i < m_vecPresetEditor.size(); ++i)
	{
		m_vecPresetEditor[i]->m_Focused = false;
	}

	m_CurEditor->m_Focused = true;
}

void PresetMgr::Init()
{
	CreatePresetEditor(new MTileEditor);
	CreatePresetEditor(new MObjectEditor);
}

void PresetMgr::Activate()
{
	for (int i = 0; i < m_vecPresetEditor.size(); ++i)
	{
		m_vecPresetEditor[i]->SetActive(true);
	}
}

void PresetMgr::Deactivate()
{
	for (int i = 0; i < m_vecPresetEditor.size(); ++i)
	{
		m_vecPresetEditor[i]->SetActive(false);
	}
}
