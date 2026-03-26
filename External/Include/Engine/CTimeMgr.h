#pragma once

class CTimeMgr
	: public singleton<CTimeMgr>
{
	SINGLE(CTimeMgr);

	// 시간
	// 게임 프레임 1초당 13만 프레임
	// 한 프레임 당 걸리는 시간 13만분의 1초
	// GetTickCount, 1초에 1000번 카운트
	// GetTickCount 한번 카운트할 때 130번 프레임 카운트
	// 따라서 GetTickCount로는 0.001초 보다 정밀해질 수 없음

	// 1. 시간 동기화를 위해 DT(DeltaTime) 을 업데이트하고 관리

private:
	// system time
	LARGE_INTEGER	m_Frequency;	// 1초당 카운팅 가능 수
	LARGE_INTEGER	m_CurrentCount;	// 현재 카운팅
	LARGE_INTEGER	m_PrevCount;	// 이전 프레임 카운팅

	// game time
	// Delta Time
	// 미세한 시간값, 한 프레임당 시간
	// 항상 달라질 수 있다.
	float			m_fDT;			// DeltaTime : 1프레임에 걸린 시간
	float			m_Time;			// 누적 시간

	float			m_fEngineDT;
	float			m_fEngineTime;

	double			m_Second;	// 1초 체크 용도 누적시간

	UINT			m_FPS;

	bool			m_IsStop;

	wchar_t			m_TimeInfo[255];

public:
	void Init();
	void Tick();
	void Render();
	void SetStopMode(bool _IsStop)
	{
		m_IsStop = _IsStop;
		if (m_IsStop)
			m_fDT = 0.f;
	}

public:
	float GetDeltaTime() { return m_fDT; }
	float GetEngineDeltaTime() { return m_fEngineDT; }
	float GetTime() { return m_Time; }
	float GetEngineTime() { return m_fEngineTime; }
};

