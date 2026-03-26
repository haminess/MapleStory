#include "pch.h"
#include "CBellumIdleState.h"

CBellumIdleState::CBellumIdleState()
	: CState(SCRIPT_TYPE::BELLUMIDLESTATE)
	, m_fUndergroundTime(0.f)
	, m_fUndergroundDuration(2.f)
{
}

CBellumIdleState::~CBellumIdleState()
{
}

void CBellumIdleState::Enter() {
	m_fUndergroundTime = 0.f;
	m_fUndergroundDuration = 2.f + (rand() % 10) / 10.f;  // 2~3초 랜덤
}

void CBellumIdleState::Tick() {
	m_fUndergroundTime += DT;

	if (m_fUndergroundTime >= m_fUndergroundDuration) {
		GetStateMachine()->ChangeState(L"Attack");
	}
}

void CBellumIdleState::Exit() {
	// 지상으로 출현 애니메이션/이펙트 재생
}
