#include "pch.h"
#include "CPlatformerScript.h"
#include "CPlatformScript.h"

CPlatformerScript::CPlatformerScript()
	: CScript(SCRIPT_TYPE::PLATFORMERSCRIPT)
	, m_Platform(nullptr)
{
}

CPlatformerScript::CPlatformerScript(const CPlatformerScript& _Other)
	: CScript(SCRIPT_TYPE::PLATFORMERSCRIPT)
{
}

CPlatformerScript::~CPlatformerScript()
{
}

void CPlatformerScript::Tick()
{
	if (m_Platform)
	{
		// 플레이어 velocity를 플랫폼과 투영시켜 이동방향을 변경
		Vector3 velo = RigidBody2D()->GetVelocity();
		float moveScale = velo.Dot(m_Platform->GetInclination()); // m_Platform->GetInclination().Dot(velo); 
		RigidBody2D()->SetVelocity(m_Platform->GetInclination() * moveScale);
	}
}

void CPlatformerScript::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if ((int)LAYER_INDEX::PLATFORM == _OtherObject->GetLayerIdx())
	{
		CPlatformScript* pPlatform = _OtherObject->GetScript<CPlatformScript>();

		if (pPlatform == nullptr || RigidBody2D() == nullptr ||
			(m_Platform != nullptr && m_Platform->IsPrior()) ||
			RigidBody2D()->GetFinalVelocity().y > 0.f)
		{
			return;
		}

		// 콜라이더 큰 몬스터 뚫리길래 임시로 해제
		if (GetOwner()->GetLayerIdx() == (int)LAYER_INDEX::PLAYER && !pPlatform->IsUpsideCollision(_Collider))
			return;

		// 한번에 한 개의 플랫폼만 충돌 반응될 수 있도록 관리한다.
		m_Platform = pPlatform;
		
		Vector3 vPos = _Collider->GetFinalPos();
		Vector3 vMove = vPos - Transform()->GetPrevPos();	// 미사용

		Vector3 vPlatPos = _OtherObject->Transform()->GetWorldPos();
		Vector3 vPlatUp = pPlatform->Transform()->GetWorldDir(DIR_TYPE::UP);
		float MoveAmount = vPlatUp.Dot(vPos);
		Vector3 vPlatUpPos = _OtherObject->Transform()->GetWorldPos() + Vector3(vPlatUp * ((pPlatform->GetHeight()) / 2.f - abs(MoveAmount)));


		// 땅의 윗벡터와 플레이어-플랫폼 위치 벡터를 내적했을 때 플러스이고
		Vector3 vDist = vPos - vPlatUpPos;
		float up = vDist.Dot(vPlatUp);

		// 플랫폼의 위 옆 벡터와 플랫포머를 내적한 값의 절반
		// 플랫폼 width와 height의 절반

		// 플레이어-플랫폼 위치 벡터와 비교한다
		// 윗벡터와 차이가 덜 나면 수직 충돌

		if (up >= -20.f)
		{
			RigidBody2D()->SetGround(true);

			//// 플레이어를 플랫폼 윗면으로 밀어낸다 -> *플랫폼 내부에 있는 경우 예외처리*
			//Vector3 vecDir = m_Platform->GetInclination();
			//Vector3 invDir = Vector3(-vecDir.y, vecDir.x, 0.f);
			//Vector3 vecDist = Transform()->GetWorldPos() - m_Platform->GetEndPos();
			//float dist = abs(invDir.Dot(vecDist)) - 2;
			//invDir.Normalize();

			//Transform()->SetWorldPos(Transform()->GetWorldPos() + invDir * dist);

			Vector3 velo = RigidBody2D()->GetVelocity();
			float moveScale = m_Platform->GetInclination().Dot(velo);

			RigidBody2D()->SetVelocity(m_Platform->GetInclination() * moveScale);
		}

	}

}

void CPlatformerScript::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (nullptr != m_Platform && _OtherObject == m_Platform->GetOwner())
	{
		RigidBody2D()->SetGround(false);
		m_Platform = nullptr;
	}
}
