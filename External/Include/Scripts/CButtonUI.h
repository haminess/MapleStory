#pragma once
#include "CUIScript.h"
class CButtonUI :
    public CUIScript
{
public:
    void Tick_UI() override;

public:
    CLONE(CButtonUI);
    CButtonUI(UINT _Type = (UINT)SCRIPT_TYPE::BUTTONUI);
    CButtonUI(const CButtonUI& _Other);
    ~CButtonUI();
};

