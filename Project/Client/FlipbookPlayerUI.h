
#pragma once
#include "ComponentUI.h"

class CFlipbook;
class FlipbookPlayerUI :
    public ComponentUI
{
private:
    int m_CurFrameIdx;
    int m_Selected;

public:
    virtual void Render_Update() override;

private:
    void SelectFlipbook(DWORD_PTR _ListUI, DWORD_PTR _SelectString);

public:
    FlipbookPlayerUI();
    ~FlipbookPlayerUI();
};