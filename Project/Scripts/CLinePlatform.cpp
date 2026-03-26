#include "pch.h"
#include "CLinePlatform.h"
#include <Engine/CRigidBody2D.h>
#include <Engine/CAssetMgr.h>

CLinePlatform::CLinePlatform()
	: CScript((UINT)SCRIPT_TYPE::LINEPLATFORM)
{
}

CLinePlatform::~CLinePlatform()
{
}

void CLinePlatform::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (L"Player" == _OtherObject->GetName())
	{
		CGameObject* pPlayer = _OtherObject;
 		if (pPlayer->RigidBody2D()->GetGravityVelocity().y < 0.f)
		{
			pPlayer->RigidBody2D()->SetGround(true);
			Vector2 inter = _Collider->GetIntersection();
			pPlayer->Transform()->SetWorldPos(Vector3(inter.x, inter.y, pPlayer->Transform()->GetWorldPos().z));
		}
	}

	if (_OtherObject->GetParent() && L"Player" == _OtherObject->GetParent()->GetName())
	{
		CGameObject* pPlayer = _OtherObject->GetParent();
		if (pPlayer->RigidBody2D()->GetGravityVelocity().y < 0.f)
		{
			pPlayer->RigidBody2D()->SetGround(true);
			Vector2 inter = _Collider->GetIntersection();
			pPlayer->Transform()->SetWorldPos(Vector3(inter.x, inter.y, pPlayer->Transform()->GetWorldPos().z));
		}
	}
}
void CLinePlatform::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (L"Player" == _OtherObject->GetName())
	{
		CGameObject* pPlayer = _OtherObject;

		//Vector3 velo = pPlayer->RigidBody2D()->GetVelocity();
		//Vector3 force = pPlayer->RigidBody2D()->GetForce();
		//Vector3 vecPlat = GetInclination1();
		//vecPlat.Normalize();
		//float dot = vecPlat.Dot(velo);
		//velo = vecPlat * vecPlat.Dot(velo);
		//force = vecPlat * vecPlat.Dot(force);

		//pPlayer->RigidBody2D()->SetVelocity(velo);
		//pPlayer->RigidBody2D()->SetForce(Vector3(0.f, 0.f, 0.f));

		if (pPlayer->RigidBody2D()->GetVelocity().y <= 0.f)
		{
			Vector3 pos = pPlayer->Transform()->GetWorldPos();
			if (IsInRange(pos.x))
			{
				/*pPlayer->Transform()->SetWorldPos(Vector3(pos.x, GetPosY(pos.x), pos.z));
				printf("Overlap\n");*/
			}
		}
	}
}

void CLinePlatform::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (L"Player" == _OtherObject->GetName())
	{
		int a = 0;
		//_OtherObject->RigidBody2D()->SetGround(false);
	}
}