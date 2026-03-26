#include "pch.h"
#include "CTexture.h"
#include "CPathMgr.h"
#include "CAssetMgr.h"
#include "CSprite.h"

#include "CDevice.h"

CTexture::CTexture()
	: CAsset(ASSET_TYPE::TEXTURE, false)
	, m_Desc{}
{
}


CTexture::~CTexture()
{
	//m_vecSprite.clear();
	//delete m_OriginSprite;
}


int CTexture::Load(const wstring& _FilePath)
{
	// 파일 -> SystemMem
	wchar_t szExt[50] = {};
	// 주소 나눠주는 함수
	_wsplitpath_s(_FilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);
	wstring strExt = szExt;

	HRESULT hr = E_FAIL;

	if (strExt == L".dds" || strExt == L".DDS")
	{
		// DDS
		hr = LoadFromDDSFile(_FilePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);
	}
	else if (strExt == L".tga" || strExt == L".TGA")
	{
		// TGA
		hr = LoadFromTGAFile(_FilePath.c_str(), nullptr, m_Image);
	}
	else
	{
		// Window Image Component(WIC)
		// png, jpg, jpeg, bmp
		hr = LoadFromWICFile(_FilePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(hr))
	{
		//MessageBox(nullptr, L"텍스쳐 로딩 실패", L"리소스 로딩 실패", MB_OK);
		return E_FAIL;
	}


	// 포맷이 B8G8R8A8_UNORM이 아니면 변환
	if (m_Image.GetMetadata().format != DXGI_FORMAT_B8G8R8A8_UNORM 
	&&  m_Image.GetMetadata().format != DXGI_FORMAT_B8G8R8A8_UNORM_SRGB)
	{
		DirectX::ScratchImage converted;
		hr = Convert(
			m_Image.GetImages(),
			m_Image.GetImageCount(),
			m_Image.GetMetadata(),
			DXGI_FORMAT_B8G8R8A8_UNORM,
			TEX_FILTER_DEFAULT,
			TEX_THRESHOLD_DEFAULT,
			converted
		);

		if (SUCCEEDED(hr))
		{
			m_Image = std::move(converted);
		}
	}



	// System -> GPU
	// m_Image -> m_Tex2D

	// 1. Texture2D 객체 생성
	// 2. Texture2D 를 전달할 때 사용할 ShaderResourceView 생성
	// DepthStencil Texture, View를 생성한 것처럼 수동으로 생성해도 되고
	// 아래 함수로 한번에 처리해도 됨 (리소스 객체 생성 + 리소스 뷰 생성)
	if (FAILED(CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount() /*이미지 크기*/
		, m_Image.GetMetadata(), m_SRV.GetAddressOf())))
	{
		int a = 0;
	}

	// 생성된 ShaderResourceView 를 이용하여 원본 객체(Texture2D) 를 알아낸다.
	// 뷰에서 리소스 텍스처 객체를 가리키기 때문에 거기서 얻어올 수 있다.
	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

	// 생성된 Texture2D 의 Desc 정보를 알아낸다.
	m_Tex2D->GetDesc(&m_Desc);

	// 스프라이트 생성
	CreateOriginSprite(_FilePath);

	return S_OK;
}


