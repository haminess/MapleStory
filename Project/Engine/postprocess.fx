#ifndef _POSTPROCESS
#define _POSTPROCESS

#include "value.fx"

// Vertex Shader 
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

// Mesh : RectMesh

VS_OUT VS_Post(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    // 렌더타겟의 두배를 곱해 화면 전체에 적용될 수 있도록 함
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_Post(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    // 회색 효과
    float Aver = (vColor.r + vColor.g + vColor.b) / 3.f;
    vColor.rgb = Aver;
        
    return vColor;
}


float4 PS_Fade(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    vColor.rgb = float3(0.f, 0.f, 0.f);
    vColor.a = g_float_0;
        
    return vColor;
}



VS_OUT VS_Screen(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    // 후처리 텍스쳐가 실제 좌표로 반영됨
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_Screen(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        
    return vColor;
}




// Mesh : RectMesh
// 후처리가 적용될 위치에 원래 출력되는 그래픽과 똑같이 가져오고 싶다
// - 필요한 정보
// 1. 픽셀 쉐이더의 픽셀 좌표 (화면 기준으로 호출된 픽셀 위치)
// 2. 렌더 타겟 해상도 (전체 화면 크기)
// - 구현 방법
// 전체 화면 대비(렌더타겟) 픽셀의 위치(0~1)와 화면을 1로 정규화
// 픽셀 좌표의 비율을 UV 좌표로 렌더타겟에서 샘플링
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    // 후처리 텍스쳐가 실제 좌표로 반영됨
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    //output.vPosition = float4(_in.vPos, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}


float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // 원래대로라면 NDC 좌표계의 비율 좌표로 보간되어 들어온다.
    // SV_Position -> Pixelshader (픽셀좌표)
    // 하지만 SV_Position 시멘틱을 사용하면 픽셀 좌표로 들어온다.
    _in.vPosition;
    
    // RenderTarget에서 렌더할 픽셀 자신의 위치를 구한다.
    // uv 좌표는 비율값
    float2 vScreenUV = _in.vPosition.xy / g_RenderResolution;
    float y = vScreenUV.y;
    
    // NoiseTexture 가 있으면
    if (g_btex_1)
    {
        // 노이즈 텍스처에서 색상 값 받아오기
        float4 vNoise = g_tex_1.Sample(g_sam_0, vScreenUV /*0~1*/ + 0.05f * g_Time);
        // 위치 조정
        vNoise -= 0.5f;
        // 범위 조정
        vNoise *= 0.05f;
        
        vScreenUV += vNoise.xy;
    }
    
    vColor = g_tex_0.Sample(g_sam_0, vScreenUV);
    
    return vColor;
}




struct VS_VortexOut
{
    float4 vPosition : SV_Position;
    float2 vObjectUV : TEXCOORD;
};


VS_VortexOut VS_Vortex(VS_IN _in)
{
    VS_VortexOut output = (VS_VortexOut) 0.f;
            
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
        
    float4 vProjPos = mul(float4(0.f, 0.f, 0.f, 1.f), g_matWVP);
    vProjPos.xyz = vProjPos.xyz / vProjPos.w;
    
    output.vObjectUV.x = (vProjPos.x + 1.f) / 2.f;
    output.vObjectUV.y = 1.f - ((vProjPos.y + 1.f) / 2.f);
        
    return output;
}


float4 PS_Vortex(VS_VortexOut _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    float effectRadius = 0.1f;
    float effectAngle = 1.f * PI;
    
    float2 center = _in.vObjectUV;
    
    float2 uv = (_in.vPosition.xy / g_RenderResolution.xy) - center;
    
    float len = length(uv * float2(g_RenderResolution.x / g_RenderResolution.y, 1.));
    float angle = atan2(uv.y, uv.x) + effectAngle * smoothstep(effectRadius, 0., len);
    float radius = length(uv);

    vColor = g_tex_0.Sample(g_sam_0, float2(radius * cos(angle), radius * sin(angle)) + center);
    
    return vColor;
}



VS_OUT VS_Enlarge(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    // 렌더타겟의 두배를 곱해 화면 전체에 적용될 수 있도록 함
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_Enlarge(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // 마우스 좌표
    float2 vMouse = g_vec2_0;
    float2 vMouseWorld = g_vec2_1;

    // 픽셀 좌표
    float2 vScreen = _in.vPosition.xy;
    
    // 타겟 좌표
    float vTargetDis = distance(g_vec2_2, vMouseWorld);
    float vTargetDis2 = distance(g_vec2_3, vMouseWorld);
    
    float dis = distance(vMouse, vScreen);
    
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    if (dis <= 100.f)
    {
        float2 vecOut = vScreen - vMouse;
	
        vColor = g_tex_0.Sample(g_sam_0, (vMouse + vecOut / 2) / g_RenderResolution);
        
        if (vTargetDis < 15.f)
            vColor.r += (0.5f + 0.5f * sin(g_Time * 10.f));
        if (vTargetDis2 < 15.f)
            vColor.b += (0.5f + 0.5f * sin(g_Time * 10.f));
    }
    else if (dis < 150.f)
    {
        float2 vecOut = vScreen - vMouse;
        float largeScale = 1 + cos((PI / 2) * (dis - 100.f) / 50.f); // 2~1
	
        vColor = g_tex_0.Sample(g_sam_0, (vMouse + vecOut / largeScale) / g_RenderResolution);
        
        if (vTargetDis < 15.f)
            vColor.r += (0.5f + 0.5f * sin(g_Time * 10.f));
        if (vTargetDis2 < 15.f)
            vColor.b += (0.5f + 0.5f * sin(g_Time * 10.f));
    }
    
    
    return vColor;
}


VS_OUT VS_Wave(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    // 렌더타겟의 두배를 곱해 화면 전체에 적용될 수 있도록 함
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_Wave(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // 마우스 좌표
    float2 vMouse = g_vec2_0;
    float2 vMouseWorld = g_vec2_1;

    // 픽셀 좌표
    float2 vScreen = _in.vPosition.xy;
    
    // 타겟 좌표
    float vTargetDis = distance(g_vec2_2, vMouseWorld);
    float vTargetDis2 = distance(g_vec2_3, vMouseWorld);
    
    float dis = distance(vMouse, vScreen);
    
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    float Radius = 150.f;
    if (dis < g_float_0 && g_float_0)
    {
        float2 vecOut = vScreen - vMouse;
        
        float r = sqrt(dis);
        float z = sin(dis * 0.1 - g_Time * 5);
        
        
        vColor = g_tex_0.Sample(g_sam_0, (vMouse + vecOut * z) / g_RenderResolution);
    }
    
    
    
    return vColor;
}



VS_OUT VS_Water(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    // 렌더타겟의 두배를 곱해 화면 전체에 적용될 수 있도록 함
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
        
    return output;
}

float4 PS_Water(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    // 마우스 좌표
    float2 vMouse = g_vec2_0;
    float2 vMouseWorld = g_vec2_1;

    // 픽셀 좌표
    float2 vScreen = _in.vPosition.xy;
    
    // 타겟 좌표
    float vTargetDis = distance(g_vec2_2, vMouseWorld);
    float vTargetDis2 = distance(g_vec2_3, vMouseWorld);
    
    float dis = distance(vMouse, vScreen);
    
    vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    if (dis < 150.f)
    {
        float2 vecOut = vScreen - vMouse;
        
        float r = sqrt(dis);
        float z = /*1.0 +*/ 0.05 * sin((dis + g_Time * 0.035) / 0.013);
        
        
        vColor = g_tex_0.Sample(g_sam_0, vecOut * z);
    }
    
    
    
    return vColor;
}




#endif