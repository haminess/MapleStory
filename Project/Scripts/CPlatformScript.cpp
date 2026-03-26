#include "pch.h"
#include "CPlatformScript.h"
#include <Engine/CRigidBody2D.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CLevelMgr.h>

CPlatformScript::CPlatformScript()
	: CScript((UINT)SCRIPT_TYPE::PLATFORMSCRIPT)
	, m_Height(5)
	, m_SideCollidable(false)
	, m_IsChange(true)
	, m_Downable(true)
	, m_MonBlock(true)
	, m_Prior(false)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::VEC3, "Start Pos", &m_StartPos });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::VEC3, "End Pos", &m_EndPos });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Height", &m_Height });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::BOOL, "Priority", &m_Prior });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::BOOL, "Platform Apply", &m_IsChange });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::BOOL, "Transform Apply", &m_ApplyTransform });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::BOOL, "Downable", &m_Downable });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::BOOL, "Monster Block", &m_MonBlock });
}

CPlatformScript::CPlatformScript(const CPlatformScript& _Other)
	: CScript(_Other)
	, m_StartPos(_Other.m_StartPos)
	, m_EndPos(_Other.m_EndPos)
	, m_vInclination(_Other.m_vInclination)
	, m_Height(_Other.m_Height)
	, m_IsChange(true)
	, m_Downable(_Other.m_Downable)
	, m_MonBlock(_Other.m_MonBlock)
	, m_Prior(_Other.m_Prior)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::VEC3, "Start Pos", &m_StartPos });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::VEC3, "End Pos", &m_EndPos });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Height", &m_Height });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::BOOL, "Priority", &m_Prior });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::BOOL, "Platform Apply", &m_IsChange });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::BOOL, "Transform Apply", &m_ApplyTransform });
}

CPlatformScript::~CPlatformScript()
{
}

void CPlatformScript::Init()
{
	// 플랫폼에 필요한 컴포넌트 추가
	CGameObject* pObject = GetOwner();
	pObject->SetName(L"PlatformObject");
	if (!pObject->GetComponent(COMPONENT_TYPE::COLLIDER2D)) pObject->AddComponent(new CCollider2D);

	if (!Collider2D()->IsIndependentScale())
	{
		Collider2D()->SetIndependentScale(true);
	}

	SetPlatformPos(m_StartPos, m_EndPos, m_Height);
}

void CPlatformScript::Begin()
{
	GetOwner()->SetLayerIdx((int)LAYER_INDEX::PLATFORM);
	SetPlatformPos(m_StartPos, m_EndPos, m_Height);
}

void CPlatformScript::Tick()
{
	if (m_IsChange)
	{
		m_IsChange = false;
		SetPlatformPos(m_StartPos, m_EndPos, m_Height);
	}
	if (m_ApplyTransform)
	{
		m_ApplyTransform = false;
		SetPlatformPos();
	}
}


void CPlatformScript::SaveComponent(FILE* _File)
{
	fwrite(&m_StartPos, sizeof(Vector3), 1, _File);
	fwrite(&m_EndPos, sizeof(Vector3), 1, _File);
	fwrite(&m_vInclination, sizeof(Vector3), 1, _File);
	fwrite(&m_Height, sizeof(float), 1, _File);
	fwrite(&m_Downable, sizeof(bool), 1, _File);
	fwrite(&m_MonBlock, sizeof(bool), 1, _File);
	fwrite(&m_Prior, sizeof(bool), 1, _File);
}

void CPlatformScript::LoadComponent(FILE* _File)
{
	fread(&m_StartPos, sizeof(Vector3), 1, _File);
	fread(&m_EndPos, sizeof(Vector3), 1, _File);
	fread(&m_vInclination, sizeof(Vector3), 1, _File);
	fread(&m_Height, sizeof(float), 1, _File);
	fread(&m_Downable, sizeof(bool), 1, _File);
	fread(&m_MonBlock, sizeof(bool), 1, _File);
	fread(&m_Prior, sizeof(bool), 1, _File);
	SetPlatformPos(m_StartPos, m_EndPos, m_Height);
}


