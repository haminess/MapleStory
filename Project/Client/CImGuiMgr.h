#pragma once


class EditorUI;
class Inspector;
class CImGuiMgr
	: public singleton<CImGuiMgr>
{
	SINGLE(CImGuiMgr)
private:
	map<string, EditorUI*>	m_mapUI;
	Inspector* m_Inspector;
	HANDLE					m_hNotify;

public:
	int Init();
	void Progress();

public:
	EditorUI* FindUI(const string& _ID);
	Inspector* GetInspector();
	void SetActive(string _ID, bool _On);
	void RegisterEditor(EditorUI* _UI);
	void CreateEditorUI();
	void LoadAssets();

private:
	void ObserveContent();

	void Tick();
	void Render();
};

