#pragma once
#include "ComponentUI.h"
class ParticleSystemUI :
    public ComponentUI
{
public:

    virtual void Render_Update()override {};

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