void CPlatformScript::SetPlatformPos(Vector3 _StartPos, Vector3 _EndPos, float _Height)
{
	// 플랫폼 정보 업데이트
	m_StartPos = _StartPos;
	m_EndPos = _EndPos;
	m_Height = _Height;
	Vector3 vecDir = m_EndPos - m_StartPos;

	SetInclination(vecDir);

	// 플랫폼 위치 설정
	Vector3 invDir = Vector3(-vecDir.y, vecDir.x, 0.f);
	invDir.Normalize();

	Vector3 vPos = m_StartPos + Vector3(vecDir / 2.f) - Vector3(invDir * m_Height / 2.f);
	Vector3 vScale = Vector3(vecDir.Length(), m_Height, 1.f);
	Vector3 vRot = Vector3(0.f, 0.f, atan(m_vInclination.y / m_vInclination.x));
	Transform()->SetWorldPos(vPos);
	Transform()->SetRelativeScale(vScale);
	Transform()->SetRelativeRotation(vRot);

	// Begin 호출 전에 들어오면 적용 안되게 하기
	if (Collider2D())
		Collider2D()->SetScale(Vector2(vScale.x, vScale.y));
}
void CPlatformScript::SetPlatformPos()
{
	// 현재 Transform 정보 가져오기
	Vector3 vPos = Transform()->GetWorldPos();       // 중심점 위치
	Vector3 vScale = Transform()->GetRelativeScale();  // 크기
	Vector3 vRot = Transform()->GetRelativeRotation(); // 회전

	m_Height = vScale.y;  // 높이는 Scale의 y값

	// 회전각으로부터 기울기 방향 계산
	m_vInclination = Vector3(cos(vRot.z), sin(vRot.z), 0.f);
	Vector3 vecDir = m_vInclination * vScale.x;   // 방향 * 길이 = 시작점에서 끝점으로의 벡터

	// 법선 벡터 (Platform의 높이 방향)
	Vector3 invDir = Vector3(-vecDir.y, vecDir.x, 0.f);
	invDir.Normalize();

	// 시작점과 끝점 계산
	// vPos = m_StartPos + (vecDir/2) - (invDir * m_Height/2) 식을 역으로 풀기
	Vector3 heightOffset = invDir * (m_Height / 2.f);  // 높이에 의한 오프셋
	Vector3 centerPos = vPos + heightOffset;           // 높이 오프셋을 더해서 중심선 위의 점으로 이동

	m_StartPos = centerPos - Vector3(vecDir / 2.f);          // 중심점에서 방향벡터의 절반만큼 뒤로
	m_EndPos = centerPos + Vector3(vecDir / 2.f);            // 중심점에서 방향벡터의 절반만큼 앞으로
}

