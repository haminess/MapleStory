#include "pch.h"
#include "CCameraScript.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>
#include <Engine/CLevelMgr.h>

CCameraScript::CCameraScript()
	: CScript((UINT)SCRIPT_TYPE::CAMERASCRIPT)
	, m_CameraSpeed(300.f)
	, m_Target(nullptr)
	, m_Find(true)

{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Camera Speed", &m_CameraSpeed });
	//AddScriptParam(tScriptParam{ SCRIPT_PARAM::GAMEOBJECT, "Target", &m_Target });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::BOOL, "Find Target", &m_Find });
}

CCameraScript::CCameraScript(const CCameraScript& _Other)
	: CScript((UINT)SCRIPT_TYPE::CAMERASCRIPT)
	, m_CameraSpeed(_Other.m_CameraSpeed)
{
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::FLOAT, "Camera Speed", &m_CameraSpeed });
	//AddScriptParam(tScriptParam{ SCRIPT_PARAM::GAMEOBJECT, "Target", &m_Target });
	AddScriptParam(tScriptParam{ SCRIPT_PARAM::BOOL, "Find Target", &m_Find });
}

CCameraScript::~CCameraScript()
{
}


void CCameraScript::SaveComponent(FILE* _File)
{
	fwrite(&m_CameraSpeed, sizeof(float), 1, _File);
	wstring name = L"";
	if (m_Target) name = m_Target->GetName();
	SaveWString(name, _File);
}

void CCameraScript::LoadComponent(FILE* _File)
{
	fread(&m_CameraSpeed, sizeof(float), 1, _File);
	LoadWString(m_TargetName, _File);
}

void CCameraScript::Begin()
{
	SetBackground(CLevelMgr::GetInst()->FindObjectByName(L"Map"));
}

Vector3 CCameraScript::ClampCameraPosition(const Vector3& _Position)
{
	if (!m_Map)
		return _Position;

	Vector3 newPos = _Position;
	float camWidth = Camera()->GetWidth() * Camera()->GetScale();
	float camHeight = camWidth / Camera()->GetAspectRatio();

	// 카메라가 배경 범위를 벗어나지 않도록 제한
	float minX = m_MapPos.x - (m_MapSize.x / 2.f) + (camWidth / 2.f);
	float maxX = m_MapPos.x + (m_MapSize.x / 2.f) - (camWidth / 2.f);
	float minY = m_MapPos.y - (m_MapSize.y / 2.f) + (camHeight / 2.f);
	float maxY = m_MapPos.y + (m_MapSize.y / 2.f) - (camHeight / 2.f);

	if (minX < maxX)
		newPos.x = std::clamp(newPos.x, minX, maxX);
	else
		newPos.x = 0.f;

	if (minY < maxY)
		newPos.y = std::clamp(newPos.y, minY, maxY);
	else
		newPos.y = 0.f;

	return newPos;
}


void CCameraScript::Tick()
{

	if (m_Find)
	{
		SetTarget(m_Target);
	}

	if (m_Target && !m_Target->IsValid())
		SetTarget(nullptr);

	if (m_Target)
	{
		// 타겟의 현재 위치 가져오기
		Vector3 vTargetPos = m_Target->Transform()->GetWorldPos();

		// 카메라가 이동할 목표 위치 계산
		Vector3 m_vOffsetPos = Vector3(0.f, 0.f, -10.f);
		Vector3 vDest = vTargetPos + m_vOffsetPos;

		// 현재 카메라 위치
		Vector3 vCurPos = Transform()->GetWorldPos();

		// 목표 지점까지의 거리 계산
		Vector3 vDiff = vDest - vCurPos;
		float distance = vDiff.Length();

		// 카메라 위치 추적
		const float smoothSpeed = 3.f;
		float lerpSpeed = smoothSpeed;

		if (distance < 1.f)
			lerpSpeed *= 0.5f;

		Vector3 vLerpPos = {};
		vLerpPos.x = Lerp(vCurPos.x, vDest.x, lerpSpeed * DT);
		vLerpPos.y = Lerp(vCurPos.y, vDest.y, lerpSpeed * DT);
		vLerpPos.z = vDest.z;

		// 미세한 떨림 방지
		const float deadzone = 0.01f;
		if (abs(vDest.x - vLerpPos.x) < deadzone)
			vLerpPos.x = vDest.x;
		if (abs(vDest.y - vLerpPos.y) < deadzone)
			vLerpPos.y = vDest.y;

		// 카메라 위치를 배경 범위 내로 제한
		vLerpPos = ClampCameraPosition(vLerpPos);

		// 카메라 위치 설정
		Transform()->SetRelativePos(vLerpPos);
	}
	else if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
		CameraOrthgraphicMove();
	else
		CameraPerspectiveMove();
}

void CCameraScript::CameraOrthgraphicMove()
{
	Vector3 vWorldPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
	{
		vWorldPos.y += DT * m_CameraSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vWorldPos.y -= DT * m_CameraSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vWorldPos.x -= DT * m_CameraSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vWorldPos.x += DT * m_CameraSpeed;
	}

	// 카메라 위치를 배경 범위 내로 제한
	vWorldPos = ClampCameraPosition(vWorldPos);
	Transform()->SetRelativePos(vWorldPos);


	float Scale = Camera()->GetScale();

	if (KEY_PRESSED(KEY::NUM0))
	{
		Scale += DT;
	}

	if (KEY_PRESSED(KEY::NUM1))
	{
		Scale -= DT;
		if (Scale < 0.1f)
			Scale = 0.1f;
	}

	Camera()->SetScale(Scale);
}

void CCameraScript::CameraPerspectiveMove()
{
	float Speed = m_CameraSpeed;
	if (KEY_PRESSED(KEY::LSHIFT))
		Speed *= 5.f;

	Vector3 vFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vector3 vRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);

	Vector3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
		vPos += vFront * DT * Speed;
	if(KEY_PRESSED(KEY::S))
		vPos -= vFront * DT * Speed;
	if (KEY_PRESSED(KEY::A))
		vPos -= vRight * DT * Speed;
	if (KEY_PRESSED(KEY::D))
		vPos += vRight * DT * Speed;
	float Scale = Camera()->GetScale();
	if (KEY_TAP(KEY::WHEEL_UP))
		Scale += EngineDT;
	if (KEY_TAP(KEY::WHEEL_DOWN))
	{
		Scale -= EngineDT;
		if (Scale < 0.1f)
			Scale = 0.1f;
	}

	Camera()->SetScale(Scale);

	Transform()->SetRelativePos(vPos);

	// 마우스 방향에 따른 오브젝트 회전
	if (KEY_PRESSED(KEY::RBTN))
	{
		Vector3 vRot = Transform()->GetRelativeRotation();

		Vector2 vDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += vDir.x * DT * 15.f;
		vRot.x += vDir.y * DT * 10.f;

		Transform()->SetRelativeRotation(vRot);
	}
}