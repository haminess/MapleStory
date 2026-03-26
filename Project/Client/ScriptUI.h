#pragma once
#include "ComponentUI.h"

#include <Engine/CScript.h>

class ScriptUI :
    public ComponentUI
{
private:
    CScript* m_TargetScript;
    
    void*   m_Value;
    CComponent*         m_SetInst;
    EUI_DELEGATE_1      m_SetterFunc;

public:
    void SetTargetScript(CScript* _Script) { m_TargetScript = _Script; }
    void SetScript(CScript* _Script);

public:
    virtual void Render_Update() override;

private:
    void SelectTexture(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
    void Render_Property();
    void Render_Param();

public:
    ScriptUI();
    ~ScriptUI();
};

