#pragma once
#include "EditorUI.h"

class TreeNode;
class ContentUI :
    public EditorUI
{
private:
    class TreeUI* m_Tree;
    vector<wstring> m_vecAssetPath;   // Content 폴더에 있는 모든 리소스 경로 

public:
    virtual void Render_Update() override;
    void Reset();

private:
    // renew
    void RenewContent();

    // load
    void ReloadContent();

public:
    virtual void Context(TreeNode* _TreeNode) override;


    void SelectAsset(DWORD_PTR _TreeNode);


    void FindAssetPath(const wstring& _FolderPath);
    ASSET_TYPE GetAssetType(const wstring& _Path);


public:
    ContentUI();
    ~ContentUI();
};

