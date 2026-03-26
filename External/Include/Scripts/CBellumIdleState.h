#pragma once
#include <Engine\CState.h>

class CBellumIdleState : 
    public CState 
{
private:
    float m_fUndergroundTime;
    float m_fUndergroundDuration;

public:
    virtual void Enter() override;
    virtual void Tick() override;
    virtual void Exit() override;

public:
	CLONE_DISABLE(CBellumIdleState);    
    CBellumIdleState();
    ~CBellumIdleState();
};
