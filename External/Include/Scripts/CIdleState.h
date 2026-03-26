#pragma once
#include <Engine/CState.h>
class CIdleState :
    public CState
{
public:
    virtual void Enter();
    virtual void Tick();
    virtual void Exit();

public:
    CLONE(CIdleState);
    CIdleState();
    ~CIdleState();
};

