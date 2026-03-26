#pragma once
#include "CCollider2D.h"
class CLineCollider2D :
	public CCollider2D
{
private:
	float m_Slope;          // 기울기
	Vector2 m_StartPos;     // 시작 위치
	Vector2 m_EndPos;       // 끝 위치

public:
	float   GetSlope() { return m_Slope; }
	Vector2 GetStartPos() { return m_StartPos; }
	Vector2 GetEndPos() { return m_EndPos; }

	void	SetSlope(float   _Slope) { m_Slope = _Slope; }
	void	SetStartPos(Vector2 _StartPos) { m_StartPos = _StartPos; }
	void	SetEndPos(Vector2 _EndPos) { m_EndPos = _EndPos; }

public:
	virtual void FinalTick() override;

public:
	CLineCollider2D();
	~CLineCollider2D();
};

