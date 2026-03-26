#pragma once
#include "CRenderComponent.h"

#include "assets.h"

class CMeshRender :
    public CRenderComponent
{
public:
    virtual void Init() override;
    virtual void Render() override;
    virtual void FinalTick() override;

public:
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _FILE) override;

public:
    CLONE(CMeshRender);
    CMeshRender();
    CMeshRender(const CMeshRender& _Other);
    ~CMeshRender();
};

