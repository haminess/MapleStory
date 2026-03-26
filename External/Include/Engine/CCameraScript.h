#pragma once
#include "CScript.h"

class CCameraScript :
    public CScript
{
private:
    float   m_CameraSpeed;

public:
    virtual void Tick() override;

private:
    void CameraOrthographicMove();
    void CameraPerspectiveMove();

public:
    CCameraScript();
    ~CCameraScript();
};

