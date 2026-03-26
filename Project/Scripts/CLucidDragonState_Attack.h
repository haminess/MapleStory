#pragma once
#include <Engine\CState.h>

class CLucidDragonState_Attack
    : public CState
{
private:
    CLucidDragonScript* m_DragonScript;

public:
    virtual void Init() override;
    virtual void Enter() override;
    virtual void Tick() override;
    virtual void Exit() override;

public:
    CLONE(CLucidDragonState_Attack);
    CLucidDragonState_Attack();
    ~CLucidDragonState_Attack();
};