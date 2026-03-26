#pragma once
#include "CAsset.h"

// 1. Graphic Shader => 렌더링 하는 쉐이더
// 2. Compute Shader => 렌더링하지 않는 쉐이더, 대량 연산 시킴

class CShader :
    public CAsset
{
protected:
    ComPtr<ID3DBlob> m_ErrBlob;	// 컴파일 실패 시 메세지를 입력받을 객체

public:
    virtual int Load(const wstring& _strFilepath) override { return S_OK; }
    virtual int Save(const wstring& _strFilepath) override { return S_OK; }

public:
    CShader(ASSET_TYPE _Type);
    ~CShader();
};

