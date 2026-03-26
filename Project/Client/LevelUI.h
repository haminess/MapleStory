#pragma once
#include "EditorUI.h"

class CLevel;
class TreeUI;
class LevelUI :
    public EditorUI
{
private:
    class TreeUI* m_LevelTree;

    vector<wstring> m_vecLevelPath;   // 레벨 경로 

public:
    virtual void Render_Update() override {}
    virtual void Context(TreeNode* _TreeNode) override;

    void RenewLevel();
    void SelectLevel(DWORD_PTR _TreeNode);

    void ClearLevel() { m_vecLevelPath.clear(); }
	void PushLevel(wstring _Level) { m_vecLevelPath.push_back(_Level); }

    void SaveAllLevels();

public:
    LevelUI();
    ~LevelUI();
};

