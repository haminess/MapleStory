#pragma once
#include "SE_Sub.h"

#include <Engine/CTexture.h>

struct tSlice
{
    ImVec2          LT;
    ImVec2          RB;
};

class SE_AtlasView :
    public SE_Sub
{
private:
    ImVec2          m_AtlasPos; // Image 위젯 좌상단 실제 위치
    ImVec2          m_MousePos; // 실제 마우스 위치

    ImVec2          m_MouseLT;  // 마우스 위치에 해당하는 Texture 의 픽셀 좌표
    ImVec2          m_MouseRB;  // 마우스 위치에 해당하는 Texture 의 픽셀 좌표

    set<Vector2>       m_PixelID;  // 등록된적이 있는 픽셀인지 확인하는 용도


    float           m_WidthSize;
    float           m_WheelScale;
    float           m_Ratio;        // 실제 해상도 대비 출력 이미지의 비율

    bool m_IsSlicing;


    // [ME]
    // SpriteEditor
    float   m_ScaleFactor;
    Vector2 m_CursorPos;

    // Edit
    bool m_OnCreate;
    bool m_OnMove;

public:
    virtual void Render_Update() override;

public:
    bool IsSlicing() { return m_IsSlicing; }

    void AutoSliceOnce(Vector2 _PixelPos);
    void AutoSliceAll();
    bool CalcSpriteSize(Vector2 _PixelPos);
    bool IsPixelOk(Vector2 _PixelPos);

private:
    void Render_Sprite();
    void CreateDrag();
    void OnDrag();

    Vector2 PosToPixel(Vector2 _Pos)
    {
        return (_Pos - Vector2(m_AtlasPos.x, m_AtlasPos.y)) / m_Ratio;
    }
    Vector2 PixelToPos(Vector2 _Pixel)
    {
        return _Pixel * m_Ratio + Vector2(m_AtlasPos.x, m_AtlasPos.y);
    }
    Vector2 SizeToPixel(Vector2 _Size)
    {
        return _Size / m_Ratio;
    }
    Vector2 PixelToSize(Vector2 _Pixel)
    {
        return _Pixel * m_Ratio;
    }

public:
    SE_AtlasView();
    ~SE_AtlasView();

    friend class SpriteEditor;
};

