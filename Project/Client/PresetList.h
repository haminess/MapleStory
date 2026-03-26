#pragma once
#include "EditorUI.h"

#define BTN_SIZE 64
#define GROUP_SIZE 100  // 그룹 전체 크기 정의

struct tPresetInfo
{
    string Name;
    Ptr<CTexture>   pBtnTex;
    Ptr<CAsset>     pAsset;
};

class PresetMgr;
class PresetList :
    public EditorUI
{
private:
    PresetMgr* m_Owner;
    vector<tPresetInfo> m_vecPreset;

    bool m_Focused;

protected:
    vector<tPresetInfo>& GetPresetList() { return m_vecPreset; }
    tPresetInfo& GetPreset(int i) { return m_vecPreset[i]; }
    int GetPresetCount() { return m_vecPreset.size(); }
    
    template <typename T>
    Ptr<T> GetAsset(int i) { return dynamic_cast<T*>(m_vecPreset[i].pAsset.Get()); }

public:
    virtual void Render_Update();
    virtual void Render_Editor() = 0;
    void Render_List();
    void Render_Preset(int _Idx);

public:
    void AddPreset(string _PresetName, Ptr<CAsset> _Asset, Ptr<CTexture> = nullptr);
    void AddAllPreset();

    virtual void SelectPreset(int _Idx) = 0;

    PresetList(string _ID);
    ~PresetList();

    friend class PresetMgr;
};

