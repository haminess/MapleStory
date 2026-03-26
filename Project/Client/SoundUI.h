#pragma once
#include "AssetUI.h"
class SoundUI :
    public AssetUI
{
private:

public:
    virtual void Render_Update() override;

public:
    SoundUI();
    ~SoundUI();
};

