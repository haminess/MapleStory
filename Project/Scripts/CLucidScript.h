#pragma once
#include <Engine\CScript.h>

class CSkillScript;
class CLucidLittleButterflyScript;
class CLucidLittleButterflySpawnScript;
class CLucidHornScript;
class CLucidScript :
    public CScript
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

    // little butterfly & horn
    CLucidLittleButterflySpawnScript* m_LittleButterfly;


public:
    void SpawnNightmareButterfly();
    void TeleportPlayer();
    void SpawnGolem();
    void SpawnToadstool();

    void SpawnLittleButterfly();

public:
    virtual void Init() override;
    virtual void Begin() override;
    virtual void Tick() override;

public:
    CLONE(CLucidScript);
    CLucidScript();
    CLucidScript(const CLucidScript& _Other);
    ~CLucidScript();
};

