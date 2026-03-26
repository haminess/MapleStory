#pragma once
#include "EditorUI.h"
#include "ScriptUI.h"

#include <Engine/Ptr.h>
#include <Engine/assets.h>


class CGameObject;
class ComponentUI;
class AssetUI;
class LevelInfoUI;

class Inspector :
    public EditorUI
{
private:
    CGameObject* m_TargetObject;
    ComponentUI* m_arrComUI[(UINT)COMPONENT_TYPE::END];
    vector<ScriptUI*>   m_vecScriptUI;

    Ptr<CAsset>     m_TargetAsset;
    AssetUI* m_arrAssetUI[(UINT)ASSET_TYPE::END];

    LevelInfoUI* m_LevelUI;

public:
    void SetTargetObject(CGameObject* _Target);
    CGameObject* GetTargetObject() { return m_TargetObject; }
    void SetTargetAsset(Ptr<CAsset> _Asset);
    void SetTargetLevel();
    Ptr<CAsset> GetTargetAsset() { return m_TargetAsset; }

    void RenewGameObject();

public:
    virtual void Render_Update() override;

private:
    void CreateComponentUI();
    void CreateAssetUI();

public:
    Inspector();
    ~Inspector();
};

