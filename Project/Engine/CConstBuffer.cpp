#include "pch.h"
#include "CConstBuffer.h"
#include "CDevice.h"

CConstBuffer::CConstBuffer(CB_TYPE _Type)
	: m_Type(_Type)
	, m_Desc{}
{
}

CConstBuffer::~CConstBuffer()
{
}

int CConstBuffer::Create(UINT _BufferSize)
{
	assert(!(_BufferSize % 16));

	// 4. 상수 버퍼 생성
	m_Desc.ByteWidth = _BufferSize;
	m_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_Desc.Usage = D3D11_USAGE_DYNAMIC;

	// 버텍스 용도의 ID3D11Buffer 객체 생성
	if (FAILED(DEVICE->CreateBuffer(&m_Desc, nullptr, m_CB.GetAddressOf())))
	{
		return E_FAIL;
	}
	return S_OK;
}

void CConstBuffer::Binding()
{
	CONTEXT->VSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->HSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->DSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->GSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
	CONTEXT->PSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
}

void CConstBuffer::Binding_CS()
{
	CONTEXT->CSSetConstantBuffers((UINT)m_Type, 1, m_CB.GetAddressOf());
}

void CConstBuffer::SetData(void* _pData, UINT _DataSize)
{
	// GPU 리소스의 매핑된 데이터를 가리키는 포인터와 그 데이터에 대한 행의 폭 등을 포함하는 구조체
	D3D11_MAPPED_SUBRESOURCE tMapSub = {};

	// GPU 리소스를 CPU에서 접근할 수 있도록 매핑
	// tMapSub.pData->GPU 상수 버퍼, 연결됨
	CONTEXT->Map(m_CB.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &tMapSub);


	if (_DataSize == 0)
	{
		_DataSize = m_Desc.ByteWidth;
	}

	// 연결된 GPU 상수 버퍼에 데이터 전송
	memcpy(tMapSub.pData, _pData, _DataSize);

	// 연결 해제
	CONTEXT->Unmap(m_CB.Get(), 0);
}

