#pragma once
#include <Engine\CState.h>
class CDieState :
    public CState
{

public:
    virtual void Enter();
    virtual void Tick();
    virtual void Exit();

public:
    CLONE(CDieState);
    CDieState();
    ~CDieState();
};

