#pragma once
#include <Engine/CState.h>
class CSkillState :
    public CState
{
private:
    

public:
    virtual void Enter();
    virtual void Tick();
    virtual void Exit();


public:
    CLONE(CSkillState);
    CSkillState();
    CSkillState(const CSkillState& _Other);
    ~CSkillState();
};

