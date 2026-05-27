#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"

CCollisionMgr::CCollisionMgr()
	: m_Matrix{}
{

}

CCollisionMgr::~CCollisionMgr()
{

}

void CCollisionMgr::Tick()
{
	if (!CLevelMgr::GetInst()->GetCurrentLevel())
		return;

	for (UINT Row = 0; Row < MAX_LAYER; ++Row)
	{
		for (UINT Col = Row; Col < MAX_LAYER; ++Col)
		{
			if (!(m_Matrix[Row] & (1 << Col)))
				continue;

			// Row , Col 두 레이어가 충돌체크를 해야한다.
			CollisionBtwLayer(Row, Col);
		}
	}
}

void CCollisionMgr::CollisionBtwLayer(UINT _Left, UINT _Right)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	const vector<CGameObject*>& vecLeft = pCurLevel->GetLayer(_Left)->GetObjects();
	const vector<CGameObject*>& vecRight = pCurLevel->GetLayer(_Right)->GetObjects();

	// 충돌 검사를 하려는 두 레이어가 다른 레이어인경우
	if (_Left != _Right)
	{
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			if (nullptr == vecLeft[i]->Collider2D())
				continue;

			for (size_t j = 0; j < vecRight.size(); ++j)
			{
				if (nullptr == vecRight[j]->Collider2D())
					continue;

				CollisionBtwCollider2D(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D());
			}
		}
	}

	// 두 레이어가 동일한 경우
	else
	{
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			if (nullptr == vecLeft[i]->Collider2D())
				continue;

			for (size_t j = i + 1; j < vecRight.size(); ++j)
			{
				if (nullptr == vecRight[j]->Collider2D())
					continue;

				CollisionBtwCollider2D(vecLeft[i]->Collider2D(), vecRight[j]->Collider2D());
			}
		}
	}
}

