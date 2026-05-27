#pragma once
#include <Engine\CScript.h>
#include "CMonsterScript.h"

class CSkillScript;
class CLucidLittleButterflyScript;
class CLucidLittleButterflySpawnScript;
class CLucidHornScript;
class CMonsterScript;
class CSliderUI;
class CLucidScript :
    public CMonsterScript
{
private:
    // Æä¾î¸®´õ½ºÆ®, µå·¡°ï, Å«³ªºñ, ÅÚ·¹Æ÷Æ®, °¨¿Á, ³­ºñ¶¼, °ñ·½, µ¶¹ö¼¸
    const float m_SkillDelay[8] = {3.f, 5.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};

    vector<CSkillScript*>               m_vecSkill;
    class CLucidFairyDustSkill*         m_FairyDustSkill;
    class CLucidDragonSkill*            m_DragonSkill;

    // Anim
    Ptr<CFlipbook> m_AnimTeleportHit;

    // pattern
    queue<CSkillScript*> m_SkillQueue;
    float m_PatternDelay;
    UINT m_TimerID;

    // little butterfly & horn
    CLucidLittleButterflySpawnScript* m_LittleButterfly;


    // UI
    CSliderUI* m_HPSlider;


public:
    void SpawnNightmareButterfly();
    void TeleportPlayer();
    void SpawnGolem();
    void SpawnToadstool();

    void SpawnLittleButterfly();

    void CastNextPattern();

    void CastLucidSkill(int _SkillNum);

    void CastFairyDust() { CastLucidSkill(0); }
    void CastSpawnDragon() { CastLucidSkill(1); }
    void CastSpawnButterfly() { CastLucidSkill(2); }
    void CastForcedTeleport() { CastLucidSkill(3); }
    void CastExplosionPrison() { CastLucidSkill(4); }
    void CastSpawnGolem() { CastLucidSkill(5); }
    void CastSpawnToadstool() { CastLucidSkill(6); }
    void End1Phase();

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Dead() override;

    virtual void BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
    {
        int a = 0;
    }
    virtual void EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
    {
        int a = 0;
    }

public:
    CLONE(CLucidScript);
    CLucidScript();
    CLucidScript(const CLucidScript& _Other);
    ~CLucidScript();
};

