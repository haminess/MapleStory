#include "pch.h"
#include "CCamera.h"
#include "CRenderMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CTransform.h"
#include "CDevice.h"
#include "components.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_Priority(-1)
	, m_Far(10000.f)
	, m_AspectRatio(1.f)
	, m_LayerCheck(0)
	, m_FOV(XM_PI / 2.f/*90도*/)
	, m_Width(0)
	, m_Scale(1.f)
{
	Vector2 vResolution = CDevice::GetInst()->GetRenderResolution();
	m_Width = (UINT)vResolution.x;
	m_AspectRatio = vResolution.x / vResolution.y;
}

CCamera::CCamera(const CCamera& _Origin)
	: CComponent(_Origin)
	, m_ProjType(_Origin.m_ProjType)
	, m_Priority(_Origin.m_Priority)
	, m_Far(_Origin.m_Far)
	, m_AspectRatio(_Origin.m_AspectRatio)
	, m_LayerCheck(_Origin.m_LayerCheck)
	, m_FOV(_Origin.m_FOV)
	, m_Width(_Origin.m_Width)
	, m_Scale(_Origin.m_Scale)
{
}

CCamera::~CCamera()
{
}



void CCamera::SaveComponent(FILE* _File)
{
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_AspectRatio, sizeof(float), 1, _File);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _File);
	fwrite(&m_Priority, sizeof(int), 1, _File);
	fwrite(&m_FOV, sizeof(float), 1, _File);
	fwrite(&m_Width, sizeof(float), 1, _File);
	fwrite(&m_Scale, sizeof(float), 1, _File);
}

void CCamera::LoadComponent(FILE* _File)
{
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_AspectRatio, sizeof(float), 1, _File);
	fread(&m_LayerCheck, sizeof(UINT), 1, _File);
	fread(&m_Priority, sizeof(int), 1, _File);
	fread(&m_FOV, sizeof(float), 1, _File);
	fread(&m_Width, sizeof(float), 1, _File);
	fread(&m_Scale, sizeof(float), 1, _File);
}


void CCamera::Begin()
{
	CRenderMgr::GetInst()->RegisterCamera(this, m_Priority);
}

void CCamera::FinalTick()
{
	// ViewSpace
	// 카메라가 바라보는 시점으로 좌표계 변환하게 해줌
	// 1. 원점에 카메라
	// 2. z 축이 카메라가 바라보는 전방방향
	// 즉, 먼저 카메라에서 원점까지의 벡터만큼 모든 오브젝트 이동시켜줌
	// 카메라가 원점이 됐다면 카메라의 Z축과 월드 좌표계의 Z축만큼 회전시켜줌

	// 뷰행렬 계산
	// 1. 원점으로 이동
	Vector3 vWorldPos = Transform()->GetWorldPos();
	Matrix matTrans = XMMatrixTranslation(-vWorldPos.x, -vWorldPos.y, -vWorldPos.z);
	m_matView = matTrans;

	// 2. 원점 Z축으로 회전 => 뷰행렬
	Matrix matRot = XMMatrixIdentity();

	Vector3 vR = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vector3 vU = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vector3 vF = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	matRot._11 = vR.x;	matRot._12 = vU.x;	matRot._13 = vF.x;
	matRot._21 = vR.y;	matRot._22 = vU.y;	matRot._23 = vF.y;
	matRot._31 = vR.z;	matRot._32 = vU.z;	matRot._33 = vF.z;

	m_matView = matTrans * matRot;


	// 투영 행렬 계산
	// 3차원 좌표계를 화면 ndc 좌표계로 변환하게 해줌
	// 직교투영
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
		m_matProj = XMMatrixOrthographicLH(m_Width * m_Scale, (m_Width / m_AspectRatio) * m_Scale, 1.f, m_Far);
	// 원근 투영
	else
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV/*시야각*/, m_AspectRatio/*종횡비*/, 1.f, m_Far);
}

void CCamera::LayerCheck(int _LayerIdx)
{
	if (m_LayerCheck & (1 << _LayerIdx))
	{
		m_LayerCheck &= ~(1 << _LayerIdx);
	}

	else
	{
		m_LayerCheck |= (1 << _LayerIdx);
	}
}



void CCamera::SortObject()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// 찍고 싶은 레이어만 렌더링한다.
		if (m_LayerCheck & (1 << i))
		{
			CLayer* pLayer = pCurLevel->GetLayer(i);

			const vector<CGameObject*>& vecObjects = pLayer->GetObjects();

			for (size_t j = 0; j < vecObjects.size(); ++j)
			{
				CRenderComponent* pRenderCom = vecObjects[j]->GetRenderComponent();

				// 분류 예외조건 검사
				if (nullptr == pRenderCom
					|| nullptr == pRenderCom->GetMesh()
					|| nullptr == pRenderCom->GetMaterial()
					|| nullptr == pRenderCom->GetMaterial()->GetShader())
					continue;

				SHADER_DOMAIN domain = pRenderCom->GetMaterial()->GetShader()->GetDomain();

				switch (domain)
				{
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
					m_vecOpaque.push_back(vecObjects[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_MASK:
					m_vecMask.push_back(vecObjects[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
					m_vecTransparent.push_back(vecObjects[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_PARTICLE:
					m_vecParticle.push_back(vecObjects[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
					m_vecPostprocess.push_back(vecObjects[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_NONE:
					// domain_none이면 렌더링 안됨
					break;
				default:
					assert(nullptr);
				}
			}
		}
	}
}

void CCamera::Render()
{
	// 카메라 뷰, 프로젝트 행렬 같이 바인딩해
	// transform 바인딩에서 register로 보냄

	g_Trans.matView = m_matView;
	g_Trans.matProj = m_matProj;

	// 불투명
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->Render();
	}

	// Mask
	for (size_t i = 0; i < m_vecMask.size(); ++i)
	{
		m_vecMask[i]->Render();
	}

	// 반투명
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->Render();
	}

	// 파티클
	for (size_t i = 0; i < m_vecParticle.size(); ++i)
	{
		m_vecParticle[i]->Render();
	}

	// 후처리	
	for (size_t i = 0; i < m_vecPostprocess.size(); ++i)
	{
		CRenderMgr::GetInst()->CopyRenderTarget();
		m_vecPostprocess[i]->Render();
	}


	// clear
	m_vecOpaque.clear();
	m_vecMask.clear();
	m_vecTransparent.clear();
	m_vecParticle.clear();
	m_vecPostprocess.clear();

	// clear하지 않으면 같은 물체가 쌓여 계속 그려질 것
}

void CCamera::SetPriority(int _Priority)
{
	//CRenderMgr::GetInst()->RegisterCamera(this, _Priority);
	m_Priority = _Priority;
}