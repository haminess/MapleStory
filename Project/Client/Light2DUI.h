#pragma once
#include "ComponentUI.h"

class Light2DUI :
    public ComponentUI
{
private:

public:
    virtual void Render_Update() override;

public:
    Light2DUI();
    ~Light2DUI();
};

