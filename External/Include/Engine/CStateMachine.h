#pragma once
#include "CComponent.h"
class CState;
class CStateMachine :
    public CComponent
{
private:
    map<wstring, CState*>  m_mapState;
    CState*                m_CurState;

    float                  m_EnterTime;

public:
    const map<wstring, CState*>& GetStates() { return m_mapState; }
    void AddState(const wstring& _StateKey, CState* _State);
    CState* FindState(const wstring& _StateKey);
    CState* GetCurrentState() { return m_CurState; }
    void ChangeState(const wstring& _StateKey);

    float GetStateEnteredTime(); 
    float GetStateElapsedTime();

public:
    virtual void Init() override;
    virtual void FinalTick() override;
    virtual void SaveComponent(FILE* _File);
    virtual void LoadComponent(FILE* _File);
    
public:
    CLONE(CStateMachine);
    CStateMachine();
    CStateMachine(const CStateMachine& _Other);
    ~CStateMachine();
};

