#pragma once
#include "singleton.h"

class CLevel;

class CLevelMgr :
    public singleton<CLevelMgr>
{
    SINGLE(CLevelMgr);
private:
    CLevel* m_CurLevel;
    CGameObject* m_CurPlayer;

	bool m_Loaded;  // 이번 프레임 레벨 로드 여부

public:
    CLevel* GetCurrentLevel() { return m_CurLevel; }
    CGameObject* GetCurrentPlayer() { return m_CurPlayer; }
    void RegisterObject(class CGameObject* _Object);
    void RegisterPlayer(class CGameObject* _Player) { m_CurPlayer = _Player; }
    CGameObject* FindObjectByName(const wstring& _Name);
    void FindObjectsByName(const wstring& _Name, vector<CGameObject*>& _Out);
    void ChangeLevelState(LEVEL_STATE _NextState);
    void ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _NextLevelState);

	CLevel* Load(const wstring& _FilePath);


public:
    void Progress();

    friend class CTaskMgr;
};

