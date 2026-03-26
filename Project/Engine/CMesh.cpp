#include "pch.h"
#include "CMesh.h"
#include "CDevice.h"

CMesh::CMesh(bool _bEngineRes)
	: CAsset(ASSET_TYPE::MESH, _bEngineRes)
	, m_VtxSysMem(nullptr)
	, m_IdxSysMem(nullptr)
	, m_VBDesc{}
	, m_VtxCount(0)
	, m_IBDesc{}
	, m_IdxCount(0)
{
}


CMesh::~CMesh()
{
	DELETE_ARR(m_VtxSysMem);
	DELETE_ARR(m_IdxSysMem);
}

void CMesh::Binding()
{
	// IA
	// 렌더링할 정점 정보 전달
	UINT Stride = sizeof(Vertex);	// 정점 메모리 간격, 개발자마다 설계한 정점의 크기가 다르기 때문에
	UINT Offset = 0;				// 수많은 정점의 일부만 그릴 때 사용

	CONTEXT->IASetVertexBuffers(0, 1 /*버퍼 갯수*/, m_VB.GetAddressOf(), &Stride, &Offset);
	CONTEXT->IASetIndexBuffer(m_IB.Get(), DXGI_FORMAT_R32_UINT /*간격 정보*/, 0);
}

void CMesh::Render()
{
	Binding();

	CONTEXT->DrawIndexed(m_IdxCount, 0, 0);
}

void CMesh::Render_Particle(int _Count)
{
	Binding();

	CONTEXT->DrawIndexedInstanced(m_IdxCount, _Count, 0, 0, 0);
}


int CMesh::Create(Vertex* _Vtx, UINT _VtxCount, UINT* _pIdx, UINT _IdxCount)
{
	// 2. 버텍스 버퍼 생성
	// 정점 데이터를 SysMem -> GPU Mem 로 이동
	assert(nullptr == m_VB && nullptr == m_IB);

	m_VtxCount = _VtxCount;
	m_IdxCount = _IdxCount;

	m_VtxSysMem = new Vertex[_VtxCount];
	memcpy(m_VtxSysMem, _Vtx, sizeof(Vertex) * m_VtxCount);
	m_IdxSysMem = new UINT[_IdxCount];
	memcpy(m_IdxSysMem, _pIdx, sizeof(UINT) * _IdxCount);


	m_VBDesc.ByteWidth = sizeof(Vertex) * _VtxCount;

	// 버퍼의 용도가 정점 정보를 저장하는 용도임을 설정
	// 자기 목적이 아닌 곳에 들어가면 실패됨, 예외 처리 혹은 최적화를 위함?
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	//// 접근할 수 있도록 함, 정점 위치 등 정보를 계속 갱신해야 하기 때문
	//VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_VBDesc.CPUAccessFlags = 0;	// 이제 정점값 수정할 수 없음
	m_VBDesc.Usage = D3D11_USAGE_DEFAULT;

	// 생성시킬 버퍼의 초기데이터 전달용
	D3D11_SUBRESOURCE_DATA tSub = {};

	// 시스템 초기 메모리의 주소
	// 시작 주소만 넘겨오 desc에 사전 정보 다 넘겼기 때문에 자동 설정됨
	tSub.pSysMem = _Vtx;

	// 버텍스 용도의 D3D11 객체 생성
	if (FAILED(DEVICE->CreateBuffer(&m_VBDesc, &tSub, m_VB.GetAddressOf())))
	{
		return E_FAIL;
	}


	// 3. Index Buffer 생성
	// 겹치는 정점 최적화하기 위해 사용
	m_IBDesc.ByteWidth = sizeof(Vertex) * _IdxCount;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.Usage = D3D11_USAGE_DEFAULT;

	tSub.pSysMem = _pIdx;
	if (FAILED(DEVICE->CreateBuffer(&m_IBDesc, &tSub, m_IB.GetAddressOf())))
	{
		return E_FAIL;
	}
}
