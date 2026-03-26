#pragma once
#include <Engine/CScript.h>

class CCameraScript :
    public CScript
{
private:
    float           m_CameraSpeed;
    Vector2         m_MapSize;    // 배경 이미지의 크기
    Vector2         m_MapPos;     // 배경 이미지의 중심 위치
    CGameObject*    m_Map;

    wstring         m_TargetName;
    CGameObject*    m_Target;
    bool            m_Find;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

    void SetTarget(CGameObject* _Target) 
    {
        if (_Target)
        {
            m_Find = false;
            m_TargetName = _Target->GetName();
            m_Target = _Target;
        }
        else
        {
            m_Find = false;
            m_TargetName = L"";
            m_Target = _Target;
        }
    }

    void SetBackground(CGameObject* _Background)
    {
        m_Map = _Background;
        if (m_Map)
        {
            Vector3 scale = m_Map->Transform()->GetWorldScale();
            Vector3 pos = m_Map->Transform()->GetWorldPos();
            m_MapSize = Vector2(scale.x, scale.y);
            m_MapPos = Vector2(pos.x, pos.y);
        }
    }

private:
    void CameraOrthgraphicMove();
    void CameraPerspectiveMove();
    Vector3 ClampCameraPosition(const Vector3& _Position);

private:
    float Lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

public:
    CLONE(CCameraScript);
    CCameraScript();
    CCameraScript(const CCameraScript& _Other);
    ~CCameraScript();
};

