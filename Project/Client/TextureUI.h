#pragma once
#include "AssetUI.h"
class TextureUI :
    public AssetUI
{
private:
    CGameObject* m_NewObject;

public:
    virtual void Render_Update() override;

public:
    TextureUI();
    ~TextureUI();
};

