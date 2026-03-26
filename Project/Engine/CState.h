#pragma once
#include "CScript.h"
#include "CStateMachine.h"
//#include "CLogMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CTimeMgr.h"

class CState :
    public CScript
{
protected:
    CStateMachine* m_Owner;
    wstring m_StateKey;

public:
    CStateMachine* GetStateMachine() { return m_Owner; }
    const wstring& GetStateKey() { return m_StateKey; }

    template<typename T>
    T* GetOwner()
    {
        T* pObject = dynamic_cast<T*>(m_Owner->GetOwner());
        assert(pObject);
        return pObject;
    }

public:
    virtual void Init() {}
    virtual void Enter() = 0;
    virtual void Tick() = 0;
    virtual void Exit() = 0;

public:
    virtual CState* Clone() = 0;
    CState(UINT ScriptType);
    ~CState();

    friend class CStateMachine;
};