void CCollisionMgr::CollisionBtwCollider2D(CCollider2D* _LeftCol, CCollider2D* _RightCol)
{
	COLLIDER_ID id = {};
	id.Left = _LeftCol->GetID();
	id.Right = _RightCol->GetID();

	map<ULONGLONG, bool>::iterator iter = m_ColInfo.find(id.ID);

	// 한번도 등록된 적이 없었다.
	if (iter == m_ColInfo.end())
	{
		// 충돌 조합 등록
		m_ColInfo.insert(make_pair(id.ID, false));
		iter = m_ColInfo.find(id.ID);
	}


	// 두 충돌체중 하나라도 Dead 상태거나 비활성화 예정이면 EndOverlap
	bool IsEnd = !_LeftCol->GetOwner()->IsValid() || !_RightCol->GetOwner()->IsValid() ||
			     !_LeftCol->IsActive() || !_RightCol->IsActive();

	if (IsEnd)
	{
		int a = 0;
	}
	if (_RightCol->GetOwner()->GetName() == L"RagingBlow" || _LeftCol->GetOwner()->GetName() == L"RagingBlow")
	{
		int a = 0;
	}

	// 현재 겹쳐있다.
	if (IsCollision(_LeftCol, _RightCol))
	{

		// 이전에도 겹쳐있었다.
		if (iter->second)
		{
			// 둘중 하나 이상이 곧 삭제 예정이다.
			if (IsEnd)
			{
				_LeftCol->EndOverlap(_RightCol);
				_RightCol->EndOverlap(_LeftCol);
				iter->second = false;
			}
			else
			{
				// 충돌중이다.
				_LeftCol->Overlap(_RightCol);
				_RightCol->Overlap(_LeftCol);
			}

		}



		// 이전에는 떨어져있었다.
		else if (!IsEnd)
		{
			_LeftCol->BeginOverlap(_RightCol);
			_RightCol->BeginOverlap(_LeftCol);
			iter->second = true;
		}
	}

	// 현재 떨어져있다.
	else
	{
		// 이전에는 겹쳐있었다.
		if (iter->second)
		{
			_LeftCol->EndOverlap(_RightCol);
			_RightCol->EndOverlap(_LeftCol);
			iter->second = false;
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider2D* _Left, CCollider2D* _Right)
{
	// 0 -- 1
	// | \  |
	// 3 -- 2 
	static Vector3 arrRect[4] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f)
	};

	Matrix matColLeft = _Left->GetColliderWorldMat();
	Matrix matColRight = _Right->GetColliderWorldMat();

	if (_Left->GetType() == COLLIDER_TYPE::BOX &&
		_Right->GetType() == COLLIDER_TYPE::BOX)
	{
		// 투영축 구하기, 투영축 == 투영을 시킬 대상
		Vector3 arrProj[4] = {};

		// 사각형 정점 데이터를 받아와 꼭짓점의 월드 좌표를 구함, 좌표를 통해 투영축 벡터 구함
		arrProj[0] = XMVector3TransformCoord(arrRect[1], matColLeft) - XMVector3TransformCoord(arrRect[0], matColLeft);		// Left
		arrProj[1] = XMVector3TransformCoord(arrRect[3], matColLeft) - XMVector3TransformCoord(arrRect[0], matColLeft);
		arrProj[2] = XMVector3TransformCoord(arrRect[1], matColRight) - XMVector3TransformCoord(arrRect[0], matColRight);	// Right
		arrProj[3] = XMVector3TransformCoord(arrRect[3], matColRight) - XMVector3TransformCoord(arrRect[0], matColRight);

		//DrawDebugRect(Vector4(1.f, 0.f, 1.f, 1.f), _Left->GetColliderWorldMat(), false, 0.f);
		//DrawDebugRect(Vector4(1.f, 0.f, 1.f, 1.f), _Right->GetColliderWorldMat(), false, 0.f);

		// 월드공간에서 두 충돌체의 중심을 이은 벡터
		Vector3 vCenter = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), matColLeft) - XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), matColRight);

		for (int i = 0; i < 4; ++i)
		{
			Vector3 vProj = arrProj[i];
			vProj.Normalize();

			float fCenter = fabs(vCenter.Dot(vProj));

			// 두 사각형의 투영값 구하기
			float fDist = 0.f;
			for (int j = 0; j < 4; ++j)
			{
				fDist += fabs(vProj.Dot(arrProj[j]));
			}
			fDist /= 2.f;

			// 사각형의 사이가 멀어질 수록 fCenter값이 커진다
			if (fDist < fCenter)
				return false;
		}
	}
	else if (_Left->GetType() == COLLIDER_TYPE::LINE &&
		_Right->GetType() == COLLIDER_TYPE::LINE)
	{

		// 사각형 정점 데이터를 받아와 꼭짓점의 월드 좌표를 구함
		Vector3 Left_LT = XMVector3TransformCoord(arrRect[1], matColLeft);
		Vector3 Left_RB = XMVector3TransformCoord(arrRect[3], matColLeft);
		Vector3 Right_LT = XMVector3TransformCoord(arrRect[1], matColRight);
		Vector3 Right_RB = XMVector3TransformCoord(arrRect[3], matColRight);
		//DrawDebugLine(Vector4(1.f, 0.f, 1.f, 1.f), Left_LT, Left_RB, false, 0.f);
		//DrawDebugLine(Vector4(1.f, 0.f, 1.f, 1.f), Right_LT, Right_RB, false, 0.f);

		Vector3 intersection;

		float x1 = Left_LT.x;
		float y1 = Left_LT.y;
		float x2 = Left_RB.x;
		float y2 = Left_RB.y;

		float x3 = Right_LT.x;
		float y3 = Right_LT.y;
		float x4 = Right_RB.x;
		float y4 = Right_RB.y;

		// 분모 계산
		float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

		const float EPSILON = 0.0001f;

		// 선분이 점인 경우 검사
		bool isLeftPoint = (abs(x1 - x2) < EPSILON && abs(y1 - y2) < EPSILON);
		bool isRightPoint = (abs(x3 - x4) < EPSILON && abs(y3 - y4) < EPSILON);

		if (isLeftPoint || isRightPoint)
		{
			if (isLeftPoint && isRightPoint)
			{
				// 둘 다 점일 경우 충돌 검사
				float dist = sqrtf((x1 - x3) * (x1 - x3) + (y1 - y3) * (y1 - y3));
				if (dist < 0.01f)
				{
					_Left->SetCollisionArea(Left_LT);
					_Right->SetCollisionArea(Left_LT);
					return true;
				}
			}
			else if (isLeftPoint)
			{
				// 왼쪽이 점일 경우 충돌 검사
				if (IsCollisionDotLine(Vector2(x1, y1), Vector2(x3, y3), Vector2(x4, y4)))
				{
					_Left->SetCollisionArea(Left_LT);
					_Right->SetCollisionArea(Left_LT);
					printf("충돌중_점\n");
					return true;
				}
			}
			else
			{
				// 오른쪽이 점일 경우 충돌 검사
				if (IsCollisionDotLine(Vector2(x3, y3), Vector2(x1, y1), Vector2(x2, y2)))
				{
					_Left->SetCollisionArea(Right_LT);
					_Right->SetCollisionArea(Right_LT);
					printf("충돌중_점\n");
					return true;
				}
			}
			return false;
		}

		// 분모가 0이면 평행하거나 일치
		if (abs(denominator) < 0.01f)
		{
			// 한 선분의 끝점이 하나라도 다른 선분에 속해있으면 충돌
			if (IsCollisionDotLine(Vector2(x1, y1), Vector2(x3, y3), Vector2(x4, y4)) ||
				IsCollisionDotLine(Vector2(x3, y3), Vector2(x1, y1), Vector2(x2, y2)) ||
				IsCollisionDotLine(Vector2(x2, y2), Vector2(x3, y3), Vector2(x4, y4)) ||
				IsCollisionDotLine(Vector2(x4, y4), Vector2(x1, y1), Vector2(x2, y2)))
			{
				printf("충돌중_평행\n");
				return true;
			}

			printf("충돌해제\n");
			return false;
		}

		// 교점 계산
		float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
		float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denominator;

		// 선분 내에 교점이 있으면 충돌
		if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f)
		{
			intersection.x = x1 + t * (x2 - x1);
			intersection.y = y1 + t * (y2 - y1);
			_Left->SetCollisionArea(Vector3(intersection.x, intersection.y, 0.f));
			_Right->SetCollisionArea(Vector3(intersection.x, intersection.y, 0.f));

			printf("충돌중_교점\n");
			return true;
		}

		return false;
	}
	else
	{
		return false;
	}

	return true;
}

