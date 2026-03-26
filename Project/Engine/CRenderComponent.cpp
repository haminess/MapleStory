#include "pch.h"
#include "CRenderComponent.h"


CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _Origin)
	: CComponent(_Origin)
	, m_Mesh(_Origin.m_Mesh)
	, m_SharedMtrl(_Origin.m_SharedMtrl)
{
	// 원본 객체가 동적재질이 없는 경우
	if (nullptr == _Origin.m_DynamicMtrl)
	{
		m_CurMtrl = m_SharedMtrl;
	}

	// 원본 객체가 동적 재질이 있는 경우
	else
	{
		CreateDynamicMaterial();
	}
}

CRenderComponent::~CRenderComponent()
{
}


void CRenderComponent::SaveComponent(FILE* _File)
{
	SaveAssetRef(m_Mesh, _File);
	SaveAssetRef(m_SharedMtrl, _File);
	SaveAssetRef(m_CurMtrl, _File);
}

void CRenderComponent::LoadComponent(FILE* _File)
{
	LoadAssetRef(m_Mesh, _File);
	LoadAssetRef(m_SharedMtrl, _File);
	LoadAssetRef(m_CurMtrl, _File);
}



// 원본 재질을 등록하는 함수
void CRenderComponent::SetMaterial(Ptr<CMaterial> _Material)
{
	// 렌더 컴포넌트는 원본 재질만 세팅 받을 수 있다.
	assert(!_Material->GetSharedMtrl().Get());

	m_CurMtrl = m_SharedMtrl = _Material;
	CreateDynamicMaterial();
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{
	m_CurMtrl = m_SharedMtrl;
	return m_SharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	if (m_DynamicMtrl.Get())
	{
		m_CurMtrl = m_DynamicMtrl;
	}
	else
	{
		CreateDynamicMaterial();
	}

	return m_DynamicMtrl;
}

void CRenderComponent::CreateDynamicMaterial()
{
	assert(m_SharedMtrl.Get());

	//if (nullptr != m_DynamicMtrl)
	//{
	//	m_CurMtrl = m_DynamicMtrl = m_DynamicMtrl->Clone();
	//}
	//else
	//{
	//	m_CurMtrl = m_DynamicMtrl = m_SharedMtrl->Clone();
	//}


	m_CurMtrl = m_DynamicMtrl = m_SharedMtrl->Clone();
}

