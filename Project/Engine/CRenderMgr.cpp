#include "pch.h"
#include "CRenderMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CLight2D.h"
#include "CFlipbookPlayer.h"
#include "CCollider2D.h"

#include "CStructuredBuffer.h"

#include "CMeshRender.h"
#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CTaskMgr.h"
#include "CFontMgr.h"

CRenderMgr::CRenderMgr()
	: m_DbgObj(nullptr)
	, m_Light2DBuffer(nullptr)
	, m_EditorCam(nullptr)
	, m_CamNum(0)
	, m_IsEditor(false)
	, m_bGrid(true)
{
	m_Light2DBuffer = new CStructuredBuffer;
	m_Light2DBuffer->Create(sizeof(tLight2DInfo), 2, SB_TYPE::SRV_ONLY, true);
}

CRenderMgr::~CRenderMgr()
{
	DELETE(m_DbgObj);
	DELETE(m_Light2DBuffer);
	DELETE(m_FadeObj);
}

void CRenderMgr::PlayAnimEff(Ptr<CFlipbook> _Flip, CGameObject* _Parent, Vector3 _Pos, Vector3 _Rot, int _Layer)
{
	if (_Flip == nullptr)
		return;

	// 남은 오브젝트 있을 경우
	auto iter = m_EffObjList.begin();
	for (; iter != m_EffObjList.end(); ++iter)
	{
		if (!(*iter)->IsActive())
		{
			AddChild(_Parent, (*iter));

			(*iter)->SetActive(true);
			(*iter)->SetLayerIdx(_Layer);
			(*iter)->Transform()->SetRelativePos(_Pos);
			(*iter)->Transform()->SetRelativeRotation(_Rot);
			(*iter)->Transform()->UseIndependentScale(true);
			(*iter)->FlipbookPlayer()->PlayOnce(_Flip, 10);

			m_EffObjList.push_front(*iter);
			m_EffObjList.erase(iter);
			return;
		}
	}

	// 모든 오브젝트가 플레이 중일 경우
	CGameObject* pNew = new CGameObject;
	pNew->SetName(L"_Eff");
	pNew->SetLayerIdx(_Layer);
	pNew->Transform()->UseIndependentScale(true);
	pNew->Transform()->SetRelativePos(_Pos);
	pNew->Transform()->SetRelativeRotation(_Rot);

	// 컴포넌트 추가
	pNew->AddComponent(new CMeshRender);
	pNew->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	pNew->AddComponent(new CFlipbookPlayer);
	pNew->FlipbookPlayer()->AddFlipbook(0, _Flip);
	pNew->FlipbookPlayer()->PlayOnce(_Flip, 10);

	if (_Parent)
		_Parent->AddChild(pNew);
	else
		CreateObject(pNew, _Layer, false);

	m_EffObjList.push_front(pNew);
}

void CRenderMgr::Init()
{
	m_DbgObj = new CGameObject;
	m_DbgObj->SetName(L"DebugObject");
	m_DbgObj->AddComponent(new CMeshRender);
	m_DbgObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));

	Vector2 vResolution = CDevice::GetInst()->GetRenderResolution();
	m_PostProcessTex = CAssetMgr::GetInst()->CreateTexture(L"PostProcessTex"
		, vResolution.x, vResolution.y
		, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE);


	// Fade Object 초기화
	m_FadeObj = new CGameObject;
	m_FadeObj->AddComponent(new CMeshRender);
	m_FadeObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_FadeObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"FadeMtrl"));

	m_FadeRatio = 0.f;
	m_Fade = false;
}

void CRenderMgr::Tick()
{
	if (CLevelMgr::GetInst()->GetCurrentLevel() == nullptr ||
		CLevelMgr::GetInst()->GetCurrentLevel()->GetState() != LEVEL_STATE::PLAY)
		return;

	auto iter = m_EffObjList.begin();
	for (; iter != m_EffObjList.end(); ++iter)
	{
		if ((*iter)->IsDead())
		{
			m_EffObjList.erase(iter);
			continue;
		}

		if (!(*iter)->IsActive())
			break;

		if (!(*iter)->FlipbookPlayer()->IsPlaying())
		{
			(*iter)->SetActive(false);
		}
	}
}

