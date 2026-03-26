#pragma once
#include <Engine/CScript.h>
class CLinePlatform :
    public CScript
{
private:
    Vector2 m_StartPos;
    Vector2 m_EndPos;

public:
    void SetPos(Vector2 _Start, Vector2 _End) 
    {
        m_StartPos = _Start;
        m_EndPos = _End;
        Collider2D()->SetLinePos(_Start, _End);
    }
    Vector2 GetStartPos() { return m_StartPos; }
    Vector2 GetEndPos() { return m_EndPos; }
    Vector2 GetInclination() { return m_EndPos - m_StartPos; }
    Vector2 GetInclination1()
    {
        XMVECTOR s, r, t;
        XMMatrixDecompose(&s, &r, &t, Collider2D()->GetColliderWorldMat());

        Vector3 pos = t, scale = s;
        Vector3 leftTop = pos - Vector3(scale / 2.f);
        Vector3 rightBottom = pos + Vector3(scale / 2.f);
        return Vector2(rightBottom.x - leftTop.x, rightBottom.y - leftTop.y);
    }

    bool IsInRange(float _x)
    {
        // x가 선분의 x범위를 벗어났는지 체크
        if (_x < min(m_StartPos.x, m_EndPos.x) || _x > max(m_StartPos.x, m_EndPos.x))
            return false;

        // 수직선인 경우
        if (abs(m_StartPos.x - m_EndPos.x) < 0.0001f)
        {
            // x값이 선분의 x값과 같으면 시작점의 y값 반환
            if (abs(_x - m_StartPos.x) < 0.0001f)
                return true;
            return false;
        }
    }

    float GetPosY(float _x)
    {
        // 수직선인 경우
        if (abs(m_StartPos.x - m_EndPos.x) < 0.0001f)
        {
            // x값이 선분의 x값과 같으면 시작점의 y값 반환
            if (abs(_x - m_StartPos.x) < 0.0001f)
                return m_EndPos.y;
        }

        // 기울기 계산
        float slope = (m_EndPos.y - m_StartPos.y) / (m_EndPos.x - m_StartPos.x);

        // y = mx + b 공식 사용
        // b(y절편) = y1 - mx1
        float b = m_StartPos.y - slope * m_StartPos.x;

        // x에서의 y값 계산
        return Transform()->GetWorldPos().y + slope * _x + b;
    }

    


public:
    virtual void Tick() override {}
    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    CLONE(CLinePlatform);
    CLinePlatform();
    ~CLinePlatform();
};

