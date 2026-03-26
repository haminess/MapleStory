#pragma once
#include <Engine\CScript.h>
class CDamageSkinScript :
    public CScript
{
private:
    int             m_Damage;
    Ptr<CTexture>   m_TexArray;

    Vector3     m_vStartPos;     // 시작 위치
    float       m_fSpeed;        // 상승 속도
    float       m_fLifeTime;     // 수명
    float       m_fCurTime;      // 현재 시간


public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void SaveComponent(FILE* _File) override;
    virtual void LoadComponent(FILE* _File) override;

public:
    CLONE(CDamageSkinScript);
    CDamageSkinScript(UINT _Type = (UINT)SCRIPT_TYPE::DAMAGESKINSCRIPT);
    CDamageSkinScript(const CDamageSkinScript& _Other);
    ~CDamageSkinScript();
};

