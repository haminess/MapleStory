#pragma once
#include "SE_Sub.h"

#include <Engine/CTexture.h>

class SE_Detail :
    public SE_Sub
{
private:
    std::thread     m_SliceThread;

    Vector2 m_SliceInput;

private:
    void Atlas();
    void Sprites();
    void Slice();

private:
    void SelectTexture(DWORD_PTR _ListUI, DWORD_PTR _SelectString);

public:
    virtual void Render_Update() override;

public:
    SE_Detail();
    ~SE_Detail();

    friend class SpriteEditor;
};

