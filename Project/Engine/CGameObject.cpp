#include "pch.h"
#include "CGameObject.h"
#include "components.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CScript.h"
#include "CRenderComponent.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CTaskMgr.h"
#include "CLayer.h"
#include "CLevel.h"


CGameObject::CGameObject()
	: m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(-1)
	, m_Active(true)
	, m_Valid(true)
	, m_Dead(false)
	, m_Render(false)
{
	// Transform 컴포넌트는 무조건 가져야 되는 기본 컴포넌트
	AddComponent(new CTransform);
	Transform()->SetRelativeScale(1.f, 1.f, 1.f);
}

CGameObject::CGameObject(const CGameObject& _Origin)
	: CEntity(_Origin)
	, m_arrCom{}
	, m_RenderCom(nullptr)
	, m_Parent(nullptr)
	, m_LayerIdx(_Origin.m_LayerIdx)
	, m_Active(true)
	, m_Dead(false)
	, m_Valid(true)
	, m_Render(false)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == _Origin.m_arrCom[i])
			continue;

		AddComponent(_Origin.m_arrCom[i]->Clone());
	}

	for (size_t i = 0; i < _Origin.m_vecScripts.size(); ++i)
	{
		AddComponent(_Origin.m_vecScripts[i]->Clone());
	}

	for (size_t i = 0; i < _Origin.m_vecChild.size(); ++i)
	{
		AddChild(_Origin.m_vecChild[i]->Clone());
	}
}

CGameObject::~CGameObject()
{
	// 같은 구문
	//DeleteArray<CComponent>(m_arrCom);
	DeleteArray(m_arrCom);
	DeleteVec(m_vecScripts);
	DeleteVec(m_vecChild);
}


void CGameObject::Begin()
{
	if (!m_Active)
		return;

	// component
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (!m_arrCom[i])
			continue;

		m_arrCom[i]->Begin();
	}

	// script component
	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		m_vecScripts[i]->Begin();
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Begin();
	}
}

void CGameObject::Tick()
{
	if (!m_Active)
		return;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (!m_arrCom[i] || !m_arrCom[i]->IsActive())
			continue;

		m_arrCom[i]->Tick();
	}

	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		if (!m_vecScripts[i] || !m_vecScripts[i]->IsActive())
			continue;

		m_vecScripts[i]->Tick();
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Tick();
	}
}

void CGameObject::FinalTick()
{
	// Level 등록
	CLevelMgr::GetInst()->RegisterObject(this);

	if (!m_Active)
		return;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (!m_arrCom[i] || !m_arrCom[i]->IsActive())
			continue;

		m_arrCom[i]->FinalTick();
	}


	// 자식만 죽을 경우 따로 Destroy
	vector<CGameObject*>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end(); )
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
		{
			(*iter)->Destroy();
			iter = m_vecChild.erase(iter);
		}
		else
		{
			++iter;
		}
	}


}

void CGameObject::Destroy()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (!m_arrCom[i])
			continue;

		m_arrCom[i]->Destroy();
	}

	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		if (!m_vecScripts[i])
			continue;

		m_vecScripts[i]->Destroy();
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->Destroy();
	}
}

void CGameObject::Render()
{
	if (!m_Active || !m_RenderCom)
		return;

	// 카메라 시야 체크
	//if (!IsInCameraView())
		//return;

	m_RenderCom->Render();
}

bool CGameObject::IsInCameraView()
{
	CCamera* pCam = CRenderMgr::GetInst()->GetRenderCamera();
	if (!pCam)
	{
		m_Render = false;
		return false;
	}

	// 오브젝트의 월드 위치와 스케일 가져오기
	Vector3 vObjPos = Transform()->GetWorldPos();
	Vector3 vObjScale = Transform()->GetWorldScale();

	// 카메라 정보 가져오기
	float camWidth = pCam->GetWidth() * pCam->GetScale();
	float camHeight = camWidth / pCam->GetAspectRatio();
	Vector3 vCamPos = pCam->Transform()->GetWorldPos();

	// 카메라 영역 계산
	float half = 2.f;
	float leftBound = vCamPos.x - (camWidth / half);
	float rightBound = vCamPos.x + (camWidth / half);
	float topBound = vCamPos.y + (camHeight / half);
	float bottomBound = vCamPos.y - (camHeight / half);

	// 오브젝트의 바운딩 박스 계산
	float objLeft = vObjPos.x - (vObjScale.x / half);
	float objRight = vObjPos.x + (vObjScale.x / half);
	float objTop = vObjPos.y + (vObjScale.y / half);
	float objBottom = vObjPos.y - (vObjScale.y / half);

	// AABB 충돌 체크
	if (objRight < leftBound ||
		objLeft > rightBound ||
		objBottom > topBound ||
		objTop < bottomBound)
	{
		m_Render =  false;
		return false;
	}

	m_Render =  true;
	return true;
}

