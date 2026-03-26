#include "pch.h"
#include "CProjectileScript.h"


CProjectileScript::CProjectileScript()
	: CScript(SCRIPT_TYPE::PROJECTILESCRIPT)\
	, m_Speed(500.f)
{
}

CProjectileScript::~CProjectileScript()
{
}

void CProjectileScript::Begin()
{
}

void CProjectileScript::Tick()
{
	if(m_UseDir)
		GetOwner()->RigidBody2D()->SetVelocity(m_Dir * m_Speed);

	if (m_UseDest)
	{
		m_DestRatio += DT * m_DestTimer;
		Vector3 vNextPos = Vector3::Lerp(m_PrevPos, m_DestPos, m_DestRatio / m_DestTimer);
		GetOwner()->Transform()->SetWorldPos(vNextPos);

		if (m_DestRatio >= 1.f)
		{
			DestroyObject(GetOwner());
			return;
		}
	}

	if (m_Timer > 0.f)
	{
		m_ElapsedTime += DT;
		if (m_ElapsedTime >= m_Timer)
		{
			DestroyObject(GetOwner());
			return;
		}
	}
}

void CProjectileScript::SaveComponent(FILE* _File)
{
}

void CProjectileScript::LoadComponent(FILE* _File)
{
}

void CProjectileScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (m_DelInst && m_Del_Begin) (m_DelInst->*m_Del_Begin)();
	if (m_Del1Inst && m_Del1_Begin) (m_Del1Inst->*m_Del1_Begin)((DWORD_PTR)_OtherCollider);
	if (m_Del2Inst && m_Del2_Begin) (m_Del2Inst->*m_Del2_Begin)((DWORD_PTR)_Collider, (DWORD_PTR)_OtherCollider);
}

void CProjectileScript::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (m_DelInst && m_Del_Over) (m_DelInst->*m_Del_Over)();
	if (m_Del1Inst && m_Del1_Over) (m_Del1Inst->*m_Del1_Over)((DWORD_PTR)_OtherCollider);
	if (m_Del2Inst && m_Del2_Over) (m_Del2Inst->*m_Del2_Over)((DWORD_PTR)_Collider, (DWORD_PTR)_OtherCollider);
}

void CProjectileScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (m_DelInst && m_Del_End) (m_DelInst->*m_Del_End)();
	if (m_Del1Inst && m_Del1_End) (m_Del1Inst->*m_Del1_End)((DWORD_PTR)_OtherCollider);
	if (m_Del2Inst && m_Del2_End) (m_Del2Inst->*m_Del2_End)((DWORD_PTR)_Collider, (DWORD_PTR)_OtherCollider);
}
