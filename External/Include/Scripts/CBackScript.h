#pragma once
#include <Engine\CScript.h>
class CBackScript :
    public CScript
{
    virtual void Tick() override {}
    virtual void SaveComponent(FILE* _File) override {}
    virtual void LoadComponent(FILE* _File) override {}

public:
    CLONE(CBackScript);
    CBackScript();
    CBackScript(const CBackScript& _Other);
    ~CBackScript();
};

