#pragma once
#include "CShader.h"


struct tScalarParam
{
    string          Desc;
    SCALAR_PARAM    eParam;
    bool            Drag;
};

struct tTexParam
{
    string          Desc;
    TEX_PARAM       eParam;
};

class CGraphicShader :
    public CShader
{
private:
    // 쉐이더
    // 그래픽 영역에 색을 채워 넣어줌
    ComPtr<ID3DBlob>                m_VSBlob; // 저장소
    ComPtr<ID3DBlob>                m_GSBlob; 
    ComPtr<ID3DBlob>                m_PSBlob; // 해당 메모리들을 활용해 아래 쉐이더들을 만들 것

    ComPtr<ID3D11VertexShader>      m_VS; // 쉐이더
    ComPtr<ID3D11GeometryShader>    m_GS;
    ComPtr<ID3D11PixelShader>       m_PS;

    // Input Layout (입력 구조)
    // 전달시키는 정점의 메모리 구조
    // 정점을 직접 설계했기 때문에 메모리 크기 다 알려줘야 함
    // Vector4, Vector2(Temp) => float3(shader)로 변환하기 위해
    // Vtx 구조체 크기 36바이트, DX 헤더에서 #pragma pack(1) 선언되어 있기 때문
    ComPtr<ID3D11InputLayout> m_Layout;
    D3D11_PRIMITIVE_TOPOLOGY m_Topology;

    RS_TYPE                     m_RSType;   // 레스터라이저 스테이트 옵션
    BS_TYPE                     m_BSType;   // 블랜드 스테이트 옵션
    DS_TYPE                     m_DSType;   // DepthStencilState 옵션

    SHADER_DOMAIN               m_Domain;

    vector<tScalarParam>            m_vecScalarParam;
    vector<tTexParam>               m_vecTexParam;



public:
    int CreateVertexShader(const wstring& _RelativePath, const string& _FuncName);
    int CreatePixelShader(const wstring& _RelativePath, const string& _FuncName);
    int CreateGeometryShader(const wstring& _RelativePath, const string& _FuncName);

public:
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_Topology = _Topology; }
    void SetRSState(RS_TYPE _State) { m_RSType = _State; }
    void SetBSState(BS_TYPE _State) { m_BSType = _State; }
    void SetDSState(DS_TYPE _Type) { m_DSType = _Type; }

    void SetDomain(SHADER_DOMAIN _Domain) { m_Domain = _Domain; }
    SHADER_DOMAIN GetDomain() { return m_Domain; }


    void AddScalarParam(const string& _Desc, SCALAR_PARAM _Param, bool _Drag = false) { m_vecScalarParam.push_back(tScalarParam{ _Desc, _Param, _Drag }); }
    void AddTexParam(const string& _Desc, TEX_PARAM _Param) { m_vecTexParam.push_back(tTexParam{ _Desc, _Param }); }

    const vector<tScalarParam>& GetScalarParam() { return m_vecScalarParam; }
    const vector<tTexParam>& GetTexParam() { return m_vecTexParam; }



    void Binding();

public:
    CLONE_DISABLE(CGraphicShader);
    CGraphicShader();
    ~CGraphicShader();
};

