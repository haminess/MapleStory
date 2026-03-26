#pragma once
#include "EditorUI.h"

struct TreeNode
{
private:
    static UINT         g_GlobalID;

private:
    class TreeUI* m_Owner;
    TreeNode* m_Parent;
    vector<TreeNode*>   m_vecChild;

    string              m_Name;
    string              m_ID;
    DWORD_PTR           m_Data;
    string              m_DataID;

    bool                m_Frame;
    bool                m_Selected;
    bool                m_Opened;
    bool                m_BeingRenamed;

public:
    DWORD_PTR GetData() { return m_Data; }
    string GetName() { return m_Name; }
    TreeUI* GetOwner() { return m_Owner; }

public:
    void Render_Update();
    void AddChildNode(TreeNode* _ChildNode)
    {
        m_vecChild.push_back(_ChildNode);
        _ChildNode->m_Parent = this;
    }
	void BeginRename() { m_BeingRenamed = true; }
	void EndRename() { m_BeingRenamed = false; }

public:
    TreeNode();
    ~TreeNode();

    friend class TreeUI;
};


class TreeUI :
    public EditorUI
{
private:
    TreeNode* m_Root;

    map<string, bool>   m_OpenedInfo;

    vector<TreeNode*>   m_vecSelected;
    bool                m_ShowRoot;
    bool                m_MultiSelection;
    bool                m_SelfDragDrop;

    EditorUI*           m_SelectedInst;
    EUI_DELEGATE_1      m_SelectedFunc;

    EditorUI*           m_SelfDragDropInst;
    EUI_DELEGATE_2      m_SelfDragDropFunc;

    TreeNode* m_DraggedNode;
    TreeNode* m_DroppedNode;


public:
    void ShowRoot(bool _Show) { m_ShowRoot = _Show; }
    void MutiSelection(bool _Multi) { m_MultiSelection = _Multi; }
    bool IsMultiSelection() { return m_MultiSelection; }
    vector<TreeNode*> GetVecSelectedNode() { return m_vecSelected; }
    void SelfDragDrop(bool _Self) { m_SelfDragDrop = _Self; }
    bool IsSelfDragDrop() { return m_SelfDragDrop; }

    // Select
    void MultiSelection(bool _Multi) { m_MultiSelection = _Multi; }
    void AddSelectedNode(TreeNode* _Node);
    void SetDraggedNode(TreeNode* _Node) { m_DraggedNode = _Node; }
    void SetDroppedNode(TreeNode* _Node);

    void AddDynamicSelect(EditorUI* _Inst, EUI_DELEGATE_1 _MemFunc) { m_SelectedInst = _Inst; m_SelectedFunc = _MemFunc; }
    void AddDynamicSelfDragDrop(EditorUI* _Inst, EUI_DELEGATE_2 _MemFunc) { m_SelfDragDropInst = _Inst; m_SelfDragDropFunc = _MemFunc; }

    TreeNode* AddItem(TreeNode* _ParentNode, const string& _Name, DWORD_PTR _Data, bool _Frame = false);
    void Clear()
    {
        if (nullptr != m_Root)
            delete m_Root;
        m_Root = nullptr;
    }

public:
    virtual void Render_Update() override;

public:
    TreeUI();
    ~TreeUI();

    friend class TreeNode;
};

