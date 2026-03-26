#include "pch.h"
#include "CRigidBody2D.h"
#include "CTimeMgr.h"
#include "CTransform.h"

CRigidBody2D::CRigidBody2D()
	: CComponent(COMPONENT_TYPE::RIGIDBODY2D)
	, m_Mode(RIGIDBODY_MODE::PLATFOMER)
	, m_MaxSpeed(0.f)
	, m_IsMove(false)
	, m_Friction(1000.f)
	, m_UseGravity(true)
	, m_GravityAccel(1500.f)
	, m_JumpSpeed(650.f)
	, m_IsGround(false)
	, m_Mass(1.f)
	, m_GravityMaxSpeed(3000.f)
	, m_GravityVelocity(0.f)

{
}

CRigidBody2D::CRigidBody2D(const CRigidBody2D& _Origin)
	: CComponent(_Origin)
	, m_Mode(_Origin.m_Mode)
	, m_Mass(_Origin.m_Mass)
	, m_Friction(_Origin.m_Friction)
	, m_MaxSpeed(_Origin.m_MaxSpeed)
	, m_IsGround(false)
	, m_UseGravity(_Origin.m_UseGravity)
	, m_GravityVelocity(0.f)
	, m_GravityAccel(_Origin.m_GravityAccel)
	, m_GravityMaxSpeed(_Origin.m_GravityMaxSpeed)
	, m_JumpSpeed(_Origin.m_JumpSpeed)
{
}

CRigidBody2D::~CRigidBody2D()
{
}


void CRigidBody2D::SaveComponent(FILE* _File)
{
	fwrite(&m_Mode, sizeof(RIGIDBODY_MODE), 1, _File);
	fwrite(&m_Mass, sizeof(float), 1, _File);
	fwrite(&m_Friction, sizeof(float), 1, _File);
	fwrite(&m_MaxSpeed, sizeof(float), 1, _File);
	fwrite(&m_GravityForce, sizeof(Vector3), 1, _File);
	fwrite(&m_GravityAccel, sizeof(Vector3), 1, _File);
	fwrite(&m_GravityMaxSpeed, sizeof(float), 1, _File);
	fwrite(&m_UseGravity, sizeof(bool), 1, _File);
	fwrite(&m_JumpSpeed, sizeof(float), 1, _File);
}

void CRigidBody2D::LoadComponent(FILE* _File)
{
	fread(&m_Mode, sizeof(RIGIDBODY_MODE), 1, _File);
	fread(&m_Mass, sizeof(float), 1, _File);
	fread(&m_Friction, sizeof(float), 1, _File);
	fread(&m_MaxSpeed, sizeof(float), 1, _File);
	fread(&m_GravityForce, sizeof(Vector3), 1, _File);
	fread(&m_GravityAccel, sizeof(Vector3), 1, _File);
	fread(&m_GravityMaxSpeed, sizeof(float), 1, _File);
	fread(&m_UseGravity, sizeof(bool), 1, _File);
	fread(&m_JumpSpeed, sizeof(float), 1, _File);
}



void CRigidBody2D::FinalTick()
{
	// 가속도 구하기
	// F == M x A
	// F / M == A
	Vector3 Accel = m_Force / m_Mass;

	// 가속도 방향으로 가속도의 크기만큼 속도가 증가함
	m_Velocity += Accel * DT;

	// 최대속력 체크, 0.f == m_MaxSpeed ==> 속력 제한이 없다
	if (0.f != m_MaxSpeed)
	{
		if (m_MaxSpeed < m_Velocity.Length())
		{
			m_Velocity.Normalize();
			m_Velocity *= m_MaxSpeed;
		}
	}

	// 주어진 힘이 없다면, 반대방향으로 마찰력을 준다.
	if (m_Force == Vector3(0.f, 0.f, 0.f))
	{
		if(m_Velocity != Vector3(0.f, 0.f, 0.f))
			ApplyFriction(m_Velocity);

		if(m_ImpulseVelocity != Vector3(0.f, 0.f, 0.f))
			ApplyFriction(m_ImpulseVelocity);
	}


	Vector3 vFinalVelocity = m_Velocity + m_ImpulseVelocity;

	// RigidBody 가 Plarformer 모드인경우
	if (m_Mode == RIGIDBODY_MODE::PLATFOMER)
	{
		// 중력기능 On
		if (m_UseGravity && !m_IsGround)
		{
			m_GravityVelocity += Vector3(0.f, -1.f, 0.f) * m_GravityAccel * DT;
			if (m_GravityMaxSpeed < m_GravityVelocity.Length())
			{
				m_GravityVelocity.Normalize();
				m_GravityVelocity *= m_GravityMaxSpeed;
			}

			vFinalVelocity += m_GravityVelocity;
		}
		// 중력기능 Off
		else
		{
			m_GravityVelocity = Vector3(0.f, 0.f, 0.f);
		}
	}


	// 속도에 따른 물체의 이동 발생
	Vector3 vObjPos = GetOwner()->Transform()->GetWorldPos();
	m_FinalVelocity = vFinalVelocity * DT;
	GetOwner()->Transform()->SetWorldPos(vObjPos + m_FinalVelocity);

	// 오브젝트의 움직임 체크
	if (vObjPos == GetOwner()->Transform()->GetWorldPos())
		m_IsMove = false;
	else
		m_IsMove = true;

	// 이번에 받은 힘을 0 으로 초기화
	m_Force = Vector3(0.f, 0.f, 0.f);

	// 속도의 방향을 시각화
	DrawDebugLine(Vector4(1.f, 1.f, 0.f, 1.f), vObjPos, vObjPos + m_Velocity);
}

void CRigidBody2D::SetGround(bool _Ground)
{
	m_IsGround = _Ground;

	if (m_IsGround)
	{
		m_GravityVelocity = Vector3(0.f, 0.f, 0.f);
	}
}

void CRigidBody2D::Jump()
{
	m_GravityVelocity.y = 0.f;
	m_GravityVelocity += (Vector3(0.f, 1.f, 0.f) * m_JumpSpeed);
	m_IsGround = false;
}

void CRigidBody2D::Jump(Vector3 _Dir)
{
	m_GravityVelocity.y = 0.f;
	m_GravityVelocity += (_Dir * m_JumpSpeed);
	m_IsGround = false;
}

void CRigidBody2D::Jump(Vector3 _Dir, float _Power)
{
	m_GravityVelocity.y = 0.f;
	m_GravityVelocity += (_Dir * _Power);
	m_IsGround = false;
}

void CRigidBody2D::ApplyFriction(Vector3& _Velocity)
{
	Vector3 vFriction = -_Velocity;
	vFriction.Normalize();
	vFriction *= m_Friction * m_Mass * DT;

	if (_Velocity.Length() < vFriction.Length())
	{
		_Velocity = Vector3(0.f, 0.f, 0.f);
	}
	else
	{
		_Velocity += vFriction;
	}
}
