#ifndef _DEBUG
#define _DEBUG

#include "value.fx"

// 입력 구조체
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    // sv_position 시멘틱이 붙어있는 아이로 레스터라이저에 넣어줌
    // 레스터라이저는 시멘틱 통해 연산
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_DebugShape(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_DebugShape(VS_OUT _in) : SV_Target /*반환 자료형 시멘틱*/
{
    
    //float Tickness = 0.05f;
    //if(Tickness < _in.vUV.x && _in.vUV.x < (1.f - Tickness) &&
    //    Tickness < _in.vUV.y && _in.vUV.y < (1.f - Tickness))
    //{
    //    discard;
    //}
    
    return g_vec4_0;
}




#endif