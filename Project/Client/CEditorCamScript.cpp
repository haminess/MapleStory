#include "pch.h"
#include "CEditorCamScript.h"
#include "CImGuiMgr.h"
#include "EditorUI.h"

#include <Engine\CKeyMgr.h>
#include <Engine\CTimeMgr.h>
#include <Engine\CTransform.h>
#include <Engine\CCamera.h>

CEditorCamScript::CEditorCamScript()
	: CScript(0)
	, m_CameraSpeed(600.f)
{
}

CEditorCamScript::~CEditorCamScript()
{
}

void CEditorCamScript::Tick()
{
	if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
		CameraOrthgraphicMove();
	else
		CameraPerspectiveMove();
}

void CEditorCamScript::CameraOrthgraphicMove()
{
	Vector3 vWorldPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
	{
		vWorldPos.y += EngineDT * m_CameraSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vWorldPos.y -= EngineDT * m_CameraSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vWorldPos.x -= EngineDT * m_CameraSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vWorldPos.x += EngineDT * m_CameraSpeed;
	}

	// 마우스 카메라 이동 시 떨림 현상!!
	// 마우스 드래그에 따라 dir 업데이트
	// 마우스 방향에 따라 카메라 이동
	// 카메라 이동된 만큼 마우스 world pos, dir 반대 방향으로 바뀜
	// 다음 업데이트 때 반대 방향으로 업데이트됨
	Transform()->SetRelativePos(vWorldPos);

	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
		return;

	float Scale = Camera()->GetScale();
	if (KEY_TAP(KEY::WHEEL_UP))
		Scale += Scale * EngineDT * m_CameraSpeed * 0.1f;
	if (KEY_TAP(KEY::WHEEL_DOWN))
		Scale -= Scale * EngineDT * m_CameraSpeed * 0.1f;


	if (Scale < 0.01f)
		Scale = 0.01f;
	if (Scale > 30.f)
		Scale = 30.f;

	Camera()->SetScale(Scale);
}

void CEditorCamScript::CameraPerspectiveMove()
{
	float Speed = m_CameraSpeed;
	if (KEY_PRESSED(KEY::LSHIFT))
		Speed *= 5.f;

	Vector3 vFront = Transform()->GetLocalDir(DIR_TYPE::FRONT);
	Vector3 vRight = Transform()->GetLocalDir(DIR_TYPE::RIGHT);
	Vector3 vUp = Transform()->GetLocalDir(DIR_TYPE::UP);

	Vector3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
		vPos += vFront * EngineDT * Speed;
	if (KEY_PRESSED(KEY::S))
		vPos -= vFront * EngineDT * Speed;
	if (KEY_PRESSED(KEY::A))
		vPos -= vRight * EngineDT * Speed;
	if (KEY_PRESSED(KEY::D))
		vPos += vRight * EngineDT * Speed;
	if (KEY_PRESSED(KEY::Q))
		vPos -= vUp * EngineDT * Speed;
	if (KEY_PRESSED(KEY::E))
		vPos += vUp * EngineDT * Speed;

	Transform()->SetRelativePos(vPos);

	//// 동작 안함
	//float Scale = Camera()->GetScale();
	//if (KEY_TAP(KEY::WHEEL_UP))
	//	Scale += Scale * EngineDT * Speed * 100.f;
	//if (KEY_TAP(KEY::WHEEL_DOWN))
	//	Scale -= Scale * EngineDT * Speed * 100.f;
	//Camera()->SetScale(Scale);

	// 마우스 방향에 따른 오브젝트 회전
	if (KEY_PRESSED(KEY::RBTN))
	{
		Vector3 vRot = Transform()->GetRelativeRotation();

		Vector2 vDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += vDir.x * EngineDT * 15.f;
		vRot.x += vDir.y * EngineDT * 10.f;

		Transform()->SetRelativeRotation(vRot);
	}
}