void CRenderMgr::Render()
{
	Tick();

	// 렌더링 시작
	RenderStart();

	if (m_IsEditor)
	{
		Render_Editor();
	}
	else
	{
		Render_Play();
	}

	Render_Text();

	// DebugRender
	//Render_Debug();


	// Fade 처리
	if (m_Fade)
	{
		if (m_FadeIn)
			m_FadeRatio = min(1.f, m_FadeRatio + DT);
		else
			m_FadeRatio = max(0.f, m_FadeRatio - DT);

		m_FadeObj->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, m_FadeRatio);

		if (m_FadeRatio == 0.f || m_FadeRatio == 1.f)
			m_Fade = false;
	}

	if (m_FadeRatio > 0.f)
		m_FadeObj->Render();
}

void CRenderMgr::RenderStart()
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");

	// Target Clear
	Vector4 vClearColor = Vector4(0.f, 0.f, 0.f, 0.f);
	CONTEXT->ClearRenderTargetView(pRTTex->GetRTV().Get(), vClearColor);
	CONTEXT->ClearDepthStencilView(pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	Vector2 vResolution = CDevice::GetInst()->GetRenderResolution();

	// ViewPort 설정 - Window 화면 영역 지정
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	viewport.Width = vResolution.x;
	viewport.Height = vResolution.y;

	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	CONTEXT->RSSetViewports(1, &viewport);

	// Rendering 목적지를 지정
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());


	// 데이터 및 리소스 바인딩
	Binding();
}

