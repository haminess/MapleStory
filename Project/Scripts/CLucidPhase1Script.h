#pragma once
#include <Engine\CScript.h>
class CLucidPhase1Script :
    public CScript
{
private:

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

public:
    CLONE(CLucidPhase1Script);
    CLucidPhase1Script();
    CLucidPhase1Script(const CLucidPhase1Script& _Other);
    ~CLucidPhase1Script();
};

