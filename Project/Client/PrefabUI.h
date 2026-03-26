#pragma once
#include "AssetUI.h"
class PrefabUI :
    public AssetUI
{
private:
    CGameObject* m_NewObject;

public:
    virtual void Render_Update() override;

public:
    PrefabUI();
    ~PrefabUI();
};

