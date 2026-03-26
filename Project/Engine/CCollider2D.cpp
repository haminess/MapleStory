#include "pch.h"
#include "CCollider2D.h"
#include "CTransform.h"
#include "CScript.h"

CCollider2D::CCollider2D()
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_Offset(Vector2(0.f, 0.f))
	, m_Scale(Vector2(1.f, 1.f))
	, m_FinalPos(Vector2(0.f, 0.f))
	, m_IndependentScale(false)
	, m_Type(COLLIDER_TYPE::BOX)
	, m_OverlapCount(0)
	, m_IsCollidable(false)
	, m_IsMovable(false)
	, m_Timer(-1.f)
{
	SetActive(true);
}


CCollider2D::CCollider2D(const CCollider2D& _Origin)
	: CComponent(_Origin)
	, m_Offset(_Origin.m_Offset)
	, m_Scale(_Origin.m_Scale)
	, m_FinalPos(_Origin.m_FinalPos)
	, m_IndependentScale(_Origin.m_IndependentScale)
	, m_OverlapCount(0)
	, m_Timer(-1.f)
{
}

CCollider2D::~CCollider2D()
{
}


void CCollider2D::SaveComponent(FILE* _File)
{
	fwrite(&m_Offset, sizeof(Vector2), 1, _File);
	fwrite(&m_Scale, sizeof(Vector2), 1, _File);
	fwrite(&m_FinalPos, sizeof(Vector2), 1, _File);
	fwrite(&m_IndependentScale, sizeof(bool), 1, _File);
}

void CCollider2D::LoadComponent(FILE* _File)
{
	fread(&m_Offset, sizeof(Vector2), 1, _File);
	fread(&m_Scale, sizeof(Vector2), 1, _File);
	fread(&m_FinalPos, sizeof(Vector2), 1, _File);
	fread(&m_IndependentScale, sizeof(bool), 1, _File);
}


void CCollider2D::FinalTick()
{
	// 크기 * 회전 * 이동
	Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, 1.f);
	Matrix matTrans = XMMatrixTranslation(m_Offset.x, m_Offset.y, 0.f);

	if (m_IndependentScale)
	{
		Vector3 vObjectScale = GetOwner()->Transform()->GetWorldScale();
		if (vObjectScale.x != 0 && vObjectScale.y != 0 && vObjectScale.z != 0)
		{
			Matrix matScaleInv = XMMatrixInverse(nullptr, XMMatrixScaling(vObjectScale.x, vObjectScale.y, vObjectScale.z));
			m_matColliderWorld = matScale * matTrans * matScaleInv * GetOwner()->Transform()->GetWorldMat();
		}
		else
		{
			m_matColliderWorld = matScale * GetOwner()->Transform()->GetWorldMat();
		}

	}
	else
	{
		m_matColliderWorld = matScale * matTrans * GetOwner()->Transform()->GetWorldMat();
	}

	if (!IsActive())
		return;

	// DebugRender 요청, 충돌체를 화면에 표시하도록 함
	switch (m_Type)
	{
	case COLLIDER_TYPE::BOX:
		BoxRender();
		break;

	case COLLIDER_TYPE::LINE:
		LineRender();
		break;
	}

	if (m_Timer > 0.f)
	{
		m_Timer -= DT;
		if (m_Timer <= 0.f)
		{
			SetActive(false);
		}
	}
}
void CCollider2D::BoxRender()
{

	if (m_OverlapCount)
	{
		DrawDebugRect(Vector4(1.f, 0.f, 0.f, 1.f), m_matColliderWorld, false, 0.f);
	}
	else
	{
		DrawDebugRect(Vector4(0.f, 1.f, 0.f, 1.f), m_matColliderWorld, false, 0.f);
	}
}

void CCollider2D::LineRender()
{
	XMVECTOR s, r, t;
	XMMatrixDecompose(&s, &r, &t, m_matColliderWorld);

	Vector3 pos = t, scale = s;
	Vector3 leftTop = pos - Vector3(scale / 2.f);
	Vector3 rightBottom = pos + Vector3(scale / 2.f);

	if (m_OverlapCount)
	{
		DrawDebugLine(Vector4(1.f, 0.f, 0.f, 1.f), leftTop, rightBottom, false, 0.f);
	}
	else
	{
		DrawDebugLine(Vector4(0.f, 1.f, 0.f, 1.f), leftTop, rightBottom, false, 0.f);
	}
}



void CCollider2D::BeginOverlap(CCollider2D* _Other)
{
	++m_OverlapCount;

	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->BeginOverlap(this, _Other->GetOwner(), _Other);
	}
}

void CCollider2D::Overlap(CCollider2D* _Other)
{
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->Overlap(this, _Other->GetOwner(), _Other);
	}
}

void CCollider2D::EndOverlap(CCollider2D* _Other)
{
	--m_OverlapCount;

	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->EndOverlap(this, _Other->GetOwner(), _Other);
	}
}