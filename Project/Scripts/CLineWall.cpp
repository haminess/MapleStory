#include "pch.h"
#include "CLineWall.h"

CLineWall::CLineWall()
	: CScript(SCRIPT_TYPE::LINEWALL)
{
}

CLineWall::~CLineWall()
{
}

void CLineWall::BeginOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (L"Player" == _OtherObject->GetName())
	{
  		CGameObject* pPlayer = _OtherObject;
  		Vector3 pos = pPlayer->Transform()->GetWorldPos();
		pPlayer->Transform()->SetWorldPos(Vector3(pPlayer->Transform()->GetPrevPos().x, pos.y, pos.z));
	}
}

void CLineWall::Overlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (L"Player" == _OtherObject->GetName())
	{
		CGameObject* pPlayer = _OtherObject;
		Vector3 pos = pPlayer->Transform()->GetWorldPos();
		pPlayer->Transform()->SetWorldPos(Vector3(pPlayer->Transform()->GetPrevPos().x, pos.y, pos.z));
	}
}

void CLineWall::EndOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	int a = 0;
}