void CPlatformScript::BlockOverlap(CCollider2D* _Collider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (!_Collider->IsCollidable() || !_OtherCollider->IsCollidable())
		return;

	static Vector3 arrRect[4] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f)
	};

	Matrix matColLeft = _Collider->GetColliderWorldMat();
	Matrix matColRight = _OtherCollider->GetColliderWorldMat();

	// 내 투영축 구하기
	Vector3 arrProj[4] = {};
	arrProj[0] = XMVector3TransformCoord(arrRect[1], matColLeft) - XMVector3TransformCoord(arrRect[0], matColLeft);	// 가로 투영축
	arrProj[1] = XMVector3TransformCoord(arrRect[0], matColLeft) - XMVector3TransformCoord(arrRect[3], matColLeft);	// 세로 투영축
	arrProj[2] = XMVector3TransformCoord(arrRect[1], matColRight) - XMVector3TransformCoord(arrRect[0], matColRight);
	arrProj[3] = XMVector3TransformCoord(arrRect[0], matColRight) - XMVector3TransformCoord(arrRect[3], matColRight);

	// 월드공간에서 두 충돌체의 중심을 이은 벡터
	Vector3 vCenter = XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), matColRight) - XMVector3TransformCoord(Vector3(0.f, 0.f, 0.f), matColLeft);

	float fOverDist[2] = {};
	for (int i = 0; i < 2; ++i)
	{
		Vector3 vProj = arrProj[i];
		vProj.Normalize();

		// 두 사각형의 투영값 - 중심벡터 투영값 = 겹친 부분
		float fCenter = fabs(vCenter.Dot(vProj));

		// 두 사각형의 투영값 구하기
		float fDist = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			fDist += fabs(vProj.Dot(arrProj[j]));
		}
		fDist /= 2.f;

		fOverDist[i] = abs(fCenter - fDist);
	}

	// 세로로 충돌
	Vector3 vPos = _OtherObject->Transform()->GetWorldPos();
	Vector3 colPos = vPos + _OtherCollider->GetOffset();		// 충돌 오브젝트가 자식일 경우 예외처리 
	Vector2 colScale = _OtherCollider->GetScale();
	Vector3 platPos = Transform()->GetWorldPos();
	Vector3 platScale = Transform()->GetWorldScale();
	if (fOverDist[0] >= fOverDist[1])
	{
		// 위에 있다
		if (vCenter.y >= 0)
		{
			//vPos.y += fOverDist[1] * 2.f;
			vPos.y = (int)(platPos.y + platScale.y / 2.f + colScale.y / 2.f - 10);
		}

		// 아래에 있다
		else
		{
			//vPos.y -= fOverDist[1] * 2.f;
			vPos.y = platPos.y - platScale.y / 2.f - colScale.y / 2.f - 1;
		}
	}

	// 가로로 충돌
	else
	{
		// 오른쪽에 있다
		if (vCenter.x >= 0)
		{
			//vPos.x += fOverDist[0] * 2.f;
			vPos.x = platPos.x + platScale.x / 2.f + colScale.x / 2.f - 1;
		}

		// 왼쪽에 있다
		else
		{
			//vPos.x -= fOverDist[0] * 2.f;
			vPos.x = platPos.x - platScale.x / 2.f - colScale.x / 2.f - 1;
		}
	}

	_OtherObject->Transform()->SetWorldPos(vPos);
}

bool CPlatformScript::IsUpsideCollision(CCollider2D* _OtherCollider)
{
	// 1) 플랫폼 접선 벡터(tangent) 구하기
	Vector3 start = Vector3(m_StartPos.x, m_StartPos.y, 0.f);
	Vector3 end = Vector3(m_EndPos.x, m_EndPos.y, 0.f);
	Vector3 tangent = end - start;

	// 세그먼트 길이가 너무 짧으면 기설정된 경사 벡터 사용
	if (tangent.Length() < 1e-6f)
		tangent = m_vInclination;

	if (tangent.Length() < 1e-6f)
		return false; // 유효한 접선이 없으면 처리 불가

	tangent.Normalize();

	// 2) 플랫폼 법선(normal) — 2D(XY) 기준으로 접선에 수직인 벡터
	Vector3 normal(-tangent.y, tangent.x, 0.f);
	normal.Normalize();

	// 3) 상대(플레이어) 월드 위치
	Vector2 pos = _OtherCollider->GetFinalPos();
	Vector3 p = Vector3(pos.x, pos.y, 0.f);

	// 4) 세그먼트 [start,end] 위에서 p에 가장 가까운 점 q 구하기
	float segLen = (end - start).Length();
	float t = tangent.Dot(p - start); // start 기준 접선 방향 거리
	if (t < 0.f)        t = 0.f;
	else if (t > segLen) t = segLen;
	Vector3 q = start + tangent * t;

	// 5) q→p 벡터와 법선의 내적 = 서피스 위(+)/아래(-) 판정
	float signedHeight = normal.Dot(p - q);

	// 6) 착지 조건: 법선 방향 위에 있고, 두께(m_Height) 안쪽이면 블록 처리
	if (signedHeight >= 0.f && signedHeight <= m_Height)
	{
		return true;
	}
	else
	{
		// 아래쪽에서 접근하거나 너무 멀면 통과 (원웨이 플랫폼)
		return false;
	}
}
