#pragma once
#include "ComponentUI.h"

class ListUI;
class MeshRenderUI :
    public ComponentUI
{
private:
    TEX_PARAM m_TexParam;


public:
    virtual void Render_Update() override;

private:
    void SelectMesh(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
    void SelectMaterial(DWORD_PTR _ListUI, DWORD_PTR _SelectString);
    void SelectTexture(DWORD_PTR _ListUI, DWORD_PTR _SelectString);



public:
    MeshRenderUI();
    ~MeshRenderUI();
};

