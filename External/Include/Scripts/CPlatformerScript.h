#pragma once
#include<Engine/CScript.h>
#include "CPlatformScript.h"

class CPlatformerScript :
    public CScript
{
private:
    CPlatformScript*    m_Platform;
    Vector3             m_NextVelocity;

public:
	CPlatformScript* GetCurPlatform() { return m_Platform; }


    virtual void Tick();

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

public:
    CLONE(CPlatformerScript);
    CPlatformerScript();
    CPlatformerScript(const CPlatformerScript& _Other);
    ~CPlatformerScript();
};

