#pragma once
#include <Engine/CScript.h>
class CParticleSystemScript :
    public CScript
{
private:
    CParticleSystem* m_PS;

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

public:
    CLONE(CParticleSystemScript);
    CParticleSystemScript();
    ~CParticleSystemScript();
};

