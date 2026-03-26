#pragma once
#include "AssetUI.h"
class FlipbookUI :
    public AssetUI
{
private:
    CGameObject* m_NewObject;

public:
    virtual void Render_Update() override;

public:
    FlipbookUI();
    ~FlipbookUI();
};

