#pragma once
#include "AssetUI.h"
class FontUI :
    public AssetUI
{
public:
    virtual void Render_Update() override;

public:
    FontUI();
    ~FontUI();
};

