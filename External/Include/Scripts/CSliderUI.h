#pragma once
#include "CUIScript.h"

enum class SLIDER_TYPE
{
    LEFT_TO_RIGHT = 1,
    RIGHT_TO_LEFT,
    TOP_TO_BOTTOM,
    BOTTOM_TO_TOP,
    RADIAL_CW,
    RADIAL_CCW,
};

class CSliderUI :
    public CUIScript
{
private:
    float       m_fRatio;
    SLIDER_TYPE m_SliderType;

public:
    void SetRatio(float _Ratio);
    float GetRatio() const { return m_fRatio; }

    void SetSliderType(SLIDER_TYPE _Type);
    SLIDER_TYPE GetSliderType() const { return m_SliderType; }

public:
    virtual void Init() override;
    virtual void Tick_UI() override;

public:
    CLONE(CSliderUI);
    CSliderUI(UINT _Type = (UINT)SCRIPT_TYPE::SLIDERUI);
    CSliderUI(const CSliderUI& _Other);
    ~CSliderUI();
};