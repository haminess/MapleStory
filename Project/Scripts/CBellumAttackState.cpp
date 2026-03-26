#include "pch.h"
#include "CBellumScript.h"
#include "CBellumAttackState.h"


CBellumAttackState::CBellumAttackState()
    : CState(SCRIPT_TYPE::BELLUMATTACKSTATE),
    m_fAttackTime(0.f), m_fAttackDuration(0.f),
    m_iCurrentPhase(1), m_currentPattern(ATTACK_PATTERN::NONE)
{
}

CBellumAttackState::~CBellumAttackState()
{
}


void CBellumAttackState::Enter() {
    m_fAttackTime = 0.f;
    SelectNextPattern();
}

void CBellumAttackState::Tick() {
    m_fAttackTime += DT;

    if (m_fAttackTime >= m_fAttackDuration) {
        GetStateMachine()->ChangeState(L"Idle");
        return;
    }

    //ExecutePattern();
}

void CBellumAttackState::SelectNextPattern() {
    CBellumScript* pBellum = GetOwner<CBellumScript>();
    float hpRatio = pBellum->GetCurrentHP() / pBellum->GetMaxHP();

    vector<ATTACK_PATTERN> availablePatterns;

    // 기본 패턴들
    availablePatterns.push_back(ATTACK_PATTERN::GROUND_BREAK);

    // 페이즈에 따른 패턴 추가
    if (hpRatio <= 0.8f)
        availablePatterns.push_back(ATTACK_PATTERN::FIRE_STRIKE);
    if (hpRatio <= 0.6f) {
        availablePatterns.push_back(ATTACK_PATTERN::WIDE_GROUND_BREAK);
        availablePatterns.push_back(ATTACK_PATTERN::POISON_SPRAY);
    }
    if (hpRatio <= 0.4f)
        availablePatterns.push_back(ATTACK_PATTERN::GIGA_BREATH);

    // 카오스 모드 전용 패턴
    if (pBellum->IsChaosMode())
        availablePatterns.push_back(ATTACK_PATTERN::TAIL_ATTACK);

    // 랜덤하게 패턴 선택
    int patternIdx = rand() % availablePatterns.size();
    m_currentPattern = availablePatterns[patternIdx];

    // 패턴별 지속시간 설정
    switch (m_currentPattern) {
    case ATTACK_PATTERN::GROUND_BREAK: m_fAttackDuration = 1.5f; break;
    case ATTACK_PATTERN::FIRE_STRIKE: m_fAttackDuration = 3.0f; break;
    case ATTACK_PATTERN::WIDE_GROUND_BREAK: m_fAttackDuration = 2.0f; break;
    case ATTACK_PATTERN::POISON_SPRAY: m_fAttackDuration = 4.0f; break;
    case ATTACK_PATTERN::GIGA_BREATH: m_fAttackDuration = 5.0f; break;
    case ATTACK_PATTERN::TAIL_ATTACK: m_fAttackDuration = 2.5f; break;
    }
}