void CRenderMgr::Binding()
{
	static CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::GLOBAL);

	g_Data.Light2DCount = (int)m_vecLight2D.size();

	pCB->SetData(&g_Data);
	pCB->Binding();
	pCB->Binding_CS();

	// 2D 광원 정보 바인딩
	static vector<tLight2DInfo> vecLight2DInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLight2DInfo());
	}

	// 데이터를 받을 구조화버퍼의 크기가 모자라면, Resize 한다.
	if (m_Light2DBuffer->GetElementCount() < vecLight2DInfo.size())
	{
		m_Light2DBuffer->Create(sizeof(tLight2DInfo), (UINT)vecLight2DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	// 광원이 1개 이상인 경우에만 구조화 버퍼 데이터로 이동
	if (!m_vecLight2D.empty())
	{
		m_Light2DBuffer->SetData(vecLight2DInfo.data(), vecLight2DInfo.size());
		m_Light2DBuffer->Binding(13);
	}

	m_vecLight2D.clear();
	vecLight2DInfo.clear();
}

void CRenderMgr::Render_Text()
{
	list<tTextInfo>::iterator iter = m_TextList.begin();

	for (; iter != m_TextList.end(); )
	{
		CFontMgr::GetInst()->DrawFont(iter->Text.c_str(), iter->Position.x, iter->Position.y, iter->FontSize, iter->Color, iter->Align, iter->Font);

		iter->Time -= DT;
		if (iter->Time <= 0)
		{
			iter = m_TextList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CRenderMgr::Render_Play()
{
	// 카메라로 레벨 렌더링
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		if (nullptr == m_vecCam[i])
			continue;

		m_vecCam[i]->SortObject();
		m_vecCam[i]->Render();

		// 현재 렌더링 중인 카메라 등록
		if (!m_vecCam[i]->GetOwner()->IsValid())
			m_vecCam[i] = nullptr;

		m_RenderCam = m_vecCam[i];
	}

	if(m_vecCam.size() > 0)
		m_RenderCam = m_vecCam[0];
}

void CRenderMgr::Render_Editor()
{
	m_RenderCam = m_EditorCam;
	m_EditorCam->SortObject();
	m_EditorCam->Render();


	// 월드 그리드 그리기
	if (m_bGrid)
	{
		int interval = 100;
		float padding = 10.f;
		float scale = m_EditorCam->GetScale();
		float width = m_EditorCam->GetWidth() * scale;
		float height = width / m_EditorCam->GetAspectRatio();
		Vector3 vCamPos = m_EditorCam->Transform()->GetWorldPos();
		Vector2 vCamLT = Vector2(vCamPos.x - width / 2.f, vCamPos.y + height / 2.f);
		Vector2 vCamRB = Vector2(vCamPos.x + width / 2.f, vCamPos.y - height / 2.f);

		if (scale < 1)
			interval = 100;
		else if (scale < 10)
			interval *= pow(2, (int)scale);
		else
			return;

		vCamLT.x += padding;
		vCamLT.y -= padding;
		vCamRB.x -= padding;
		vCamRB.y += padding;


		// 수직선
		for (int x = ((int)vCamLT.x / interval) * interval; x < vCamRB.x; x += interval)
		{
			DrawDebugLine(Vector4(1.f, 1.f, 1.f, 0.5f), Vector3(x, vCamLT.y, 0.f), Vector3(x, vCamRB.y, 0.f));
		}

		// 수평선
		for (int y = ((int)vCamLT.y / interval) * interval; y > vCamRB.y; y -= interval)
		{
			DrawDebugLine(Vector4(1.f, 1.f, 1.f, 0.5f), Vector3(vCamLT.x, y, 0.f), Vector3(vCamRB.x, y, 0.f));
		}

	}

}

void CRenderMgr::Render_Debug()
{
	if (m_RenderCam)
	{
		g_Trans.matView = m_RenderCam->m_matView;
		g_Trans.matProj = m_RenderCam->m_matProj;
	}

	list<tDebugShapeInfo>::iterator iter = m_DbgList.begin();

	for (; iter != m_DbgList.end(); )
	{
		// 모양 설정
		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
			break;
		case DEBUG_SHAPE::RECT_FILLED:
			m_DbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		case DEBUG_SHAPE::CIRCLE_FILLED:
			m_DbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh"));
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		case DEBUG_SHAPE::CROSS:
			break;
		case DEBUG_SHAPE::LINE:
			m_DbgObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"LineMesh"));
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		}

		// 위치설정
		if ((*iter).matWorld == XMMatrixIdentity())
		{
			m_DbgObj->Transform()->SetRelativePos((*iter).WorldPos);
			m_DbgObj->Transform()->SetRelativeScale((*iter).Scale);
			m_DbgObj->Transform()->SetRelativeRotation((*iter).Rotation);
			m_DbgObj->Transform()->FinalTick();
		}
		else
		{
			m_DbgObj->Transform()->SetWorldMat((*iter).matWorld);
		}


		// 색상 설정
		m_DbgObj->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).Color);

		if ((*iter).DepthTest)
		{
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetDSState(DS_TYPE::NO_WRITE);
		}
		else
		{
			m_DbgObj->MeshRender()->GetMaterial()->GetShader()->SetDSState(DS_TYPE::NO_TEST_NO_WRITE);
		}

		// 렌더
		m_DbgObj->Render();

		(*iter).Time += DT;
		if ((*iter).Duration <= (*iter).Time)
		{
			iter = m_DbgList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, UINT _Priority)
{
	// 카메라 제거
	if (-1 == _Priority)
	{
		vector<CCamera*>::iterator iter = m_vecCam.begin();
		for (; iter != m_vecCam.end(); ++iter)
		{
			if (*iter == _Cam)
			{
				m_vecCam.erase(iter);
				return;
			}
		}
	}

	else
	{
		if (m_vecCam.size() <= _Priority)
		{
			m_vecCam.resize(_Priority + 1);
		}

		CCamera* pOther = m_vecCam[_Priority];
		int iOther = _Cam->GetCameraPriority();
		if (-1 != iOther)
		{
			m_vecCam[iOther] = pOther;
		}

		m_vecCam[_Priority] = _Cam;
	}
}

void CRenderMgr::CopyRenderTarget()
{
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}
