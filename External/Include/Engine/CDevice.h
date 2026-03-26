#pragma once

class CConstBuffer;
class CTexture;
class CDevice
	: public singleton<CDevice>
{
	SINGLE(CDevice);

private:
	HWND								m_hWnd;			// 화면을 그릴 윈도우
	Vector2								m_Resolution;

	ComPtr<ID3D11Device>				m_Device;		// GPU 제어, GPU 메모리에 데이터를 생성, GPU에 뭔가를 만들고 싶을 때 사용, 동적 할당과 유사
	// 생성할 것들 : shader, texture, swapchain, view, buffer, input layout

	ComPtr<ID3D11DeviceContext>			m_Context;		// GPU 제어, GPU 렌더링 동작 명령, Rendering, GPGPU(General Purpose GPU)
	// 렌더링 명령 : clear, get/set, map, draw, copy, shader

	ComPtr<IDXGISwapChain>				m_SwapChain;	// BackBuffer 관리 및 최종 렌더링 제출, 전면/후면 버퍼 소유

	Ptr<CTexture>						m_RTTex;        // RenderTargetTexture
	Ptr<CTexture>						m_DSTex;        // DepthStencilTexture

	// 화면 표시 영역 설정(위치, 크기, 깊이)
	D3D11_VIEWPORT						m_ViewPort;

	// GPU로 데이터 바인딩하기 위한 상수 버퍼
	CConstBuffer*						m_arrCB[(UINT)CB_TYPE::END];

	// 래스터라이저, 정점 3D -> 픽셀 2D 변환
	// 화면 좌표계 변환(3D -> 3D NDC -> 2D 해상도 좌표), 픽셀 생성 -> 컬링
	ComPtr<ID3D11RasterizerState>		m_RSState[(UINT)RS_TYPE::END];

	// 알파블렌드 설정
	ComPtr<ID3D11BlendState>			m_BSState[(UINT)BS_TYPE::END];

	// 샘플러, 텍스쳐에서 픽셀 셰이더로 색상 데이터를 추출하는 객체
	// 색상 데이터 추출방식 설정 (텍스쳐 UV 좌표가 맞지 않을 때 주로 사용)
	ComPtr<ID3D11SamplerState>			m_Sampler[2];

	// 깊이 설정
	ComPtr<ID3D11DepthStencilState>		m_DSState[(UINT)DS_TYPE::END];	//ID3D11BlendState*			m_BSState;

public:
	void Clear();
	void Present();

public:
	ComPtr<ID3D11Device>		GetDevice() { return m_Device; }
	ComPtr<ID3D11DeviceContext> GetContext() { return m_Context; }
	D3D11_VIEWPORT GetViewport() { return m_ViewPort; }

	CConstBuffer* GetCB(CB_TYPE _Type) { return m_arrCB[(UINT)_Type]; }
	ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _Type) { return m_RSState[(UINT)_Type]; }
	ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _Type) { return m_BSState[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_DSState[(UINT)_Type]; }

public:
	Vector2 GetRenderResolution() { return m_Resolution; }

public:
	int Init(HWND _hWnd, Vector2 _Resolution);

private:
	int CreateSwapChain();
	int CreateView();
	int CreateConstBuffer();
	int CreateRasterizerState();
	int CreateSamplerState();
	int CreateBlendState();
	int CreateDepthStencilState();
};

