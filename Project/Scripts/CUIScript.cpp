#include "pch.h"
#include "CUIScript.h"

#include <Engine\CKeyMgr.h>

CUIScript::CUIScript(UINT _Type)
    : CScript(_Type)
    , m_bVisible(true)
    , m_bInteractable(true)
    , m_bHovered(false)
    , m_bPressed(false)
    , m_iSortOrder(0)
{
    AddScriptParam({ SCRIPT_PARAM::BOOL, "Visible", &m_bVisible });
    AddScriptParam({ SCRIPT_PARAM::BOOL, "Interactable", &m_bInteractable });
    AddScriptParam({ SCRIPT_PARAM::BOOL, "Hovered", &m_bHovered });
    AddScriptParam({ SCRIPT_PARAM::BOOL, "Pressed", &m_bPressed });
    AddScriptParam({ SCRIPT_PARAM::INT, "SortOrder",&m_iSortOrder });
}

CUIScript::CUIScript(const CUIScript& _Other)
    : CScript(_Other)
    , m_bVisible(_Other.m_bVisible)
    , m_bInteractable(_Other.m_bInteractable)
    , m_bHovered(false)
    , m_bPressed(false)
    , m_iSortOrder(_Other.m_iSortOrder)
{
    ClearScriptParam();

    AddScriptParam({ SCRIPT_PARAM::BOOL, "Visible", &m_bVisible });
    AddScriptParam({ SCRIPT_PARAM::BOOL, "Interactable", &m_bInteractable });
    AddScriptParam({ SCRIPT_PARAM::BOOL, "Hovered", &m_bHovered });
    AddScriptParam({ SCRIPT_PARAM::BOOL, "Pressed", &m_bPressed });
    AddScriptParam({ SCRIPT_PARAM::INT, "SortOrder",&m_iSortOrder });
}

CUIScript::~CUIScript()
{
}

void CUIScript::Init()
{
}

void CUIScript::Tick()
{
    if (!m_bVisible)
        return;

    Tick_UI();

    if (!m_bInteractable)
        return;

    bool bMouseOn = IsMouseHovered();

    if (bMouseOn && !m_bHovered)
    {
        m_bHovered = true;
        OnMouseEnter();
    }
    else if (!bMouseOn && m_bHovered)
    {
        m_bHovered = false;
        OnMouseExit();
    }

    if (bMouseOn && KEY_TAP(KEY::LBTN))
    {
        m_bPressed = true;
        OnMouseDown();
    }

    if (m_bPressed && KEY_PRESSED(KEY::LBTN))
    {
        OnMousePressed();
    }

    if (m_bPressed && KEY_RELEASED(KEY::LBTN))
    {
        OnMouseUp();

        if (bMouseOn)
        {
            OnClick();
        }

        m_bPressed = false;
    }
}

void CUIScript::SaveComponent(FILE* _File)
{
    fwrite(&m_bVisible, sizeof(bool), 1, _File);
    fwrite(&m_bInteractable, sizeof(bool), 1, _File);
    fwrite(&m_iSortOrder, sizeof(int), 1, _File);
}

void CUIScript::LoadComponent(FILE* _FILE)
{
    fread(&m_bVisible, sizeof(bool), 1, _FILE);
    fread(&m_bInteractable, sizeof(bool), 1, _FILE);
    fread(&m_iSortOrder, sizeof(int), 1, _FILE);
}

void CUIScript::SetVisible(bool _Visible)
{
    m_bVisible = _Visible;

    if (!m_bVisible)
    {
        m_bHovered = false;
        m_bPressed = false;
    }
}

bool CUIScript::IsVisible() const
{
    return m_bVisible;
}

void CUIScript::SetInteractable(bool _Interactable)
{
    m_bInteractable = _Interactable;

    if (!m_bInteractable)
        m_bPressed = false;
}

bool CUIScript::IsInteractable() const
{
    return m_bInteractable;
}

void CUIScript::SetUIPos(Vector2 _Pos)
{
    Vector3 vPos = Transform()->GetWorldPos();
    vPos.x = _Pos.x;
    vPos.y = _Pos.y;

    Transform()->SetWorldPos(vPos);
}

Vector2 CUIScript::GetUIPos()
{
    Vector3 vPos = GetOwner()->Transform()->GetWorldPos();

    return Vector2(vPos.x, vPos.y);
}

void CUIScript::SetUIScale(Vector2 _Scale)
{
    Transform()->SetRelativeScale(Vector3(_Scale.x, _Scale.y, 1.f));
}

Vector2 CUIScript::GetUIScale()
{
    Vector3 vScale = Transform()->GetRelativeScale();

    return Vector2(vScale.x, vScale.y);
}

void CUIScript::SetSortOrder(int _Order)
{
    m_iSortOrder = _Order;
}

int CUIScript::GetSortOrder() const
{
    return m_iSortOrder;
}

bool CUIScript::IsMouseHovered()
{
    Vector3 vMousePos = CKeyMgr::GetInst()->GetMouseWorldPos();

    Vector3 vPos = Transform()->GetWorldPos();
    Vector3 vScale = Transform()->GetRelativeScale();

    float fLeft = vPos.x - vScale.x * 0.5f;
    float fRight = vPos.x + vScale.x * 0.5f;
    float fTop = vPos.y + vScale.y * 0.5f;
    float fBottom = vPos.y - vScale.y * 0.5f;

    return fLeft <= vMousePos.x && vMousePos.x <= fRight
        && fBottom <= vMousePos.y && vMousePos.y <= fTop;
}