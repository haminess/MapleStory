#include "pch.h"
#include "CKeyMgr.h"
#include "CEngine.h"
#include "CCamera.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CDevice.h"
#include "CConstBuffer.h"
#include "CTaskMgr.h"
#include "CFontMgr.h"

int g_arrKeyValue[(int)KEY::END]
=
{
	// 0
	'Q', 'W', 'E', 'R','T','Y','U','I','O','P',
	'A','S','D','F','G','H','J','K','L',
	'Z','X','C','V','B','N','M',
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 189, 187,

	// 4
	VK_UP,
	VK_DOWN,
	VK_LEFT,
	VK_RIGHT,

	// 8
	VK_SPACE,
	VK_RETURN,

	// 10
	VK_NUMPAD0,
	VK_NUMPAD1,
	VK_NUMPAD2,
	VK_NUMPAD3,
	VK_NUMPAD4,
	VK_NUMPAD5,
	VK_NUMPAD6,
	VK_NUMPAD7,
	VK_NUMPAD8,
	VK_NUMPAD9,

	VK_F1,
	VK_F2,
	VK_F3,
	VK_F4,
	VK_F5,
	VK_F6,
	VK_F7,
	VK_F8,
	VK_F9,
	VK_F10,
	VK_F11,
	VK_F12,

	// 20
	VK_LBUTTON,
	VK_RBUTTON,
	VK_MBUTTON,
	WM_SYSKEYDOWN,
	WM_SYSKEYDOWN,

	VK_DELETE,
	VK_LSHIFT,
	VK_RSHIFT,
	VK_CONTROL,
	VK_MENU,
	VK_ESCAPE,
	VK_TAB,
	WM_SYSKEYDOWN,
};