int CTexture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat, UINT _BindFlag, D3D11_USAGE _Usage)
{
	m_Desc.Width = _Width;
	m_Desc.Height = _Height;

	m_Desc.ArraySize = 1;
	m_Desc.Format = _PixelFormat;

	// 텍스쳐의 용도
	m_Desc.BindFlags = _BindFlag;

	// CPU 에서 생성 이후에 접근이 가능한지 옵션
	m_Desc.Usage = _Usage;

	if (m_Desc.Usage == D3D11_USAGE_DYNAMIC)
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else if (m_Desc.Usage == D3D11_USAGE_STAGING)
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	// 밉맵 개수가 1 ==> 원본만 존재함 
	m_Desc.MipLevels = 1;

	m_Desc.MiscFlags = 0;
	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;

	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
	{
		return E_FAIL;
	}


	// View 생성	
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		if (FAILED(DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf())))
		{
			return E_FAIL;
		}
	}

	else
	{
		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			// srgb 보정 버전
			D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvDesc.Texture2D.MipSlice = 0;

			if (FAILED(DEVICE->CreateRenderTargetView(m_Tex2D.Get(), &rtvDesc, m_RTV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			if (FAILED(DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

int CTexture::Create(ComPtr<ID3D11Texture2D> _Tex2D)
{
	assert(_Tex2D.Get());

	m_Tex2D = _Tex2D;
	m_Tex2D->GetDesc(&m_Desc);

	// View 생성	
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		if (FAILED(DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf())))
		{
			return E_FAIL;
		}
	}

	else
	{
		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			//// srgb 보정 버전
			//D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			//rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
			//rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			//rtvDesc.Texture2D.MipSlice = 0;

			if (FAILED(DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr /*&rtvDesc*/, m_RTV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		// UAV 생성하기
		if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			if (FAILED(DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

int CTexture::CreateArrayTexture(const vector<Ptr<CTexture>>& _vecTex)
{
	if (_vecTex.empty() || _vecTex[0] == nullptr || !_vecTex[0]->GetDesc().Width)
		return E_FAIL;


	// 텍스처 배열 생성
	m_Desc = _vecTex[0]->GetDesc();
	m_Desc.ArraySize = (UINT)_vecTex.size();
	m_Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	m_Desc.MipLevels = 1;

	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
	{
		return E_FAIL;
	}


	// 원본 각 텍스쳐를 생성된 배열 텍스쳐의 각 칸으로 복사시킨다.
	for (size_t i = 0; i < _vecTex.size(); ++i)
	{
		UINT Offset = D3D11CalcSubresource(0, i, 1);

		CONTEXT->UpdateSubresource(m_Tex2D.Get(), Offset, nullptr
			, _vecTex[i]->GetPixels()
			, _vecTex[i]->GetRowPitch()
			, _vecTex[i]->GetSlicePitch());
	}

	// Shader Resrouce View 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};

	tSRVDesc.Format = m_Desc.Format;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	tSRVDesc.Texture2DArray.MipLevels = 1;
	tSRVDesc.Texture2DArray.MostDetailedMip = 0;
	tSRVDesc.Texture2DArray.ArraySize = m_Desc.ArraySize;

	if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &tSRVDesc, m_SRV.GetAddressOf())))
		return E_FAIL;

	return S_OK;
}

int CTexture::CreateArrayTexture(const vector<Ptr<CTexture>>& vecTex, vector<tTexArraySize>& vecTexSize)
{
	if (vecTex.empty() || vecTex[0] == nullptr)
		return E_FAIL;

	// 가장 큰 텍스처의 크기 찾기
	UINT maxWidth = 0;
	UINT maxHeight = 0;
	for (const auto& tex : vecTex)
	{
		maxWidth = max(maxWidth, tex->GetWidth());
		maxHeight = max(maxHeight, tex->GetHeight());
	}

	// UV 좌표 및 해상도 정보 저장
	vecTexSize.resize(vecTex.size());
	for (size_t i = 0; i < vecTex.size(); ++i)
	{
		// 원본 해상도 저장
		vecTexSize[i].TexResolution = Vector2(
			(float)vecTex[i]->GetWidth(),
			(float)vecTex[i]->GetHeight()
		);

		// UV 비율 계산
		vecTexSize[i].TexSizeUV = Vector2(
			vecTexSize[i].TexResolution.x / (float)maxWidth,
			vecTexSize[i].TexResolution.y / (float)maxHeight
		);
	}

	// 중간 버퍼 생성
	vector<ScratchImage> resizedImages(vecTex.size());

	// 각 텍스처를 동일한 크기로 리사이즈
	for (size_t i = 0; i < vecTex.size(); ++i)
	{
		if (vecTex[i]->GetWidth() != maxWidth || vecTex[i]->GetHeight() != maxHeight)
		{
			// DirectXTex의 Resize 함수 사용
			HRESULT hr = DirectX::Resize(
				*vecTex[i]->m_Image.GetImages(),
				maxWidth,
				maxHeight,
				DirectX::TEX_FILTER_DEFAULT,
				resizedImages[i]);

			if (FAILED(hr))
			{
				wchar_t debugStr[256];
				swprintf_s(debugStr, L"Failed to resize texture %zu: Width=%u, Height=%u\n",
					i, vecTex[i]->GetWidth(), vecTex[i]->GetHeight());
				OutputDebugString(debugStr);
				return E_FAIL;
			}
		}
		else
		{
			// 크기가 같을 경우 새로운 ScratchImage에 복사
			const Image* srcImage = vecTex[i]->m_Image.GetImages();
			HRESULT hr = resizedImages[i].Initialize2D(
				srcImage->format,
				srcImage->width,
				srcImage->height,
				1,
				1);

			if (SUCCEEDED(hr))
			{
				const uint8_t* srcPixels = srcImage->pixels;
				uint8_t* destPixels = resizedImages[i].GetImages()->pixels;
				memcpy(destPixels, srcPixels, srcImage->slicePitch);
			}
			else
			{
				wchar_t debugStr[256];
				swprintf_s(debugStr, L"Failed to copy texture %zu\n", i);
				OutputDebugString(debugStr);
				return E_FAIL;
			}
		}
	}

	// 텍스처 배열 생성
	m_Desc = vecTex[0]->GetDesc();
	m_Desc.Width = maxWidth;
	m_Desc.Height = maxHeight;
	m_Desc.ArraySize = (UINT)vecTex.size();
	m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	m_Desc.MipLevels = 1;
	m_Desc.MiscFlags = 0;
	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;

	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
	{
		OutputDebugString(L"Failed to create Texture2D Array\n");
		return E_FAIL;
	}


	for (size_t i = 0; i < vecTex.size(); ++i)
	{
		// 리사이즈된 이미지 데이터 복사
		UINT subresource = D3D11CalcSubresource(0, i, 1);
		const Image* img = resizedImages[i].GetImages();

		CONTEXT->UpdateSubresource(
			m_Tex2D.Get(),
			subresource,
			nullptr,
			img->pixels,
			img->rowPitch,
			img->slicePitch
		);
	}

	// SRV 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = m_Desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.ArraySize = m_Desc.ArraySize;
	srvDesc.Texture2DArray.FirstArraySlice = 0;

	if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &srvDesc, m_SRV.GetAddressOf())))
	{
		OutputDebugString(L"Failed to create SRV for Texture Array\n");
		return E_FAIL;
	}

	return S_OK;
}



int CTexture::SaveSprites(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	assert(pFile);

	size_t SpriteCount = m_vecSprite.size();
	fwrite(&SpriteCount, sizeof(size_t), 1, pFile);

	for (int i = 0; i < m_vecSprite.size(); ++i)
	{
		m_vecSprite[i]->SaveContinue(pFile);
		m_vecSprite[i]->SetRelativePath(_FilePath);
	}

	fclose(pFile);

	return S_OK;
}



void CTexture::AddSprite(CSprite* _Sprite)
{
	m_vecSprite.push_back(_Sprite);
}


Ptr<CSprite> CTexture::GetOriginSprite() { return m_OriginSprite; }

void CTexture::CreateOriginSprite(const wstring& _FilePath)
{
	if (IsEngineAsset())
		return;

	m_OriginSprite = new CSprite(true);
	m_OriginSprite->SetAtlasTexture(this);
	m_OriginSprite->SetOffset(Vector2(0.f, 0.f));
	m_OriginSprite->SetSlice(GetResolution());
	m_OriginSprite->SetBackground(GetResolution());

	wstring path = CPathMgr::GetInst()->GetRelativePath(_FilePath);
	wstring name = CPathMgr::GetInst()->GetNameFromPath(_FilePath, false) + L".sprite";
	wstring key = CPathMgr::GetInst()->GetFolderFromPath(path) + name;
	m_OriginSprite->SetName(name);
	m_OriginSprite->SetKey(key);
	m_OriginSprite->SetRelativePath(path);
	CAssetMgr::GetInst()->AddAsset<CSprite>(key, m_OriginSprite);
}


void CTexture::Binding(int _RegisterNum)
{
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CTexture::Clear(int _RegisterNum)
{
	// 이전에 사용되었던 레지스터를 비워 렌더링되지 않게 한다
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, &pSRV);
}


void CTexture::Binding_SRV_CS(int _RegisterNum)
{
	m_RecentSRVNum = _RegisterNum;
	CONTEXT->CSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CTexture::Binding_UAV_CS(int _RegisterNum)
{
	assert(m_UAV.Get());

	m_RecentUAVNum = _RegisterNum;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, m_UAV.GetAddressOf(), &i);
}

void CTexture::Clear_SRV_CS()
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->CSSetShaderResources(m_RecentSRVNum, 1, &pSRV);
}

void CTexture::Clear_UAV_CS()
{
	ID3D11UnorderedAccessView* pUAV = nullptr;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(m_RecentUAVNum, 1, &pUAV, &i);
}