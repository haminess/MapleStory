#include "pch.h"
#include "CTransform.h"
#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_IndependentScale(false)
	, m_Depth(1.f)
	, m_RelativePos(Vector3(0.f, 0.f, 0.f))
	, m_RelativeRotation(Vector3(0.f, 0.f, 0.f))
	, m_RelativeScale(Vector3(1.f, 1.f, 1.f))
{
}

CTransform::~CTransform()
{
}


void CTransform::SaveComponent(FILE* _File)
{
	fwrite(&m_RelativePos, sizeof(Vector3), 1, _File);
	fwrite(&m_RelativeScale, sizeof(Vector3), 1, _File);
	fwrite(&m_RelativeRotation, sizeof(Vector3), 1, _File);
	fwrite(&m_IndependentScale, sizeof(bool), 1, _File);
}

void CTransform::LoadComponent(FILE* _FILE)
{
	fread(&m_RelativePos, sizeof(Vector3), 1, _FILE);
	fread(&m_RelativeScale, sizeof(Vector3), 1, _FILE);
	fread(&m_RelativeRotation, sizeof(Vector3), 1, _FILE);
	fread(&m_IndependentScale, sizeof(bool), 1, _FILE);
}

void CTransform::FinalTick()
{
	m_PrevPos = GetWorldPos();

	// 단위 행렬
	// 역원, 항등원이 같다
	//	|1	 0	 0	 0|
	//	|0	 1	 0	 0|
	//	|0	 0	 1	 0|
	//	|0	 0	 0	 1|
	m_matWorld = XMMatrixIdentity();

	// 순서 중요!!!
	// 크기 행렬은 대각선
	//				|Sx	 0	 0	 0|		
	//	|a, b, c|	|0	 Sy	 0	 0|	=>	|a * Sx, b * Sy, c * Sz, 1|
	//				|0	 0	 Sz	 0|		
	//				|0	 0	 0	 1|		
	Matrix matScale = XMMatrixScaling(m_RelativeScale.x, m_RelativeScale.y, m_RelativeScale.z);



	// 회전 행렬
	// (x, y) 좌표의 점을 θ만큼 회전 -> (xcos - ysin, xsin + ycos)
	//				|cos	cos	0	0|		xcos - ysin
	//	|x, y, z|	|sin	sin	0	0|	=>	xsin + ycos
	//				|0		0		1|		z		z
	//				|0		0		0|		1		1
	// 각 축에 대한 행렬을 곱해줌
	Matrix matRot = XMMatrixRotationX(m_RelativeRotation.x)
				  * XMMatrixRotationY(m_RelativeRotation.y)
				  * XMMatrixRotationZ(m_RelativeRotation.z);


	// 이동 행렬은 4행
	//				|1	0	0	0|		
	//	|x, y, z|	|0	1	0	0|	=>	|x + Tx, y + Ty, z + Tz, 1|
	//				|0	0	1	0|		
	//				|tX	tY	tZ	1|		
	Matrix matTrans = XMMatrixTranslation(m_RelativePos.x, m_RelativePos.y, m_RelativePos.z);

	// 크기 * 회전 * 이동 순서
	m_matWorld = matScale * matRot * matTrans;

	// 방향 벡터 갱신
	m_LocalDir[(UINT)DIR_TYPE::RIGHT]	= Vector3(1.f, 0.f, 0.f);
	m_LocalDir[(UINT)DIR_TYPE::UP]		= Vector3(0.f, 1.f, 0.f);
	m_LocalDir[(UINT)DIR_TYPE::FRONT]	= Vector3(0.f, 0.f, 1.f);

	for (int i = 0; i < 3; ++i)
	{
		// 회전 행렬만 적용하기 위해 4번째 좌표 동치좌표(이동변환을 위한) 넣지 않는다

		// 1. coord		4번째 좌표를 1로 확장해서 계산
		// 2. normal	4번째 좌표를 0로 확장해서 계산
		//XMVector3TransformCoord(m_LocalDir[i], matRot);
		m_WorldDir[i] = m_LocalDir[i] = XMVector3TransformNormal(m_LocalDir[i], matRot);
	}

	// 부모가 있다면
	if (GetOwner()->GetParent())
	{
		const Matrix& matParentWorld = GetOwner()->GetParent()->Transform()->GetWorldMat();

		// 자식 오브젝트는 부모 오브젝트의 크기에 영향을 받는다.
		if (false == m_IndependentScale)
		{
			m_matWorld *= matParentWorld;
		}

		// 자식 오브젝트가 독립적인 크기를 유지하고 싶은 경우
		else
		{
			Vector3 vParentWorldScale = GetOwner()->GetParent()->Transform()->GetRelativeScale();
			Matrix matParentScale = XMMatrixScaling(vParentWorldScale.x, vParentWorldScale.y, vParentWorldScale.z);
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			// 자식 LocalMat * 부모크기-1 * 부모월드행렬(부모크기 * 부모회전 * 부모이동)
			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}



		// WorldDir(방향 벡터) 구하기
		m_WorldDir[(UINT)DIR_TYPE::RIGHT] = Vector3(1.f, 0.f, 0.f);
		m_WorldDir[(UINT)DIR_TYPE::UP] = Vector3(0.f, 1.f, 0.f);
		m_WorldDir[(UINT)DIR_TYPE::FRONT] = Vector3(0.f, 0.f, 1.f);
		for (int i = 0; i < 3; ++i)
		{
			// 크기 벡터와 이동 벡터는 적용되지 않게 한다.
		
			// 이동벡터는 영향을 받으면 안됨(월드 행렬로 계산하기 때문에)
			// 따라서 Coord가 아니라 Normal로 계산해야 한다.
			m_WorldDir[i] = XMVector3TransformNormal(m_WorldDir[i], m_matWorld);

			// 크기 벡터는 적용될 수밖에 없어 방향 벡터의 크기가 다양하게 반영된다.
			// 따라서 크기가 1이 되도록 방향벡터를 정규화
			m_WorldDir[i].Normalize();
		}
	}
}

Vector3 CTransform::GetWorldScale()
{
	Vector3 vScale = m_RelativeScale;

	if (false == m_IndependentScale)
	{
		CGameObject* pParent = GetOwner()->GetParent();

		while (pParent)
		{
			vScale *= pParent->Transform()->GetRelativeScale();

			if (pParent->Transform()->m_IndependentScale)
				break;
			else
				pParent = pParent->GetParent();
		}
	}

	return vScale;
}

void CTransform::Binding()
{
	CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);

	g_Trans.matWorld = m_matWorld;


	// 뷰 행렬에서 이동 성분만 추출하여 깊이에 따른 시차 계산
	Vector3 viewPos = Vector3(g_Trans.matView._41, g_Trans.matView._42, g_Trans.matView._43);
	Matrix matViewParallax = g_Trans.matView;

	// 깊이값이 0에 가까울수록 이동량을 줄임
	// m_Depth를 0~1 사이값으로 가정
	matViewParallax._41 = viewPos.x * m_Depth;
	matViewParallax._42 = viewPos.y * m_Depth;
	matViewParallax._43 = viewPos.z * m_Depth;


	g_Trans.matWV = g_Trans.matWorld * matViewParallax;
	g_Trans.matWVP = g_Trans.matWV * g_Trans.matProj;

	// 상수 버퍼로 메모리 보내기
	// GPU Memory(DRAM?)으로 이동
	pCB->SetData(&g_Trans);

	// GPU에서 사용할 수 있도록 
	// GPU Memory와 GPU 레지스터를 바인딩(연결)한다.
	pCB->Binding();
}