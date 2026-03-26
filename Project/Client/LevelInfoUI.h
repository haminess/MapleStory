#pragma once
#include "EditorUI.h"
class LevelInfoUI :
    public EditorUI
{
private:

public:
    virtual void Render_Update() override;

public:
    LevelInfoUI();
    ~LevelInfoUI();
};

