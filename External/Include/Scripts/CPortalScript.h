#pragma once
#include <Engine\CScript.h>
class CPortalScript :
    public CScript
{
private:
    wstring m_LevelName;
    int     m_MoveID;

    int     m_PortalID;

public:
    int GetPortalID() { return m_PortalID; }
    int GetMovePortalID() { return m_MoveID; }
    const wstring& GetMoveLevel() { return m_LevelName; }

public:
    virtual void Init() override {}
    virtual void Tick() override {}

    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CPortalScript);
    CPortalScript(UINT _Type = (UINT)SCRIPT_TYPE::PORTALSCRIPT);
    CPortalScript(const CPortalScript& _Other);
    ~CPortalScript();
};

