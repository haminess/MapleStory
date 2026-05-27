#include "pch.h"
#include "CSliderUI.h"

#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>

CSliderUI::CSliderUI(UINT _Type)
    : CUIScript(_Type)
    , m_fRatio(1.f)
    , m_SliderType(SLIDER_TYPE::LEFT_TO_RIGHT)
{
    AddScriptParam({ SCRIPT_PARAM::FLOAT, "Ratio", &m_fRatio });
    AddScriptParam({ SCRIPT_PARAM::INT, "SliderType", &m_SliderType });
}

CSliderUI::CSliderUI(const CSliderUI& _Other)
    : CUIScript(_Other)
    , m_fRatio(_Other.m_fRatio)
    , m_SliderType(_Other.m_SliderType)
{
    ClearScriptParam();

    AddScriptParam({ SCRIPT_PARAM::FLOAT, "Ratio", &m_fRatio });
    AddScriptParam({ SCRIPT_PARAM::INT, "SliderType", &m_SliderType });
}

CSliderUI::~CSliderUI()
{
}

void CSliderUI::Init()
{
    CUIScript::Init();
}

void CSliderUI::Tick_UI()
{
    m_fRatio = std::clamp(m_fRatio, 0.f, 1.f);

    CMeshRender* pMR = GetOwner()->MeshRender();
    if (!pMR)
        return;

    Ptr<CMaterial> pMtrl = pMR->GetMaterial();
    if (nullptr == pMtrl)
        return;

    pMtrl->SetScalarParam(FLOAT_0, m_fRatio);
    pMtrl->SetScalarParam(INT_0, (int)m_SliderType);
}

void CSliderUI::SetRatio(float _Ratio)
{
    m_fRatio = std::clamp(_Ratio, 0.f, 1.f);
}

void CSliderUI::SetSliderType(SLIDER_TYPE _Type)
{
    m_SliderType = _Type;
}