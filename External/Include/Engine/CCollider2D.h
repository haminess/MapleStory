#pragma once
#include "CComponent.h"

enum class COLLIDER_TYPE
{
    BOX,
    CIRCLE,
    LINE,   // pos1: box 좌상단, pos2: box 우하단
    END
};

class CCollider2D :
    public CComponent
{
protected:
    Vector2     m_Offset;
    Vector2     m_Scale;
    Vector2     m_FinalPos;
    Matrix      m_matColliderWorld;     // 크기, 회전, 이동
    bool        m_IndependentScale;
    COLLIDER_TYPE m_Type;

    Vector3     m_CollisionArea;


    int         m_OverlapCount;

    bool        m_IsCollidable;
    bool        m_IsMovable;

    float       m_Timer;

public:
    Vector2 GetOffset() { return m_Offset; }
    void SetOffset(Vector2 _Offset) { m_Offset = _Offset; }
    Vector2 GetScale() { return m_Scale; }
    void SetScale(Vector2 _Scale) { m_Scale = _Scale; }

    Vector2 GetFinalPos() 
    {
        return m_matColliderWorld.Translation();
    }
    Vector2 GetFinalScale() 
    {
        Vector3 scale;
        XMVECTOR xmScale;
        XMVECTOR Temp1;
        XMVECTOR Temp2;
        XMMatrixDecompose(&xmScale, &Temp1, &Temp2, m_matColliderWorld);

        scale.x = XMVectorGetX(xmScale);
        scale.y = XMVectorGetY(xmScale);
        scale.z = XMVectorGetZ(xmScale);

        return scale;
    }

    COLLIDER_TYPE GetType() { return m_Type; }
    void SetType(COLLIDER_TYPE _Type) { m_Type = _Type; }
    void SetLinePos(Vector2 _P1, Vector2 _P2) 
    { 
        float xMinus = _P2.x - _P1.x;
        float yMinus = _P2.y - _P1.y;
        m_Offset.x = xMinus == 0 ? _P1.x : _P1.x + xMinus / 2.f;
        m_Offset.y = yMinus == 0 ? _P1.y : _P1.y + yMinus / 2.f;
        m_Scale = Vector2(xMinus, yMinus);
    }

    Vector3 GetCollisionArea() { return m_CollisionArea; }
    void SetCollisionArea(Vector3 _Area) { m_CollisionArea = _Area; }

    Vector2 GetIntersection() { return Vector2(m_CollisionArea.x, m_CollisionArea.y); }
    Vector3 GetStartPos() 
    {
        XMVECTOR s, r, t;
        XMMatrixDecompose(&s, &r, &t, m_matColliderWorld);

        Vector3 pos = t, scale = s;
        Vector3 leftTop = pos - Vector3(scale / 2.f);

        return leftTop;
    }
    Vector3 GetEndPos() 
    {
        XMVECTOR s, r, t;
        XMMatrixDecompose(&s, &r, &t, m_matColliderWorld);

        Vector3 pos = t, scale = s;
        Vector3 rightBottom = pos + Vector3(scale / 2.f);

        return rightBottom;
    }

    const Matrix& GetColliderWorldMat() { return m_matColliderWorld; }

    void SetIndependentScale(bool _Scale) { m_IndependentScale = _Scale; }
    bool IsIndependentScale() { return m_IndependentScale; }
    void SetMovable(bool _Move) { m_IsMovable = _Move; }
    void SetCollidable(bool _Collision) { m_IsCollidable = _Collision; }
    bool IsMovable() { return m_IsMovable; }
    bool IsCollidable() { return m_IsCollidable; }

    void SetTimer(float _Time) { m_Timer = _Time; }

public:
    virtual void FinalTick() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

private:
    void BoxRender();
    void LineRender();

public:
    void BeginOverlap(CCollider2D* _Other);
    void Overlap(CCollider2D* _Other);
    void EndOverlap(CCollider2D* _Other);

public:
    CLONE(CCollider2D);
    CCollider2D();
    CCollider2D(const CCollider2D& _Origin);
    ~CCollider2D();
};