bool CCollisionMgr::IsCollisionDotLine(Vector2 _Dot, Vector2 _Line1, Vector2 _Line2)
{
	float x1 = _Dot.x;
	float y1 = _Dot.y;
	float x3 = _Line1.x;
	float y3 = _Line1.y;
	float x4 = _Line2.x;
	float y4 = _Line2.y;

	// Left가 점인 경우, Right 선분과의 충돌 검사
	float t = ((x1 - x3) * (x4 - x3) + (y1 - y3) * (y4 - y3)) /
		((x4 - x3) * (x4 - x3) + (y4 - y3) * (y4 - y3));

	if (t >= 0.0f && t <= 1.0f)
	{
		// 선분 위의 가장 가까운 점 계산
		float px = x3 + t * (x4 - x3);
		float py = y3 + t * (y4 - y3);

		// 점과의 실제 거리 계산
		float distance = sqrtf((px - x1) * (px - x1) + (py - y1) * (py - y1));

		// 일정 거리 이내일 때만 충돌로 판정
		if (distance < 0.1f)
		{
			return true;
		}
	}
	return false;
}

void CCollisionMgr::CollisionCheck(UINT _Left, UINT _Right)
{
	UINT Row = _Left;
	UINT Col = _Right;

	if (Col < Row)
	{
		Row = _Right;
		Col = _Left;
	}

	if (m_Matrix[Row] & (1 << Col))
	{
		m_Matrix[Row] &= ~(1 << Col);
	}
	else
	{
		m_Matrix[Row] |= (1 << Col);
	}
}
