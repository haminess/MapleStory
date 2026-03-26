#pragma once
#include "CScript.h"
class CParticleSystemScript :
    public CScript
{
private:
    CParticleSystem* m_PS;

public:
    virtual void Begin() override;
    virtual void Tick() override;

public:
    CParticleSystemScript();
    ~CParticleSystemScript();
};

