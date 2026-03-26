#pragma once
#include <Engine\CScript.h>

class CPortalScript;
class CPlayerScript;
class CSpawnerScript;
class CLevelScript :
    public CScript
{
private:
    wstring m_BgmName;

    CPlayerScript* m_Player;
    CGameObject* m_Map;
    vector<CPortalScript*>  m_vecPortal;
    vector<CSpawnerScript*> m_vecSpawner;

    UINT* m_LevelLayer;

public:
    virtual void Begin() override;
    virtual void Tick() override {}

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    void RegisterPortal(CPortalScript* _Portal, int _ID);
    CPortalScript* GetPortal(int _PortalNum);

public:
    CLONE(CLevelScript);
    CLevelScript();
    ~CLevelScript();
};

