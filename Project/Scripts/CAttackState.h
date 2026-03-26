#pragma once
#include <Engine/CState.h>

class CAttackState
	: public CState
{
public:
    virtual void Enter();
    virtual void Tick();
    virtual void Exit();

public:
    CLONE(CAttackState);
    CAttackState();
    ~CAttackState();
};

