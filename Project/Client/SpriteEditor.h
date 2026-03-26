#pragma once
#include "EditorUI.h"
#include <Engine/CSprite.h>
#include <Engine/CFlipbook.h>

class SE_AtlasView;
class SE_Detail;
class CTexture;
class CSprite;
class CFlipbook;


class SpriteEditor :
    public EditorUI
{
private:
    // Editor
    SE_AtlasView*       m_AtlasView;
    SE_Detail*          m_Detail;

    // Sprite
    Ptr<CTexture>       m_AtlasTex;
    list<CSprite>       m_Sprite;

    vector<CSprite*>    m_Selected;

    // Slice
    set<Vector2>        m_PixelID;  // 등록된적이 있는 픽셀인지 확인하는 용도
    CSprite*            m_AutoSlice;

    CGameObject*    m_MapPreviewObject;


public:
    SE_AtlasView* GetAtlasView() { return m_AtlasView; }
    SE_Detail* GetDetail() { return m_Detail; }

    Ptr<CTexture> GetAtlas() { return m_AtlasTex; }
    list<CSprite>& GetSprites() { return m_Sprite; }

    CGameObject* GetPreviewObject();

public:
    // Sprite
    void CreateSprite(Vector2 _Pos, Vector2 _Scale, Vector2 _Back = { 0.f, 0.f }, Vector2 _Offset = {0.f, 0.f});
    void DeleteSprite(CSprite* _Sprite)
    {
        auto iter = m_Sprite.begin();
        for (; iter != m_Sprite.end(); ++iter)
        {
            if (&*iter == _Sprite)
            {
                m_Sprite.erase(iter);
                return;
            }
        }
    }
    void ClearAllSprite()
    {
        m_Sprite.clear();
    }

    // Select
    void SelectSprite(CSprite* _Idx) { m_Selected.push_back(_Idx); }
    void DeselectSprite(CSprite* _Sprite)
    {
        auto iter = m_Selected.begin();
        for (; iter != m_Selected.end(); ++iter)
        {
            if (*iter == _Sprite)
            {
                m_Selected.erase(iter);
                return;
            }
        }
    }
    void SelectClear() { m_Selected.clear(); }
    const vector<CSprite*>& GetSelected() { return m_Selected; }
    CSprite* GetSelectedOne() 
    { 
        if (m_Selected.size() > 0)
            return m_Selected.back();

        return nullptr;
    }

    // Slice
    void SliceByCount(Vector2 _Count);
    void SliceBySize(Vector2 _Size);

    void AutoSliceOnce(Vector2 _PixelPos);
    void AutoSliceAll();
    bool IsAutoSlicing() { return m_AutoSlice ? true : false; }
private:
    bool CalcSpriteSize(Vector2 _PixelPos);
    bool IsPixelOk(Vector2 _PixelPos);


public:
    void Init();
    virtual void Render_Update() override;

public:
    virtual void Activate() override;
    virtual void Deactivate() override;

    

public:
    // 스프라이트 데이터 관련
    void LoadFromDirectory();
    void Load(Ptr<CTexture> _Tex);
    void Save();

public:
    void CreateFlipbook(bool _Loop = false);

public:
    SpriteEditor();
    ~SpriteEditor();
};