CKeyMgr::CKeyMgr()
{

}
CKeyMgr::~CKeyMgr()
{

}
Vector2 CKeyMgr::GetScreenToWorld(Vector2 _Screen)
{
	CCamera* pCam = CRenderMgr::GetInst()->GetRenderCamera();
	if (pCam && !CTaskMgr::GetInst()->IsLevelChanged())
	{
		Vector3 vCamPos = pCam->Transform()->GetWorldPos();

		// 카메라의 좌상단과 우하단 좌표 계산
		float scale = 1.f;
		float width = pCam->GetWidth();
		if (pCam->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
		{
			scale = pCam->GetScale();
			width *= scale;
		}
		float height = width / pCam->GetAspectRatio();

		Vector2 vCamLT = Vector2(vCamPos.x - width / 2.f, vCamPos.y + height / 2.f);
		return Vector2(vCamLT.x + _Screen.x * scale, vCamLT.y - _Screen.y * scale);
	}
}

Vector2 CKeyMgr::GetWorldToScreen(Vector2 _World)
{
	CCamera* pCam = CRenderMgr::GetInst()->GetRenderCamera();
	if (pCam && !CTaskMgr::GetInst()->IsLevelChanged())
	{
		Vector3 vCamPos = pCam->Transform()->GetWorldPos();

		// 카메라의 scale과 크기 계산
		float scale = 1.f;
		float width = pCam->GetWidth();
		if (pCam->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
		{
			scale = pCam->GetScale();
			width *= scale;
		}
		float height = width / pCam->GetAspectRatio();

		// 카메라의 좌상단 좌표 계산
		Vector2 vCamLT = Vector2(vCamPos.x - width / 2.f, vCamPos.y + height / 2.f);

		// 월드 좌표에서 카메라 좌상단 기준으로의 거리를 구한 뒤
		// scale로 나누어 스크린 좌표로 변환
		return Vector2(
			(_World.x - vCamLT.x) / scale,
			(vCamLT.y - _World.y) / scale
		);
	}
	return Vector2(0.f, 0.f);
}

void CKeyMgr::Init()
{
	for (int i = 0; i < (int)KEY::END; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}
}
void CKeyMgr::Tick()
{
	// 게임 윈도우가 포커싱 중일 때만 키 관련 계산이 동작하도록 한다.
	// **GetFocus 메서드
	// 현재 포커싱 중인 윈도우 핸들을 반환한다.

	// 변경
	// 포커싱 중인 윈도우가 있는 중에만
	if (GetFocus())
	{
		for (int i = 0; i < (int)KEY::END; ++i)
		{
			// 키 눌림
			if (GetAsyncKeyState(g_arrKeyValue[i]) && 0x8001 || m_vecKey[i].NextPressed)
			{
				// 키가 눌려있고, 이전에는 눌려있지 않았다.
				if (false == m_vecKey[i].PrevPressed)
				{
					m_vecKey[i].State = KEY_STATE::TAP;
				}

				// 키가 눌려있고, 이전에도 눌려있었다.
				else
				{
					m_vecKey[i].State = KEY_STATE::PRESSED;
				}
				m_vecKey[i].PrevPressed = true;
				m_vecKey[i].NextPressed = false;
			}

			// 키 안눌림
			else
			{
				// 키가 눌려있다가 최초로 뗐다.
				if (m_vecKey[i].PrevPressed)
				{
					m_vecKey[i].State = KEY_STATE::RELEASED;
				}

				// 아무 상태 아님
				else
				{
					m_vecKey[i].State = KEY_STATE::NONE;
				}

				m_vecKey[i].PrevPressed = false;
			}
		}

		m_MousePrevPos = m_MousePos;
		m_MouseWorldPrevPos = m_MouseWorldPos;

		// 마우스 스크린 좌표 갱신
		POINT Pos;
		GetCursorPos(&Pos);
		ScreenToClient(CEngine::GetInst()->GetMainWnd(), &Pos);	// 모니터 -> 윈도우 좌표
		m_MousePos = Vector2((float)Pos.x, (float)Pos.y);

		// 마우스 월드 좌표 갱신
		CCamera* pCam = CRenderMgr::GetInst()->GetRenderCamera();
		if (pCam && !CTaskMgr::GetInst()->IsLevelChanged())
		{
			Vector3 vCamPos = pCam->Transform()->GetWorldPos();

			// 카메라의 좌상단과 우하단 좌표 계산
			float scale = 1.f;
			float width = pCam->GetWidth();
			if (pCam->GetProjType() == PROJ_TYPE::ORTHOGRAPHIC)
			{
				scale = pCam->GetScale();
				width *= scale;
			}
			float height = width / pCam->GetAspectRatio();

			Vector2 vCamLT = Vector2(vCamPos.x - width / 2.f, vCamPos.y + height / 2.f);
			m_MouseWorldPos = Vector2(vCamLT.x + m_MousePos.x * scale, vCamLT.y - m_MousePos.y * scale);
		}

		wstring mousepos = std::to_wstring((int)m_MouseWorldPos.x) + L", " + std::to_wstring((int)m_MouseWorldPos.y);
		CRenderMgr::GetInst()->AddText(mousepos.c_str(), m_MousePos.x, m_MousePos.y, 16, FONT_RGBA(255, 255, 255, 255));


		// 마우스 이동 방향 계산
		m_MouseDir = m_MousePos - m_MousePrevPos;
		m_MouseWorldDir = m_MouseWorldPos - m_MouseWorldPrevPos;

		g_Data.MousePos = m_MousePos;
		g_Data.MouseWorldPos = m_MouseWorldPos;

		CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::GLOBAL);
	}

	// 포커싱 중인 윈도우가 없을 경우
	else
	{
		for (int i = 0; i < (int)KEY::END; ++i)
		{
			if (KEY_STATE::TAP == m_vecKey[i].State || KEY_STATE::PRESSED == m_vecKey[i].State)
			{
				m_vecKey[i].State = KEY_STATE::PRESSED;
			}

			else if (KEY_STATE::RELEASED == m_vecKey[i].State)
			{
				m_vecKey[i].State = KEY_STATE::NONE;
			}

			m_vecKey[i].PrevPressed = false;
		}
	}

}
