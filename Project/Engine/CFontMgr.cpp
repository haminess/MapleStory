#include "pch.h"
#include "CFontMgr.h"
#include "CFont.h"
#include "CAssetMgr.h"

#include "CDevice.h"

CFontMgr::CFontMgr()
	: m_FW1Factory(nullptr)
	, m_DefaultFont(nullptr)
{
}

CFontMgr::~CFontMgr()
{
	if (nullptr != m_FW1Factory)
		m_FW1Factory->Release();

	if (nullptr != m_DefaultFont)
		m_DefaultFont->Release();
}

void CFontMgr::Init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory)))
	{
		assert(nullptr);
	}

	// Font Asset Ãß°¡
	CFont* pFont = new CFont(true);
	pFont->Create(m_FW1Factory, L"Arial");
	CAssetMgr::GetInst()->AddAsset<CFont>(L"Arial", pFont);

	pFont = new CFont(true);
	pFont->Create(m_FW1Factory, L"¸¼Àº °íµñ");
	CAssetMgr::GetInst()->AddAsset<CFont>(L"SunnyGothic", pFont);

	pFont = new CFont(true);
	pFont->Create(m_FW1Factory, L"¹ÙÅÁ");
	CAssetMgr::GetInst()->AddAsset<CFont>(L"Background", pFont);

	if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_DefaultFont)))
	{
		assert(nullptr);
	}
}

void CFontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color, TEXT_ALIGN _Align, Ptr<CFont> _Font)
{
	UINT Flag = FW1_RESTORESTATE;

	switch (_Align)
	{
	case TEXT_ALIGN::CENTER:
		Flag |= FW1_CENTER;
		break;

	case TEXT_ALIGN::RIGHT:
		Flag |= FW1_RIGHT;
		break;

	case TEXT_ALIGN::LEFT:
	default:
		break;
	}

	IFW1FontWrapper* pFont = nullptr;
	if (_Font == nullptr)
		pFont = m_DefaultFont;
	else
		pFont = _Font->m_FontWrapper;

	pFont->DrawString(
			CONTEXT,
			_pStr,				// String
			_fFontSize,			// Font size
			_fPosX,				// X position
			_fPosY,				// Y position
			_Color,				// Text color, 0xAaBbGgRr
			Flag				// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
		);
}
