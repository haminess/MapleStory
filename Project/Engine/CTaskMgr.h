#pragma once
#include "singleton.h"


class CTaskMgr :
    public singleton<CTaskMgr>
{
    SINGLE(CTaskMgr);

private:
    vector<tTask>               m_vecTask;

    // 타이머
    // m_vecScriptTimer는 매 프레임 수정되기 때문에 ID 핸들을 따로 관리
    // 매 프레임마다 m_TimerIndex에 ID와 vector index 정보를 업데이트
    // 타이머 ID만 가지고 있으면 m_vecScriptTimer[m_TimerIndex[ID]]로 접근 가능
    vector<tScriptTimerInfo>    m_vecScriptTimer;
    vector<tScriptTimerInfo>    m_vecTimerAdd;
    unordered_map<UINT, size_t> m_TimerIndex;
    UINT                        m_NextTimerId = 1;

    vector<CGameObject*>    m_vecGC;
    vector<CGameObject*>    m_vecDisabled;

    // 오브젝트 생성, 삭제 시점에 트리거
    bool                    m_LevelChanged;


public:
    bool IsLevelChanged() { return m_LevelChanged; }
    void NotifyLevelChanged() { m_LevelChanged = true; }

    void AddTask(const tTask& _task) 
    {
        m_vecTask.push_back(_task);
    }

    UINT AddTimer(tScriptTimerInfo _Info)
    {
        const UINT id = m_NextTimerId++;
        _Info.Id = id;

        m_vecTimerAdd.push_back(_Info);

        //const size_t index = m_vecScriptTimer.size();
        //m_vecScriptTimer.emplace_back(std::move(_Info));

        //m_TimerIndex[id] = index;
        return id;
    }

    UINT SetTimer(CScript* _Inst, SCRIPT_DELEGATE _Func, float _Time, bool _Loop = false)
    {
        tScriptTimerInfo timer = {};

        timer.Inst = _Inst;
        timer.Func = _Func;
        timer.Timer = _Time;
        timer.ElapsedTime = 0.f;
        timer.IsLoop = _Loop;        

        return AddTimer(timer);
    }

    UINT SetTimer(CScript* _Inst, SCRIPT_DELEGATE_1 _Func, float _Time, bool _Loop = false)
    {
        tScriptTimerInfo timer = {};

        timer.Inst = _Inst;
        timer.Func_1 = _Func;
        timer.Timer = _Time;
        timer.ElapsedTime = 0.f;
        timer.IsLoop = _Loop;        

        return AddTimer(timer);
    }

    UINT SetTimer(CScript* _Inst, SCRIPT_DELEGATE_2 _Func, float _Time, bool _Loop = false)
    {
        tScriptTimerInfo timer = {};

        timer.Inst = _Inst;
        timer.Func_2 = _Func;
        timer.Timer = _Time;
        timer.ElapsedTime = 0.f;
        timer.IsLoop = _Loop;        

        return AddTimer(timer);
    }

    tScriptTimerInfo* GetTimer(UINT _TimerID)
    {
        auto it = m_TimerIndex.find(_TimerID);
        if (it == m_TimerIndex.end())
            return nullptr;

        size_t index = it->second;
        return &m_vecScriptTimer[index];
    }

    void StopTimer(UINT _TimerID)
    {
        auto it = m_TimerIndex.find(_TimerID);
        if (it == m_TimerIndex.end())
            return;

        size_t idx = it->second;
        m_vecScriptTimer[idx].Inst = nullptr;
        m_vecScriptTimer[idx].Func = nullptr;
        m_vecScriptTimer[idx].Func_1 = nullptr;
        m_vecScriptTimer[idx].Func_2 = nullptr;

        m_TimerIndex.erase(it);
    }

public:
    void Tick();
    void ExcuteTimer();
};

