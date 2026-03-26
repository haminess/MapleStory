#pragma once
#include <Engine/CScript.h>
class CPlatformScript :
    public CScript
{
private:
    Vector3 m_StartPos;
    Vector3 m_EndPos;
    Vector3 m_vInclination;
    float m_Height;

    bool m_Downable;
    bool m_MonBlock;
    bool m_Prior;

    bool m_SideCollidable;
    bool m_IsChange;
    bool m_ApplyTransform;

public:
    void SetPlatformPos(Vector3 _StartPos, Vector3 _EndPos, float _Height = 20.f);
    void SetPlatformPos();

    void SetInclination(Vector3 _Dir)
    {
        m_vInclination = _Dir;
        m_vInclination.Normalize();
    }

	bool IsBlock() { return m_MonBlock; }
	bool IsDownable() { return m_Downable; }
	bool IsPrior() { return m_Prior; }

    Vector3 GetStartPos() { return m_StartPos; }
    Vector3 GetEndPos() { return m_EndPos; }
    float GetHeight() { return m_Height; }
    Vector3 GetInclination() { return m_vInclination; }

    bool IsUpsideCollision(CCollider2D* _OtherCollider);

private:
    void BlockOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    CLONE(CPlatformScript);
    CPlatformScript();
    CPlatformScript(const CPlatformScript& _Other);
    ~CPlatformScript();
};

