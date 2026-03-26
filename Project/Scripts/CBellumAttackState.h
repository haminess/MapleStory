#pragma once
#include <Engine\CState.h>
#include "CBellumScript.h"

class CBellumAttackState : 
    public CState
{
private:
    float m_fAttackTime;
    float m_fAttackDuration;
    int m_iCurrentPhase;
    ATTACK_PATTERN m_currentPattern;

public:
    virtual void Enter() override;
    virtual void Tick() override;
    virtual void Exit() override {}

    void SelectNextPattern();
    //void ExecutePattern();

public:
    CLONE_DISABLE(CBellumAttackState);
    CBellumAttackState();
    ~CBellumAttackState();
};
