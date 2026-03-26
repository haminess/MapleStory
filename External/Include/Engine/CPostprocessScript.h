#pragma once
#include "CScript.h"
class CMouseScript :
    public CScript
{
public:
    virtual void Tick() override;

public:
    CMouseScript();
    ~CMouseScript();
};

