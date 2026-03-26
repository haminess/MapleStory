#pragma once
#include <Engine\CScript.h>

enum class ATTACK_PATTERN {
    NONE,
    GROUND_BREAK,      // 지면 뚫기
    FIRE_STRIKE,       // 화염구
    WIDE_GROUND_BREAK, // 광역 지면 뚫기
    POISON_SPRAY,      // 독 분사
    GIGA_BREATH,       // 기가 브레스
    TAIL_ATTACK        // 꼬리 공격 (카오스 모드)
};

enum class BELLUM_PHASE {
    PHASE_1 = 1,   // 100% - 80%
    PHASE_2,       // 80% - 60%
    PHASE_3,       // 60% - 40%
    PHASE_4        // 40% - 0%
};

struct StalactiteInfo {
    Vector2 position;
    float fallTime;
    bool isActive;
};

class CBellumScript : public CScript {
private:
    // 기본 스탯
    float m_fMaxHP;
    float m_fCurrentHP;
    bool m_bChaosMode;
    BELLUM_PHASE m_eCurrentPhase;

    // 상태 머신
    CStateMachine* m_pStateMachine;

    // 패턴 관련
    vector<Vector2> m_vTailPositions;  // 꼬리 위치들
    vector<StalactiteInfo> m_vStalactites;  // 종유석 정보
    float m_fStalactiteSpawnTimer;

    // 카오스 모드 전용
    float m_fBreathWindForce;
    vector<CGameObject*> m_vTailObjects;

    // 전투 관련
    float m_fInvincibleTime;
    bool m_bIsInvincible;
    Vector2 m_vTargetPosition;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    void InitializeStates();
    void UpdatePhase();
    void SpawnStalactites();
    //void UpdateStalactites();

    void TakeDamage(float _fDamage);
    void SetChaosMode(bool _bChaos) { m_bChaosMode = _bChaos; }
    bool IsChaosMode() const { return m_bChaosMode; }

    float GetCurrentHP() const { return m_fCurrentHP; }
    float GetMaxHP() const { return m_fMaxHP; }
    BELLUM_PHASE GetCurrentPhase() const { return m_eCurrentPhase; }

    //// 패턴 실행 함수들
    //void ExecuteGroundBreak();
    //void ExecuteFireStrike();
    //void ExecuteWideGroundBreak();
    //void ExecutePoisonSpray();
    //void ExecuteGigaBreath();
    //void ExecuteTailAttack();

    void CreateWarningCircle(const Vector2& _pos, float _radius, float _duration);
    Vector2 GetRandomPositionInMap();

public:
    CLONE(CBellumScript);
    CBellumScript();
    ~CBellumScript();
};