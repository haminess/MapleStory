#include "pch.h"
#include "CDevice.h"

#include "CAssetMgr.h"
#include "CConstBuffer.h"

CDevice::CDevice()
	: m_ViewPort{}
	, m_arrCB{}
	, m_RSState{}
	, m_BSState{}
	, m_Sampler{}
{}

CDevice::~CDevice()
{
	for (UINT i = 0; i < (UINT)CB_TYPE::END; ++i)
	{
		DELETE(m_arrCB[i]);
	}
}

int CDevice::Init(HWND _hWnd, Vector2 _Resolution)
{
	m_hWnd = _hWnd;
	m_Resolution = _Resolution;

	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL Level = D3D_FEATURE_LEVEL_11_0;

	// 1. Devcie, Context 생성하기
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
		, iFlag, nullptr, 0, D3D11_SDK_VERSION
		, m_Device.GetAddressOf(), &Level, m_Context.GetAddressOf())))
	{
		return E_FAIL;
	}


	// 2. SwapChain 생성하기
	if (FAILED(CreateSwapChain())) return E_FAIL;


	// 3. RenderTarget, DepthStencilTex, View 생성하기
	if (FAILED(CreateView())) return E_FAIL;

	// 4. State 생성하기
	if (FAILED(CreateRasterizerState())) return E_FAIL;
	if (FAILED(CreateSamplerState())) return E_FAIL;
	if (FAILED(CreateBlendState())) return E_FAIL;
	if (FAILED(CreateDepthStencilState())) return E_FAIL;


	// 5. ViewPort 설정 - Window 화면 영역 지정
	// Viewport는 3D 렌더링된 객체가 실제로 화면에 표시되는 영역
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;

	m_ViewPort.Width = m_Resolution.x;
	m_ViewPort.Height = m_Resolution.y;

	m_ViewPort.MinDepth = 0.f;
	m_ViewPort.MaxDepth = 1.f;

	m_Context->RSSetViewports(1, &m_ViewPort);

	// Rendering 목적지를 지정
	m_Context->OMSetRenderTargets(1, m_RTTex->GetRTV().GetAddressOf(), m_DSTex->GetDSV().Get());

	// 6. 상수버퍼 생성
	if (FAILED(CreateConstBuffer())) return E_FAIL;

	g_Data.RenderResolution = m_Resolution;

	// 디버깅 설정
	m_Device->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Device") - 1, "Device");
	m_Context->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Context") - 1, "Context");
	m_SwapChain->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("SwapChain") - 1, "SwapChain");
	m_RTTex->GetRTV()->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("RTTex") - 1, "RTTex");
	m_DSTex->GetDSV()->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("DSTex") - 1, "DSTex");

	for (int i = 0; i < 2; ++i)
	{
		if (nullptr == m_Sampler[i])
			continue;
		m_Sampler[i]->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Sampler" + i) - 1, "Sampler" + i);
	}
	for (int i = 0; i < (int)BS_TYPE::END; ++i)
	{
		if (nullptr == m_BSState[i])
			continue;
		m_BSState[i]->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("BSState" + i) - 1, "BSState" + i);
	}
	for (int i = 0; i < (int)DS_TYPE::END; ++i)
	{
		if (nullptr == m_DSState[i])
			continue;
		m_DSState[i]->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("DSState" + i) - 1, "DSState" + i);
	}
	for (int i = 0; i < (int)RS_TYPE::END; ++i)
	{
		if (nullptr == m_RSState[i])
			continue;
		m_RSState[i]->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("RSState" + i) - 1, "RSState" + i);
	}





	// 어댑터 열거
	// https://lipcoder.tistory.com/24
	// 어뎁터에 접근할 index 변수
	//UINT i = 0;
	//// Adapter를 담아둘 포인터 변수
	//IDXGIAdapter* adapter = nullptr;
	//// Adapter들을 저장할 vector 컨테이너
	//std::vector<IDXGIAdapter*> adapterList;

	//// IDXGIFactory 객체를 통해 어뎁터들을 열거한다. i를 index로 사용한다.
	//// 만약 index가 시스템에 존재하는 어뎁터의 갯수와 같거나 더 크다면 while문을 종료한다.
	//while (mdxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	//{
	//	// 어뎁터 정보 구조체
	//	DXGI_ADAPTER_DESC desc;
	//	adapter->GetDesc(&desc);

	//	// 어뎁터의 이름을 출력한다. (GPU 정보)
	//	std::wstring text = L"***Adapter: ";
	//	test += desc.Description;
	//	text += L"\n";

	//	OutputDebugString(text.c_str());

	//	// 어뎁터 정보를 컨테이너에 담는다.
	//	adapterList.push_back(adapter);

	//	++i;
	//}

	//// adapter의 모든 출력을 열거하고 release한다.
	//for (size_t i = 0; i < adapterList.size(); ++i)
	//{
	//	LogAdapterOutputs(adapterList[i]);
	//	ReleaseCom(adapterList[i]);
	//}
	

	return S_OK;
}

int CDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC Desc = {};

	Desc.BufferCount = 1;                               // 백버퍼 개수
	Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Swapchain 백버퍼 텍스쳐의 용도 설정

	Desc.BufferDesc.Width = m_Resolution.x;             // 백버퍼 해상도 Width
	Desc.BufferDesc.Height = m_Resolution.y;            // 백버퍼 해상도 Height
	Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	Desc.BufferDesc.RefreshRate.Denominator = 1;        // 화면 갱신 속도, 분모
	Desc.BufferDesc.RefreshRate.Numerator = 60;         // 화면 갱신 속도, 분자

	Desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	Desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	Desc.OutputWindow = m_hWnd;                         // 백버퍼에 그려진 이미지를 출력시킬 윈도우
	Desc.Windowed = true;                               // 창모드, 전체화면 모드

	Desc.SampleDesc.Count = 1;
	Desc.SampleDesc.Quality = 0;

	Desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;         // 이전에 그려진 이미지를 저장하지 않음
	Desc.Flags = 0;

	ComPtr<IDXGIDevice>    IDXGIDevice = nullptr;
	ComPtr<IDXGIAdapter>   pAdapter = nullptr;
	ComPtr<IDXGIFactory>   pFactory = nullptr;

	// 128 비트
	// GUID (Global Unique ID)
	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)IDXGIDevice.GetAddressOf());
	IDXGIDevice->GetAdapter(pAdapter.GetAddressOf());
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	if (FAILED(pFactory->CreateSwapChain(m_Device.Get(), &Desc, m_SwapChain.GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CDevice::CreateView()
{
	// 1. RenderTarget 텍스쳐   
	ComPtr<ID3D11Texture2D> pTex2D = nullptr;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pTex2D.GetAddressOf());
	m_RTTex = CAssetMgr::GetInst()->CreateTexture(L"RenderTargetTex", pTex2D);

	// 2. DepthStencil 텍스쳐
	m_DSTex = CAssetMgr::GetInst()->CreateTexture(L"DepthStencilTex", m_Resolution.x, m_Resolution.y, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);

	return S_OK;
}

int CDevice::CreateConstBuffer()
{
	m_arrCB[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer(CB_TYPE::TRANSFORM);
	m_arrCB[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(TransInfo));

	m_arrCB[(UINT)CB_TYPE::MATERIAL] = new CConstBuffer(CB_TYPE::MATERIAL);
	m_arrCB[(UINT)CB_TYPE::MATERIAL]->Create(sizeof(MtrlConst));

	m_arrCB[(UINT)CB_TYPE::SPRITE] = new CConstBuffer(CB_TYPE::SPRITE);
	m_arrCB[(UINT)CB_TYPE::SPRITE]->Create(sizeof(SpriteInfo));

	m_arrCB[(UINT)CB_TYPE::GLOBAL] = new CConstBuffer(CB_TYPE::GLOBAL);
	m_arrCB[(UINT)CB_TYPE::GLOBAL]->Create(sizeof(GlobalData));

	return S_OK;
}

int CDevice::CreateRasterizerState()
{
	// CULL_BACK
	m_RSState[(UINT)RS_TYPE::CULL_BACK] = nullptr;

	// CULL_FRONT
	D3D11_RASTERIZER_DESC Desc = {};
	Desc.CullMode = D3D11_CULL_FRONT;
	Desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

	// CULL_NONE
	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

	// WIRE_FRAME
	Desc.CullMode = D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_WIREFRAME;
	DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());

	return S_OK;
}

int CDevice::CreateSamplerState()
{
	D3D11_SAMPLER_DESC Desc = {};

	// 1. 텍스쳐 접근 모드
	// 텍스처 경계 밖에 있는([0.0, 1.0] 범위를 벗어난) 텍스처 좌표 추출 설정
	D3D11_TEXTURE_ADDRESS_MODE mode;
	mode = D3D11_TEXTURE_ADDRESS_WRAP;			// 텍스처를 타일화
	mode = D3D11_TEXTURE_ADDRESS_MIRROR;		// 텍스처를 대칭 이동
	mode = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;	// 
	mode = D3D11_TEXTURE_ADDRESS_CLAMP;			// 0.0 또는 1.0의 텍스처 색으로 설정
	mode = D3D11_TEXTURE_ADDRESS_BORDER;		// desc 또는 HLSL 코드에 지정된 테두리 색으로 설정

	// 2. 이방성 필터링 샘플러
	// 2-1) 동일한 텍셀 데이터 사용
	Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.Filter = D3D11_FILTER_ANISOTROPIC;
	DEVICE->CreateSamplerState(&Desc, m_Sampler[0].GetAddressOf());
	
	// 2-2) MIN_MAG_POINT 필터
	// 축소 및 확대를 위해 선형 보간 사용
	Desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	DEVICE->CreateSamplerState(&Desc, m_Sampler[1].GetAddressOf());

	CONTEXT->VSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->HSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->DSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->GSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->PSSetSamplers(0 /*샘플러 버퍼 번호*/, 1 /*샘플러 수*/, m_Sampler[0].GetAddressOf());
	CONTEXT->CSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());

	CONTEXT->VSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->HSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->DSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->GSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->PSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->CSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());

	return S_OK;
}

