#pragma once
#include <Engine\CScript.h>

class CUIScript :
    public CScript
{
private:
    bool    m_bVisible;
    bool    m_bInteractable;

    bool    m_bHovered;
    bool    m_bPressed;

    int     m_iSortOrder;

public:
    virtual void Init() override;
    virtual void Tick() override final;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

protected:
    virtual void Tick_UI() {}

public:
    void SetVisible(bool _Visible);
    bool IsVisible() const;

    void SetInteractable(bool _Interactable);
    bool IsInteractable() const;

    void SetUIPos(Vector2 _Pos);
    Vector2 GetUIPos();

    void SetUIScale(Vector2 _Scale);
    Vector2 GetUIScale();

    void SetSortOrder(int _Order);
    int GetSortOrder() const;

public:
    bool IsMouseHovered();

protected:
    virtual void OnMouseEnter() {}
    virtual void OnMouseExit() {}
    virtual void OnMouseDown() {}
    virtual void OnMousePressed() {}
    virtual void OnMouseUp() {}
    virtual void OnClick() {}

public:
    CLONE(CUIScript);
    CUIScript(UINT _Type = (UINT)SCRIPT_TYPE::UISCRIPT);
    CUIScript(const CUIScript& _Other);
    ~CUIScript();
};