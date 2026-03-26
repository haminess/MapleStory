#include "pch.h"
#include "CMaterial.h"
#include "CGraphicShader.h"
#include "CConstBuffer.h"
#include "CDevice.h"
#include "CTexture.h"


CMaterial::CMaterial(bool _EngineRes)
	: CAsset(ASSET_TYPE::MATERIAL, _EngineRes)
{
	m_Const.v4Arr[0] = Vector4(1.f, 0.f, 0.f, 1.f);
}

CMaterial::~CMaterial()
{
}

CMaterial* CMaterial::Clone()
{
	CMaterial* pClone = new CMaterial(*this);
	pClone->m_SharedMtrl = this;
	return pClone;
}

CMaterial::CMaterial(const CMaterial& _Origin)
	: CAsset(_Origin)
	, m_Shader(_Origin.m_Shader)
	, m_Const(_Origin.m_Const)
	, m_arrTex{}
	, m_SharedMtrl(_Origin.m_SharedMtrl)
{
	for (UINT i = 0; i < (UINT)TEX_PARAM::TEX_END; ++i)
	{
		m_arrTex[i] = _Origin.m_arrTex[i];
	}
}


int CMaterial::Save(const wstring& _FilePath)
{
	// m_SharedMtrl 원형 재질을 가리킨다 == 동적재질이다.
	// 동적 재질을 저장하려고 했다. ==> 에러
	// 동적 재질 = 게임이 플레이되는 도중에 일시적으로 만들어서 쓰고 버리는 재질
	assert(!m_SharedMtrl.Get());

	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");
	assert(pFile);

	SaveAssetRef(m_Shader, pFile);

	fwrite(&m_Const, sizeof(MtrlConst), 1, pFile);

	for (int i = 0; i < (int)TEX_PARAM::TEX_END; ++i)
	{
		SaveAssetRef(m_arrTex[i], pFile);
	}

	fclose(pFile);

	return S_OK;
}


int CMaterial::Load(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");
	assert(pFile);

	LoadAssetRef(m_Shader, pFile);

	fread(&m_Const, sizeof(MtrlConst), 1, pFile);

	for (int i = 0; i < (int)TEX_PARAM::TEX_END; ++i)
	{
		LoadAssetRef(m_arrTex[i], pFile);
	}

	fclose(pFile);

	return S_OK;
}


void CMaterial::Binding()
{
	if (nullptr == m_Shader)
		return;


	// Texture 상수 버퍼로 바인딩
	for (int i = 0; i < TEX_END; ++i)
	{
		if (nullptr == m_arrTex[i])
		{
			m_Const.bTex[i] = 0;
			// 다른 상수버퍼에 이전에 바인딩 된 값이 존재할 수 있기 때문에 초기화
			CTexture::Clear(i);
			continue;
		}

		// 타입에 맞는 상수 버퍼로 바인딩
		m_Const.bTex[i] = 1;
		m_arrTex[i]->Binding(i);
	}

	// 상수 데이터 바인딩
	static CConstBuffer* pMtrlCB = CDevice::GetInst()->GetCB(CB_TYPE::MATERIAL);
	pMtrlCB->SetData(&m_Const);
	pMtrlCB->Binding();

	// Shader 바인딩
	m_Shader->Binding();
}

void CMaterial::SetTexParam(TEX_PARAM _Param, Ptr<CTexture> _tex)
{
	m_arrTex[_Param] = _tex;
}

void* CMaterial::GetScalarParam(SCALAR_PARAM _Type)
{
	switch (_Type)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		return m_Const.iArr + _Type;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		return m_Const.fArr + (_Type - FLOAT_0);
	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		return m_Const.v2Arr + (_Type - VEC2_0);
	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		return m_Const.v4Arr + (_Type - VEC4_0);
	case MAT_0:
	case MAT_1:
		return m_Const.mat + (_Type - MAT_0);
	}

	assert(nullptr);
}

