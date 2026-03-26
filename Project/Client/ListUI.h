#pragma once
#include "EditorUI.h"

class ListUI :
	public EditorUI
{
protected:
	vector<string>  m_vecList;
	string m_strSelected;

	int             m_SelectedIdx;
	bool			m_bClicked;
	
	bool			m_Toggle;

	// DoubleClicked 시 호출시킬 Delegate
	EditorUI* m_DbclickedInst;
	EUI_DELEGATE_1 m_DbclickedFunc1;
	EUI_DELEGATE_2 m_DbclickedFunc2;

public:
	void AddItem(const string& _Item) { m_vecList.push_back(_Item); }
	void AddItem(const wstring& _Item) { m_vecList.push_back(string(_Item.begin(), _Item.end())); }

	void AddItem(const vector<string>& _vecItem) { m_vecList.insert(m_vecList.end(), _vecItem.begin(), _vecItem.end()); }
	void AddItem(const vector <wstring>& _vecItem)
	{
		for (size_t i = 0; i < _vecItem.size(); ++i)
		{
			m_vecList.push_back(string(_vecItem[i].begin(), _vecItem[i].end()));
		}
	}

	string GetSelectedString() { return m_strSelected; }
	void AddDynamicDoubleClicked(EditorUI* _Inst, EUI_DELEGATE_1 _MemFunc)
	{
		m_DbclickedInst = _Inst;
		m_DbclickedFunc1 = _MemFunc;
	}
	void AddDynamicDoubleClicked(EditorUI* _Inst, EUI_DELEGATE_2 _MemFunc)
	{
		m_DbclickedInst = _Inst;
		m_DbclickedFunc2 = _MemFunc;
	}


public:
	const vector<string>& GetList() { return  m_vecList; }
	int GetSelectedNum() { return m_SelectedIdx; }
	void SelectItem(int _Num) { m_SelectedIdx = _Num; }
	void GetSelectedNum(int _num) { m_SelectedIdx = _num; }
	void Clear() { m_vecList.clear(); }

public:
	virtual void Render_Update() override;

private:
	virtual void Activate() override
	{
		m_strSelected = "";
		m_DbclickedFunc1 = nullptr;
		m_DbclickedFunc2 = nullptr;
	}
	virtual void Deactivate() override
	{
		m_vecList.clear();
		m_SelectedIdx = -1;
	}

public:
	ListUI();
	~ListUI();
};

