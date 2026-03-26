#pragma once
#include <Engine/CState.h>
class CSpawnState :
    public CState
{
public:
    virtual void Enter();
    virtual void Tick();
    virtual void Exit();

public:
    CLONE(CSpawnState);
    CSpawnState();
    ~CSpawnState();
};

