
#pragma once
#include "ComponentUI.h"

class CFlipbook;
class StateMachineUI :
    public ComponentUI
{
private:

public:
    virtual void Render_Update() override;

private:
    void SelectState(DWORD_PTR _ListUI, DWORD_PTR _SelectString);

public:
    StateMachineUI();
    ~StateMachineUI();
};