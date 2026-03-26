#include "pch.h"
#include "CMeshRender.h"
#include "CMaterial.h"

#include "assets.h"
#include "CTransform.h"
#include "CFlipbookPlayer.h"
#include "CAssetMgr.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
}

CMeshRender::CMeshRender(const CMeshRender& _Other)
	: CRenderComponent(_Other)
{
	CreateDynamicMaterial();
	Ptr<CMaterial> pMtrl = GetMaterial();
	for (int i = 0; i <= (int)TEX_PARAM::TEX_5; ++i)
	{
		Ptr<CTexture> pTex = nullptr;
		pTex = _Other.GetMaterial().Get()->GetTexParam((TEX_PARAM)i);
		pMtrl->SetTexParam((TEX_PARAM)i, pTex);
	}
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::Init()
{
	CreateDynamicMaterial();
}

void CMeshRender::Render()
{
	if (FlipbookPlayer())
	{
		FlipbookPlayer()->Binding();
	}

	// 위치정보
	Transform()->Binding();

	// 사용할 쉐이더
	GetMaterial()->Binding();

	// 렌더링
	GetMesh()->Render();

	if (FlipbookPlayer())
	{
		FlipbookPlayer()->Clear();
	}
}

void CMeshRender::FinalTick()
{
}

void CMeshRender::SaveComponent(FILE* _FILE)
{
	CRenderComponent::SaveComponent(_FILE);
	Ptr<CMaterial> pMtrl = GetMaterial();
	for (int i = 0; i <= (int)TEX_PARAM::TEX_5; ++i)
	{
		SaveAssetRef(pMtrl->GetTexParam((TEX_PARAM)i), _FILE);
	}
}

void CMeshRender::LoadComponent(FILE* _FILE)
{
	CRenderComponent::LoadComponent(_FILE);

	// 오브젝트에 공용 Material로 일괄적용 불가 문제 (보류)
	CreateDynamicMaterial();
	Ptr<CMaterial> pMtrl = GetMaterial();
	for (int i = 0; i <= (int)TEX_PARAM::TEX_5; ++i)
	{
		Ptr<CTexture> pTex = nullptr;
		LoadAssetRef(pTex, _FILE);
		pMtrl->SetTexParam((TEX_PARAM)i, pTex);
	}
}
