#pragma once
#include <Engine\CScript.h>
class CSpawnerScript :
    public CScript
{
private:
    wstring m_PrefabName;
    Ptr<CPrefab> m_Prefab;

public:
    void Spawn();

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CSpawnerScript);
    CSpawnerScript(UINT _Type = (UINT)SCRIPT_TYPE::SPAWNERSCRIPT);
    CSpawnerScript(const CSpawnerScript& _Other);
    ~CSpawnerScript();
};

