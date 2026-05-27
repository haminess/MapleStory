#pragma once
#include "CUIScript.h"
class CFont;
class CTextUI :
    public CUIScript
{
private:
    tTextInfo m_Info;
    wstring m_Font;
    Vector4 m_Color;

public:
    virtual void Tick_UI() override;

    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    void SetText(const wstring& _Text) { m_Info.Text = _Text; }
    const wstring& GetText() const { return m_Info.Text; }

    void SetFont(const wstring& _Font);
    const Ptr<CFont> GetFont() const { return m_Info.Font; }

    void SetFontSize(float _Size) { m_Info.FontSize = _Size; }
    float GetFontSize() const { return m_Info.FontSize; }

    void SetColor(UINT _Color) { m_Info.Color = _Color; }
    UINT GetColor() const { return m_Info.Color; }

public:
    void LoadFont()
    {
        SetFont(m_Font);
    }

public:
    CLONE(CTextUI);
    CTextUI(UINT _Type = (UINT)SCRIPT_TYPE::TEXTUI);
    CTextUI(const CTextUI& _Other);
    ~CTextUI();
};

