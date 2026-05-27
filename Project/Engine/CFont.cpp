#include "pch.h"
#include "CFont.h"
#include "CDevice.h"


CFont::CFont(bool _EngineRes)
	: CAsset(ASSET_TYPE::FONT, _EngineRes)
{
}
CFont::CFont(const CFont& _Other)
	: CAsset(_Other)
{
}

CFont::~CFont()
{
	if (nullptr != m_FontWrapper)
		m_FontWrapper->Release();
}

CFont* CFont::Clone()
{
	CFont* pClone = new CFont(*this);
	pClone->Load(m_FontName);
	return pClone;
}

int CFont::Save(const wstring& _FilePath)
{
	return S_OK;
}


int CFont::Create(IFW1Factory* _Factory, const wstring& _FontKey)
{
	SetKey(_FontKey);

	m_FontName = _FontKey;

	if (FAILED(_Factory->CreateFontWrapper(DEVICE, m_FontName.c_str(), &m_FontWrapper)))
	{
		assert(nullptr);
		return E_FAIL;
	}

	return S_OK;
}

