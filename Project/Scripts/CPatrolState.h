#pragma once
#include <Engine/CState.h>
class CPatrolState :
	public CState
{
private:
	float m_fPatrolTime;      // 현재 패트롤 진행 시간
	float m_fPatrolDuration;  // 한 방향으로 이동할 시간
	float m_fMovementSpeed;   // 이동 속도
	int m_iDirection;         // 이동 방향 (-1 또는 1)

public:
	void SetDirection(int _iDir) { m_iDirection = _iDir; }
	int GetDirection() { return m_iDirection; }

public:
	virtual void Enter();
	virtual void Tick();
	virtual void Exit();


public:
	CLONE(CPatrolState);
	CPatrolState();
	~CPatrolState();
};

