
#pragma once
#include <Engine/CState.h>

class CPlayerAttackState :
    public CState
{
public:
    virtual void Enter();
    virtual void Tick();
    virtual void Exit();

public:
    CLONE(CPlayerAttackState);
    CPlayerAttackState();
    ~CPlayerAttackState();
};
