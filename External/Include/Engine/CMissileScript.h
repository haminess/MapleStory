#pragma once
#include "CScript.h"
class CMissileScript :
    public CScript
{
private:
    Vector3 m_Velocity;

public:
    void SetVelocity(Vector3 _Velo) { m_Velocity = _Velo; }

public:
    virtual void Tick() override;
    virtual void Begin() override;

public:
    CMissileScript();
    ~CMissileScript();
};

