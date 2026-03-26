#pragma once


union COLLIDER_ID
{
    struct
    {
        UINT Left;
        UINT Right;
    };

    ULONGLONG ID;
};




class CCollider2D;

class CCollisionMgr :
    public singleton<CCollisionMgr>
{
    SINGLE(CCollisionMgr)
private:
    UINT                    m_Matrix[MAX_LAYER];
    map<ULONGLONG, bool>    m_ColInfo;


public:
    void CollisionCheck(UINT _Left, UINT _Right);
    void CollisionCheckClear() { memset(m_Matrix, 0, sizeof(UINT) * MAX_LAYER); }
    UINT* GetCollisionMatrix() { return m_Matrix; }

private:
    void CollisionBtwLayer(UINT _Left, UINT _Right);
    void CollisionBtwCollider2D(CCollider2D* _LeftCol, CCollider2D* _RightCol);
    bool IsCollision(CCollider2D* _Left, CCollider2D* _Right);
    bool IsCollisionDotLine(Vector2 _Dot, Vector2 _Line1, Vector2 _Line2);


public:
    void Tick();
};

