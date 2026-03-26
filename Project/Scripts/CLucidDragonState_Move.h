#pragma once
#include <Engine\CState.h>

class CLucidDragonState_Move
    : public CState
{
private:
    bool m_IsDowned;

public:
    virtual void Init() override;
    virtual void Enter() override;
    virtual void Tick() override;
    virtual void Exit() override;

public:
    CLONE(CLucidDragonState_Move);
    CLucidDragonState_Move();
    ~CLucidDragonState_Move();
};
