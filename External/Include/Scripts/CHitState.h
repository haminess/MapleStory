#pragma once
#include <Engine/CState.h>
class CHitState :
    public CState
{
    float m_ExitTime;
    float m_ElapsedTime;

public:
    virtual void Enter();
    virtual void Tick();
    virtual void Exit();

public:
    CLONE(CHitState);
    CHitState();
    ~CHitState();
};

