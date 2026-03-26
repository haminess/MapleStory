#pragma once
#include "AssetUI.h"
class MaterialUI :
    public AssetUI
{
private:
    TEX_PARAM   m_OpenTexType;


public:
    virtual void Render_Update() override;

private:
    void ShaderParameter();
    void SelectGraphicShader(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
    void SelectTexture(DWORD_PTR _ListUI, DWORD_PTR _SelectString);



public:
    MaterialUI();
    ~MaterialUI();
};

