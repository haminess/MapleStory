#pragma once
#include <Engine/CScript.h>

enum class BLOCK_DIR
{
    LEFT,
    RIGHT,
    BOTH,
};

class CWallScript :
    public CScript
{
private:
    BLOCK_DIR m_Dir;

public:
    BLOCK_DIR GetDirection() { return m_Dir; }
    void SetDirection(BLOCK_DIR _Dir) { m_Dir = _Dir; }

    virtual void Init() override;
    virtual void Tick() override {}
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CWallScript);
    CWallScript();
    CWallScript(const CWallScript& _Other);
    ~CWallScript();
};

