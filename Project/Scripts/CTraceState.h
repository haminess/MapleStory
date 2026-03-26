#pragma once
#include <Engine/CState.h>
class CTraceState :
    public CState
{
private:
    Vector2 m_InitPos;
    bool    m_Return;

public:
    Vector2 GetInitPos() { return m_InitPos; }
    void SetInitPos(Vector2 _Pos) { m_InitPos = _Pos; }

public:
    virtual void Enter() override;
    virtual void Tick() override;
    virtual void Exit() override;

public:
    CLONE(CTraceState);
    CTraceState();
    ~CTraceState();
};

