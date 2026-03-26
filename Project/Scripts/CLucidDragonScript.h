#pragma once
#include <Engine\CScript.h>
class CLucidDragonScript :
    public CScript
{
private:
    Vector2 m_Offset;
    int     m_Count;
    float   m_Distance;

    Vector2 m_HitRange;
    Vector2 m_HitPos;
    vector<CGameObject*> m_vecBreathAnim;
    CGameObject* m_AlertAnim;

public:
    void Spawn();
    void Attack();
    void BeginBreath();
    void EndBreath();

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    CLONE(CLucidDragonScript)
    CLucidDragonScript();
    CLucidDragonScript(const CLucidDragonScript& _Other);
    ~CLucidDragonScript();
};
