#pragma once
#include "EditorUI.h"

#include <Engine/CGameObject.h>

class ComponentUI :
    public EditorUI
{
private:
    CGameObject* m_TargetObject;
    const COMPONENT_TYPE  m_Type;

public:
    void SetTargetObject(CGameObject* _Target);
    CGameObject* GetTargetObject() { return m_TargetObject; }

protected:
    void ComponentTitle(const string& _title);

private:
    void ComponentContext();
    void SelectComponent(DWORD_PTR, DWORD_PTR);
    void SelectScript(DWORD_PTR, DWORD_PTR);

public:
    ComponentUI(const string& _ID, COMPONENT_TYPE _Type);
    ComponentUI(const string& _ID);
    ~ComponentUI();
};

