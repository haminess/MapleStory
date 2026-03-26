#pragma once
#include "CAsset.h"

// 이미지 확장자
// 압축 방식에 따라 이미지 포맷 확장자가 달라짐
// 
// png, jpg, jpeg, tga, dds 등등

struct tTexArraySize
{
    Vector2 TexSizeUV;
    Vector2 TexResolution;
};

class CSprite;
class CTexture :
    public CAsset
{
private:
    ScratchImage                        m_Image;    // 이미지 파일로딩 및 저장 기능
    ComPtr<ID3D11Texture2D>             m_Tex2D;    // SysMem(ScratchImage) -> GPUMem (Resource의 자식은 GPU 메모리에 존재하는) 텍스쳐

    ComPtr<ID3D11RenderTargetView>      m_RTV;
    ComPtr<ID3D11DepthStencilView>      m_DSV;
    ComPtr<ID3D11ShaderResourceView>    m_SRV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    D3D11_TEXTURE2D_DESC                m_Desc;

    int                                 m_RecentSRVNum;
    int                                 m_RecentUAVNum;

    CSprite*                            m_OriginSprite;
    vector<CSprite*>                    m_vecSprite;


public:
    UINT GetWidth() { return m_Desc.Width; }
    UINT GetHeight() { return m_Desc.Height; }
    tPixel* GetPixels() { return (tPixel*)m_Image.GetPixels(); }

    Vector2 GetResolution() { return Vector2(m_Desc.Width, m_Desc.Height); }
    float GetAspectRatio() { return (float)m_Desc.Width / m_Desc.Height; }

    ComPtr<ID3D11Texture2D> GetTex2D() { return m_Tex2D; }
    ComPtr<ID3D11RenderTargetView> GetRTV() { return m_RTV; }
    ComPtr<ID3D11DepthStencilView> GetDSV() { return m_DSV; }
    ComPtr<ID3D11ShaderResourceView> GetSRV() { return m_SRV; }

    D3D11_TEXTURE2D_DESC GetDesc() { return m_Desc; }
    UINT GetRowPitch() { return (UINT)m_Image.GetImages()->rowPitch; }      // 텍스쳐 행의 바이트 크기
    UINT GetSlicePitch() { return (UINT)m_Image.GetImages()->slicePitch; }  // 텍스쳐 면의 바이트 크기

public:
    int CreateArrayTexture(const vector<Ptr<CTexture>>& _vecTex);
    int CreateArrayTexture(const vector<Ptr<CTexture>>& _vecTex, vector<tTexArraySize>& _vecTexSize);

    Ptr<CSprite> GetOriginSprite();
    void CreateOriginSprite(const wstring& _FilePath);

    void AddSprite(CSprite* _Sprite);
    const vector<CSprite*>& GetSprites() { return m_vecSprite; }
    vector<CSprite*>& GetSpritesForEdit() { return m_vecSprite; }
    
public:
    void Binding(int _RegisterNum);
    static void Clear(int _RegisterNum);

    void Binding_SRV_CS(int _RegisterNum);
    void Binding_UAV_CS(int _RegisterNum);

    void Clear_SRV_CS();
    void Clear_UAV_CS();


private:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _FilePath) override { return S_OK; }

    int Create(UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat, UINT _BindFlag, D3D11_USAGE _Usage = D3D11_USAGE_DEFAULT);
    int Create(ComPtr<ID3D11Texture2D> _Tex2D);

public:
    int SaveSprites(const wstring& _FilePath);

public:
    CLONE_DISABLE(CTexture);
    CTexture();
    ~CTexture();

    friend class CAssetMgr;
};

