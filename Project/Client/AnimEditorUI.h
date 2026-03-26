
#pragma once
#include "EditorUI.h"
class CFlipbookPlayer;
class CFlipbook;
class CSprite;

class AnimEditorUI :
    public EditorUI
{
private:
    float m_ScaleFactor;
    Vector2 m_PreviewPos;

    Ptr<CFlipbook>      m_CurAnim;
    CFlipbookPlayer*    m_PreviewPlayer;
    int m_CurSpriteIdx;

    bool m_IsNew;

    // Select Sprites
    vector<CSprite*>    m_Selected;



public:
    virtual void Render() override;
    virtual void Render_Update() override;

private:
    void Preview();
    void Detail_Anim();
    void Detail_Frame();

public:
    void SetCurAnim(Ptr<CFlipbook> _Anim);
    Ptr<CFlipbook> GetCurAnim() { return m_CurAnim; }

public:
    void CreateFlipbook();
    void LoadFlipbook();
    void LoadSprite();

public:
    void LoadFlipbook(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
    void SelectSprite(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
    void Save();

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

public:
    // 폴더 입력 시 자동으로 애니메이션 생성
    void AddSpriteInFolder(const wstring& _Path);




public:
    AnimEditorUI();
    ~AnimEditorUI();
};

