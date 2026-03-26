
#ifndef _FADE
#define _FADE


#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};


VS_OUT VS_Fade(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vPosition = float4(_in.vPos.xy, 0.f, 1.f);
    output.vUV = _in.vUV;
    return output;
}

float4 PS_Fade(VS_OUT _in) : SV_Target
{
    return g_vec4_0;
}

#endif