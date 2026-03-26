#include "pch.h"
#include "CBellumScript.h"
#include <Engine/CStateMachine.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CRigidBody2D.h>
//#include "CWarningCircle.h"
#include "CBellumIdleState.h"
#include "CBellumAttackState.h"

CBellumScript::CBellumScript()
    : CScript(SCRIPT_TYPE::BELLUMSCRIPT)
    , m_fMaxHP(m_bChaosMode ? 200000000000.f : 550000000.f)  // 카오스: 2000억, 노멀: 5.5억
    , m_fCurrentHP(m_fMaxHP)
    , m_bChaosMode(false)
    , m_eCurrentPhase(BELLUM_PHASE::PHASE_1)
    , m_fStalactiteSpawnTimer(0.f)
    , m_fBreathWindForce(200.f)
    , m_fInvincibleTime(0.f)
    , m_bIsInvincible(false)
{
}

CBellumScript::~CBellumScript()
{
    for (auto& obj : m_vTailObjects)
    {
        if (nullptr != obj)
            delete obj;
    }
}

void CBellumScript::Begin()
{
    // StateMachine 컴포넌트 추가 및 상태 초기화
    m_pStateMachine = GetOwner()->StateMachine();
    InitializeStates();

    // 종유석 풀 초기화
    m_vStalactites.resize(20);
    for (auto& stalactite : m_vStalactites)
    {
        stalactite.isActive = false;
        stalactite.fallTime = 0.f;
    }

    // 카오스 모드일 경우 꼬리 오브젝트 생성
    if (m_bChaosMode)
    {
        for (int i = 0; i < 6; ++i)
        {
            //CGameObject* pTailObj = new CGameObject;
            //pTailObj->SetName(L"BellumTail_" + std::to_wstring(i));
            //pTailObj->AddComponent<CCollider2D>(new CCollider2D);
            //pTailObj->SetActive(false);
            //m_vTailObjects.push_back(pTailObj);
        }
    }
}

void CBellumScript::Tick()
{
    // 무적 시간 처리
    if (m_bIsInvincible)
    {
        m_fInvincibleTime -= DT;
        if (m_fInvincibleTime <= 0.f)
        {
            m_bIsInvincible = false;
            m_fInvincibleTime = 0.f;
        }
    }

    // 페이즈 업데이트
    UpdatePhase();

    // 종유석 시스템 업데이트
    m_fStalactiteSpawnTimer += DT;
    if (m_fStalactiteSpawnTimer >= (m_bChaosMode ? 5.f : 20.f))  // 카오스: 5초, 노멀: 20초
    {
        SpawnStalactites();
        m_fStalactiteSpawnTimer = 0.f;
    }
    //UpdateStalactites();
}

void CBellumScript::InitializeStates()
{
    m_pStateMachine->AddState(L"Idle", new CBellumIdleState());
    m_pStateMachine->AddState(L"Attack", new CBellumAttackState());
    m_pStateMachine->ChangeState(L"Idle");
}

void CBellumScript::UpdatePhase()
{
    float hpRatio = m_fCurrentHP / m_fMaxHP;
    BELLUM_PHASE newPhase;

    if (hpRatio > 0.8f) newPhase = BELLUM_PHASE::PHASE_1;
    else if (hpRatio > 0.6f) newPhase = BELLUM_PHASE::PHASE_2;
    else if (hpRatio > 0.4f) newPhase = BELLUM_PHASE::PHASE_3;
    else newPhase = BELLUM_PHASE::PHASE_4;

    if (m_eCurrentPhase != newPhase)
    {
        m_eCurrentPhase = newPhase;
        // 페이즈 전환 효과 및 처리
    }
}

void CBellumScript::SpawnStalactites()
{
    const int COUNT = 5;
    for (int i = 0; i < COUNT; ++i)
    {
        // 비활성화된 종유석 찾기
        for (auto& stalactite : m_vStalactites)
        {
            if (!stalactite.isActive)
            {
                stalactite.position = GetRandomPositionInMap();
                stalactite.fallTime = 2.f;  // 경고 시간
                stalactite.isActive = true;

                // 경고 이펙트 생성
                CreateWarningCircle(stalactite.position, 30.f, 2.f);
                break;
            }
        }
    }
}

void CBellumScript::TakeDamage(float _fDamage)
{
    if (m_bIsInvincible)
        return;

    m_fCurrentHP = max(0.f, m_fCurrentHP - _fDamage);

    // 사망 처리
    if (m_fCurrentHP <= 0.f)
    {
        // 사망 처리 로직
        DestroyObject(GetOwner());
    }
}

void CBellumScript::CreateWarningCircle(const Vector2& _pos, float _radius, float _duration)
{
    //CGameObject* pWarning = new CGameObject;
    //pWarning->AddComponent<CWarningCircle>(_pos, _radius, _duration);
    //CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    //pCurLevel->AddObject((int)LAYER_INDEX::EFFECT, pWarning, true);
}

Vector2 CBellumScript::GetRandomPositionInMap()
{
    // 맵 크기에 따라 조정 필요
    float x = (float)(rand() % 1000);
    float y = (float)(rand() % 600);
    return Vector2(x, y);
}