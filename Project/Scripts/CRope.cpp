#include "pch.h"
#include "CRope.h"
#include "CPlayerScript.h"

#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>

CRope::CRope()
	:CScript(SCRIPT_TYPE::ROPE)
{
}

CRope::~CRope()
{
}

void CRope::Init()
{
	CGameObject* pObject = GetOwner();
	pObject->SetName(L"RopeObject");
	if (!pObject->GetComponent(COMPONENT_TYPE::COLLIDER2D)) pObject->AddComponent(new CCollider2D);

	if (!Collider2D()->IsIndependentScale())
	{
		Collider2D()->SetIndependentScale(false);
		Collider2D()->SetScale(Vector2(1.f, 1.f));
	}
}

void CRope::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (L"Player" == _OtherObject->GetName())
	{
		Vector3 ropeColPos = _Collider->GetFinalPos();
		Vector3 ropeColSca = _Collider->GetFinalScale();
		float ropeTop = ropeColPos.y + ropeColSca.y * 0.5f;

		Vector3 playerColPos = _OtherCollider->GetFinalPos();
		Vector3 playerColSca = _OtherCollider->GetFinalScale();
		float playerBottom = playerColPos.y - playerColSca.y * 0.5f;

		// 플레이어 바닥이 로프 윗면 근처 + 플레이어가 로프보다 위
		bool bTop = (playerBottom >= ropeTop - 5.f) && (playerColPos.y > ropeColPos.y);

		// 줄타기 시작
		if (!_OtherObject->GetScript<CPlayerScript>()->IsRope() && ((KEY_TAP(KEY::UP)&&!bTop) || KEY_PRESSED(KEY::DOWN)))
		{
			CPlayerScript* pPlayer = _OtherObject->GetScript<CPlayerScript>();
			pPlayer->SetRope(true);
		}

		// 줄타는 중
		if (KEY_PRESSED(KEY::UP))
		{
			CPlayerScript* pPlayer = _OtherObject->GetScript<CPlayerScript>();
			if (pPlayer->IsRope())
			{
				Vector3 pos = _OtherObject->Transform()->GetWorldPos();
				_OtherObject->Transform()->SetWorldPos(Vector3(Transform()->GetWorldPos().x, pos.y + DT * 100.f, pos.z));
			}
		}
		if (KEY_PRESSED(KEY::DOWN))
		{
			CPlayerScript* pPlayer = _OtherObject->GetScript<CPlayerScript>();
			if (pPlayer->IsRope())
			{
				Vector3 pos = _OtherObject->Transform()->GetWorldPos();
				_OtherObject->Transform()->SetWorldPos(Vector3(Transform()->GetWorldPos().x, pos.y - DT * 100.f, pos.z));
			}
		}

		// 줄타기 종료
		// 충돌 해제됐거나 중간에 점프했을 경우
		if (KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT))
		{
			if (KEY_TAP(KEY::SPACE))
			{
				CPlayerScript* pPlayer = _OtherObject->GetScript<CPlayerScript>();
				pPlayer->SetRope(false);
			}
		}
	}
}

void CRope::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (L"Player" == _OtherObject->GetName())
	{
		CPlayerScript* pPlayer = _OtherObject->GetScript<CPlayerScript>();
		pPlayer->SetRope(false);
	}
}
