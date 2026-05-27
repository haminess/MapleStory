#include "pch.h"
#include "CTextUI.h"
#include <Engine/CFontMgr.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CAssetMgr.h>

CTextUI::CTextUI(UINT _Type)
	: CUIScript(_Type)
	, m_Info {L"", Vector2(), 16.f, (UINT)FONT_RGBA(255,255,255,255), 0.f, nullptr, TEXT_ALIGN::LEFT }
	, m_Color{255.f, 255.f, 255.f, 255.f}
{
	AddScriptParam({ SCRIPT_PARAM::TEXT, "Text", &m_Info.Text });
	AddScriptParam({ SCRIPT_PARAM::VEC4, "Color", &m_Color });
	AddScriptParam({ SCRIPT_PARAM::INT, "Align", &m_Info.Align });
	AddScriptParam({ SCRIPT_PARAM::FLOAT, "Size", &m_Info.FontSize });
	AddScriptParam({ SCRIPT_PARAM::TEXT, "Font",&m_Font });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Load Font", this, (SCRIPT_DELEGATE)&CTextUI::LoadFont});
}

CTextUI::CTextUI(const CTextUI& _Other)
	: CUIScript(_Other)
	, m_Info(_Other.m_Info)
	, m_Color(_Other.m_Color)
{
	AddScriptParam({ SCRIPT_PARAM::TEXT, "Text", &m_Info.Text });
	AddScriptParam({ SCRIPT_PARAM::VEC4, "Color", &m_Color });
	AddScriptParam({ SCRIPT_PARAM::INT, "Align", &m_Info.Align });
	AddScriptParam({ SCRIPT_PARAM::FLOAT, "Size", &m_Info.FontSize });
	AddScriptParam({ SCRIPT_PARAM::TEXT, "Font",&m_Font });
	AddScriptParam({ SCRIPT_PARAM::FUNCTION, "Load Font", this, (SCRIPT_DELEGATE)&CTextUI::LoadFont });
}

CTextUI::~CTextUI()
{
}


void CTextUI::Tick_UI()
{
	m_Info.Color = FONT_RGBA(m_Color.x, m_Color.y, m_Color.z, m_Color.w);
	m_Info.Position = CKeyMgr::GetWorldToResolutionPos(Vector2(Transform()->GetWorldPos().x, Transform()->GetWorldPos().y));
	CRenderMgr::GetInst()->AddText(m_Info);
}

void CTextUI::SaveComponent(FILE* _File)
{
	SaveWString(m_Info.Text, _File);

	fwrite(&m_Info.Position, sizeof(Vector2), 1, _File);
	fwrite(&m_Info.FontSize, sizeof(float), 1, _File);
	fwrite(&m_Color, sizeof(Vector4), 1, _File);
	fwrite(&m_Info.Time, sizeof(float), 1, _File);
	fwrite(&m_Info.Align, sizeof(TEXT_ALIGN), 1, _File);

	SaveWString(m_Font, _File);
}

void CTextUI::LoadComponent(FILE* _File)
{
	LoadWString(m_Info.Text, _File);

	fread(&m_Info.Position, sizeof(Vector2), 1, _File);
	fread(&m_Info.FontSize, sizeof(float), 1, _File);
	fread(&m_Color, sizeof(Vector4), 1, _File);
	fread(&m_Info.Time, sizeof(float), 1, _File);
	fread(&m_Info.Align, sizeof(TEXT_ALIGN), 1, _File);

	LoadWString(m_Font, _File);

	m_Info.Color = FONT_RGBA(m_Color.x, m_Color.y, m_Color.z, m_Color.w);
	SetFont(m_Font);
}

void CTextUI::SetFont(const wstring& _Font)
{
	m_Info.Font = CAssetMgr::GetInst()->FindAsset<CFont>(_Font).Get();
}