void CGameObject::SetActive(bool _bActive)
{
	if (m_Active == _bActive)
		return;

	m_Active = _bActive;
	m_Valid = m_Active;

	if (_bActive)
		Activate();
	else
		Deactivate();
}

void CGameObject::Activate()
{
	// 활성화 돼 있는 컴포넌트들만 Activate 함수 호출
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (!m_arrCom[i])
			continue;

		m_arrCom[i]->Activate();
	}

	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		if (!m_vecScripts[i])
			continue;

		m_vecScripts[i]->Activate();
	}

	vector<CGameObject*>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end(); ++iter)
	{
		(*iter)->Activate();
	}
}

void CGameObject::Deactivate()
{
	// 활성화 돼 있는 컴포넌트들만 Deactivate 함수 호출
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (!m_arrCom[i] || !m_arrCom[i]->IsActive())
			continue;

		m_arrCom[i]->Deactivate();
	}

	for (size_t i = 0; i < m_vecScripts.size(); ++i)
	{
		if (!m_vecScripts[i] || !m_vecScripts[i]->IsActive())
			continue;

		m_vecScripts[i]->Deactivate();
	}

	vector<CGameObject*>::iterator iter = m_vecChild.begin();
	for (; iter != m_vecChild.end(); ++iter)
	{
		(*iter)->Deactivate();
	}

	// 한 프레임만 더 돌아가도록 등록
	CLevelMgr::GetInst()->RegisterObject(this);
}

CComponent* CGameObject::AddComponent(CComponent* _Component)
{
	COMPONENT_TYPE Type = _Component->GetType();

	if (COMPONENT_TYPE::SCRIPT == Type)
	{
		m_vecScripts.push_back((CScript*)_Component);
	}
	else
	{
		// 입력으로 들어오는 컴포넌트와 이미 동일한 컴포넌트를 오브젝트가 가지고 있는 경우
		if (m_arrCom[(UINT)Type])
		{
			//assert(0);

			delete _Component;
			return nullptr;
		}

		// 입력으로 들어오는 컴포넌트가 렌더링과 관련된 컴포넌트이면 1개만 가질 수 있도록 예외처리 한다.
		if (dynamic_cast<CRenderComponent*>(_Component))
		{
			assert(!m_RenderCom);

			m_RenderCom = (CRenderComponent*)_Component;
		}


		// 입력된 컴포넌트의 주소를 저장
		m_arrCom[(UINT)Type] = _Component;
	}

	// 컴포넌트의 소유오브젝트를 세팅
	_Component->m_Owner = this;

	// 컴포넌트 초기화
	if (_Component->IsInitialized() == false)
	{
		_Component->m_Initialized = true;
		_Component->Init();
	}

	return _Component;
}

CComponent* CGameObject::AddComponent(COMPONENT_TYPE _ComType)
{
	return AddComponent(CComponent::GetComponent(_ComType));
}

void CGameObject::DeleteComponent(COMPONENT_TYPE _ComType)
{
	if (_ComType == COMPONENT_TYPE::SCRIPT)
	{
		assert(0);
		return;
	}

	delete m_arrCom[(UINT)_ComType];
	m_arrCom[(UINT)_ComType] = nullptr;
}

void CGameObject::AddChild(CGameObject* _Child)
{
	m_vecChild.push_back(_Child);
	_Child->m_Parent = this;
	
	// 초기 등록이면 부모 layer 따라가기
	if(_Child->GetLayerIdx() == -1) 
		_Child->SetLayerIdx(GetLayerIdx());

	CTaskMgr::GetInst()->NotifyLevelChanged();
}

bool CGameObject::IsAncestor(CGameObject* _Other)
{
	CGameObject* pParent = m_Parent;

	while (pParent)
	{
		if (pParent == _Other)
			return true;

		pParent = pParent->m_Parent;
	}

	return false;
}
void CGameObject::DisconnectWithLayer()
{
	// 소속 레이어가 없다면
	if (-1 == m_LayerIdx)
		return;

	CLayer* pLayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_LayerIdx);
	pLayer->DisconnectObject(this);
}

void CGameObject::DisconnectWithParent()
{
	if (nullptr == m_Parent)
		return;

	vector<CGameObject*>::iterator iter = m_Parent->m_vecChild.begin();

	for (; iter != m_Parent->m_vecChild.end(); ++iter)
	{
		if (*iter == this)
		{
			m_Parent->m_vecChild.erase(iter);
			m_Parent = nullptr;
			return;
		}
	}

	assert(nullptr);
}

void CGameObject::RegisterAsParent()
{
	// 소속 레이어가 없다면
	if (-1 == m_LayerIdx)
		return;

	CLayer* pLayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_LayerIdx);
	pLayer->RegisterAsParent(this);

}

void CGameObject::SetLayerIdx(int _idx)
{
	m_LayerIdx = _idx;
}