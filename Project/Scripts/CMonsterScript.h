#pragma once
#include "CLifeScript.h"

enum struct MON_ANIM
{
    IDLE,
    MOVE,
    ATTACK,
    TRACE,
    HIT,
    DIE,
    SPAWN,
    END,
};

class CPlatformerScript;
class CMonsterScript :
    public CLifeScript
{
private:
    Ptr<CFlipbook> m_Flipbook[(UINT)MON_ANIM::END];
    CPlatformerScript* m_Platformer;
    
    float   m_fStateChangeTime;    // 상태 변경 주기
    float   m_fAccTime;            // 누적 시간

    int             m_HitPower;
    Ptr<CFlipbook>  m_HitAnim;
    int             m_HitCount;

public:    
    void AddFlipbook(MON_ANIM _AnimType, Ptr<CFlipbook> _Flipbook) { m_Flipbook[(UINT)_AnimType] = _Flipbook; }

public:
    void Hit(int _Power, int _Count, Ptr<CFlipbook> _HitEff, Vector2 _Dir);
    void HitTimer();


public:
    virtual void Damage(int _Power, int _Count = 0) override;
    virtual void Dead() override;

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;


    void RandomState();
    int RandomDirection();

public:
    CLONE(CMonsterScript);
    CMonsterScript();
    CMonsterScript(UINT _Type);
    ~CMonsterScript();
};