int CDevice::CreateBlendState()
{
	m_BSState[(UINT)BS_TYPE::DEFAULT] = nullptr;

	D3D11_BLEND_DESC Desc = {};

	Desc.AlphaToCoverageEnable = false;

	// 여러개의 렌더타겟에 독립 개별 적용할 것인지 일괄 적용할 것인지
	// 개별 적용할 것이면 렌더타겟마다 옵션을 설정해주어야 한다.
	// 일괄 적용할 것이면 0번 렌더타겟만 설정
	Desc.IndependentBlendEnable = false;
	Desc.RenderTarget[0].BlendEnable = true;                // 블렌드 사용 여부

	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;          // 블렌딩 방식을 색상끼리 더하기 방식으로 사용
	// Source : PixelShader return 값(RGB), 에 곱할 계수 방식을 설정
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;      // 계수를 Alpha로 하여 RGB에 Alpha를 곱해서 더함
	// Destination : 색상이 출력될 렌더타겟, 블렌드 될 원본 RGB값 보관
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // RGB에 (1-Alpha)를 곱해서 더함

	// 합쳐진 RGBA에서 Alpha에 들어갈 값 방식
	// 크게 의미 없기 때문에 아무 값이나 넣음
	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO /*D3D11_BLEND_ONE도 가능*/;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;


	if (FAILED(DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ALPHABLEND].GetAddressOf())))
	{
		return E_FAIL;
	}

	return S_OK;
}

int CDevice::CreateDepthStencilState()
{
	// Less
	m_DSState[(UINT)DS_TYPE::LESS] = nullptr;

	// LessEqual
	D3D11_DEPTH_STENCIL_DESC Desc = {};

	Desc.StencilEnable = false;
	Desc.DepthEnable = true;                            // 깊이판정 진행
	Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;       // 작거나 같은경우 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // 깊이 기록

	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());

	// Greater
	Desc.StencilEnable = false;
	Desc.DepthEnable = true;                            // 깊이판정 진행
	Desc.DepthFunc = D3D11_COMPARISON_GREATER;          // 깊이가 큰 경우 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // 깊이 기록

	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::GREATER].GetAddressOf());

	// NoTest
	Desc.StencilEnable = false;
	Desc.DepthEnable = false;                           // 깊이판정 수행하지 않음
	Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;           // 항상 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;   // 깊이 기록

	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST].GetAddressOf());

	// NoWrite
	Desc.StencilEnable = false;
	Desc.DepthEnable = true;                            // 깊이판정 진행
	Desc.DepthFunc = D3D11_COMPARISON_LESS;             // 작은경우 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  // 깊이 기록하지 않음

	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());

	// NoTest NoWrite
	Desc.StencilEnable = false;
	Desc.DepthEnable = false;                           // 깊이판정 X
	Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;           // 항상 통과
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;  // 깊이 기록하지 않음

	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());

	return S_OK;
}



void CDevice::Clear()
{
	// RenderTarget 및 DepthStencilTexture 클리어
	Vector4 vClearColor = Vector4(0.5f, 0.5f, 0.5f, 0.f);
	m_Context->ClearRenderTargetView(m_RTTex->GetRTV().Get(), vClearColor);
	m_Context->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}

void CDevice::Present()
{
	// 화면(윈도우) 에 RenderTarget 에 그려진 이미지를 출력시킨다.
	m_SwapChain->Present(0, 0);
}