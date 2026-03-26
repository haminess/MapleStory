#pragma once
#include <Engine\CScript.h>
class CLucidPhase2Script :
    public CScript
{
private:

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

public:
    CLONE(CLucidPhase2Script);
    CLucidPhase2Script();
    CLucidPhase2Script(const CLucidPhase2Script& _Other);
    ~CLucidPhase2Script();
};

