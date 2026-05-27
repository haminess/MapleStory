#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"

// Vertex Shader
// 정점마다 호출할 함수
// 여기에서 렌더링 파이프라인 수행
// IA -> VS -> Rasterizer -> Pixel Shader
// Rasterizer에서 NDC 좌표 입력값을 받아서 실제 화면해상도(NDC 기반이 아닌)에 픽셀이 어디 위치하는 지 계산 
// 즉, 정점이 실제 렌더 될 위치를 반환하는 함수
// 정점 쉐이더(VS)에서 리턴한 값이 Rasterizer 함수로 넘어간다.
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
};


VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
            
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    // 스프라이트에서는 스프라이트 크기 사용
    if (g_SpriteUse)
    {
        float3 vOffsetPos = _in.vPos + float3(g_OffsetUV, 0.f);
        output.vPosition = mul(float4(vOffsetPos, 1.f), g_matWVP);
        //output.vPosition += float4(g_OffsetUV.x, g_OffsetUV.y, 0.f, 0.f);
    }
        
    return output;
}

// Pixel Shader
// 반환값이 float4, 색상값
// 즉, 정점이 실제 렌더 될 색상을 반환하는 함수
// 매개변수는 VS_Test의 반환값
// Interpolation, 보간 => 가중치 개념
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    // 1. 픽셀 쉐이더를 중도폐기처분
    //clip(-1); discard;    
    
    // 2. 텍스쳐 샘플링
    // Sample(sampler, uv) 
    // 바인딩된 텍스쳐의 정보를 얻는 함수 
    // g_tex_0에 texutre 정보가 바인딩 되었으면, 텍스쳐를 샘플링
    // texture에서 uv 좌표에 해당하는 색 정보를 가져옴
    //float4 vColor = g_tex.Sample(g_sam_1 /*샘플러*/, _in.vUV);
    
    
    float4 vColor = (float4) 0.f;
    
    if (g_vec2_2.y == 3.f)
        discard;
    
    // Flipbook 의 현재 Sprite 를 재생해야 하는 경우
    if (g_SpriteUse)
    {
        //float2 vSpriteUV = g_LeftTopUV + (_in.vUV * g_SliceUV);
        
        float2 vBackroundLeftTop = g_LeftTopUV + (g_SliceUV / 2.f) - (g_BackgroundUV / 2.f);
        float2 vSpriteUV = vBackroundLeftTop + (_in.vUV * g_BackgroundUV) - g_OffsetUV;
        
        if (vSpriteUV.x < g_LeftTopUV.x || g_LeftTopUV.x + g_SliceUV.x < vSpriteUV.x
            || vSpriteUV.y < g_LeftTopUV.y || g_LeftTopUV.y + g_SliceUV.y < vSpriteUV.y)
        {
            // 투명한 부분은 렌더링하지 않는다.
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_1 /*샘플러*/, vSpriteUV /*텍스쳐(UV) 좌표*/);
        }
        vColor.a *= g_SpriteAlpha;
    }
    
    // FlipbookPlayer 가 없거나 재생중인 Flipbook 이 없는 경우
    else
    {
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            vColor = GetDebugColor(_in.vUV, 10);
    }
       
        
    if (vColor.a == 0.f)
        discard;
    
     // 광원처리
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
    
    vColor.rgb *= LightColor;

    // sRGB 보정
    vColor.rgb = pow(vColor.rgb, 1.0 / 2.2);
    
    return vColor;
}


// Std2DAlphaBlend
float4 PS_Std2D_AlphaBlend(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (g_SpriteUse)
    {
        float2 vBackroundLeftTop = g_LeftTopUV + (g_SliceUV / 2.f) - (g_BackgroundUV / 2.f);
        float2 vSpriteUV = vBackroundLeftTop + (_in.vUV * g_BackgroundUV); // - g_OffsetUV;
        
        if (vSpriteUV.x < g_LeftTopUV.x || g_LeftTopUV.x + g_SliceUV.x < vSpriteUV.x
            || vSpriteUV.y < g_LeftTopUV.y || g_LeftTopUV.y + g_SliceUV.y < vSpriteUV.y)
        {
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_1, vSpriteUV);
        }
        
        vColor.a *= g_SpriteAlpha;
    }
    else
    {
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            discard; //vColor = GetDebugColor(_in.vUV, 10);
        
    }
    
    if (vColor.a == 0.f)
        discard;
    
     // 광원처리
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
    
    vColor.rgb *= LightColor;
    
    // sRGB 보정
    vColor.rgb = pow(vColor.rgb, 1.0 / 2.2);
    
    return vColor;
}


// Std2dPaperBurn
float4 PS_Std2D_PaperBurn(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;
    
    if (g_SpriteUse)
    {
        //float2 vSpriteUV = g_LeftTopUV + (_in.vUV * g_SliceUV);
        
        float2 vBackroundLeftTop = g_LeftTopUV + (g_SliceUV / 2.f) - (g_BackgroundUV / 2.f);
        float2 vSpriteUV = vBackroundLeftTop + (_in.vUV * g_BackgroundUV) - g_OffsetUV;
        
        if (vSpriteUV.x < g_LeftTopUV.x || g_LeftTopUV.x + g_SliceUV.x < vSpriteUV.x
            || vSpriteUV.y < g_LeftTopUV.y || g_LeftTopUV.y + g_SliceUV.y < vSpriteUV.y)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
        else
        {
            vColor = g_Atlas.Sample(g_sam_1, vSpriteUV);
        }
        
        vColor.a *= g_SpriteAlpha;
    }
    else
    {
        // 첫번째 텍스쳐는 물체의 색상
        if (g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        else
            vColor = GetDebugColor(_in.vUV, 10);
    }
    
    
    // 2번째 텍스쳐 사용, 노이즈 텍스쳐
    if (g_btex_1)
    {
        float4 vNoise = g_tex_1.Sample(g_sam_0, _in.vUV);
        if (1.f < vNoise.r + g_float_0)
            discard;
    }
    
    if (vColor.a == 0.f)
        discard;
    
    // 광원처리
    float3 LightColor = float3(0.f, 0.f, 0.f);
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalcLight2D(i, _in.vWorldPos, LightColor);
    }
    
    vColor.rgb *= LightColor;
    
    return vColor;
}

struct GS_OUT
{
    float4 vPosition : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
    uint digitIdx : TEXCOORD1;
};

VS_OUT VS_DamageSkin(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    float4 worldPos = mul(float4(0.f, 0.f, 0.f, 1.f), g_matWorld);

    output.vWorldPos = worldPos.xyz;

    return output;
}

[maxvertexcount(64)]
void GS_DamageSkin(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _outStream)
{
    int digitCount = g_int_1;
    float spacing = g_float_1;

    float3 worldScale;
    worldScale.x = length(float3(g_matWorld._11, g_matWorld._12, g_matWorld._13));
    worldScale.y = length(float3(g_matWorld._21, g_matWorld._22, g_matWorld._23));
    worldScale.z = length(float3(g_matWorld._31, g_matWorld._32, g_matWorld._33));

    float digitWidth = worldScale.x;
    float digitHeight = worldScale.y;

    float totalWidth = digitWidth + digitWidth * spacing * (digitCount - 1);

    float3 center = _in[0].vWorldPos;

    float startX = -totalWidth * 0.5f;
    
    matrix matVP = mul(g_matView, g_matProj);

    for (int i = 0; i < digitCount; ++i)
    {
        float x0 = startX + digitWidth * spacing * i;
        float x1 = x0 + digitWidth;

        float y0 = -digitHeight * 0.5f;
        float y1 = digitHeight * 0.5f;

        float3 pos[4];

        pos[0] = center + float3(x0, y1, 0.f);
        pos[1] = center + float3(x1, y1, 0.f);
        pos[2] = center + float3(x0, y0, 0.f);
        pos[3] = center + float3(x1, y0, 0.f);

        float2 uv[4];

        uv[0] = float2(0.f, 0.f);
        uv[1] = float2(1.f, 0.f);
        uv[2] = float2(0.f, 1.f);
        uv[3] = float2(1.f, 1.f);

        GS_OUT output = (GS_OUT) 0.f;

        output.digitIdx = i;

        output.vWorldPos = pos[0];
        output.vPosition = mul(float4(pos[0], 1.f), matVP);
        output.vUV = uv[0];
        _outStream.Append(output);

        output.vWorldPos = pos[1];
        output.vPosition = mul(float4(pos[1], 1.f), matVP);
        output.vUV = uv[1];
        _outStream.Append(output);

        output.vWorldPos = pos[2];
        output.vPosition = mul(float4(pos[2], 1.f), matVP);
        output.vUV = uv[2];
        _outStream.Append(output);

        output.vWorldPos = pos[3];
        output.vPosition = mul(float4(pos[3], 1.f), matVP);
        output.vUV = uv[3];
        _outStream.Append(output);

        _outStream.RestartStrip();
    }
}
float4 PS_DamageSkin(GS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;

    int number = g_int_0;
    int digitCount = g_int_1;

    if (g_btexarr_0)
    {
        int currentDigit = _in.digitIdx;

        int targetNumber = 0;

        if (number == 0)
        {
            targetNumber = 0;
        }
        else
        {
            targetNumber = number;

            for (int i = 0; i < digitCount - currentDigit - 1; ++i)
            {
                targetNumber /= 10;
            }

            targetNumber %= 10;
        }

        vOutColor = g_texarr_0.Sample(
            g_sam_0,
            float3(_in.vUV, targetNumber)
        );
    }
    else
    {
        vOutColor = GetDebugColor(_in.vUV, 10);
    }

    vOutColor.a *= g_float_0;

    return vOutColor;
}

VS_OUT VS_UI(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;

    // UI는 카메라 View 영향 제거
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWorld);
    output.vPosition = mul(output.vPosition, g_matProj);

    output.vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld).xyz;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_UI(VS_OUT _in) : SV_Target
{
    float4 vColor = (float4) 0.f;

    if (g_btex_0)
    {
        vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    else
    {
        vColor = GetDebugColor(_in.vUV, 10);
    }

    if (vColor.a <= 0.f)
        discard;

    return vColor;
}

#define SLIDER_NONE            0
#define SLIDER_LEFT_TO_RIGHT   1
#define SLIDER_RIGHT_TO_LEFT   2
#define SLIDER_TOP_TO_BOTTOM   3
#define SLIDER_BOTTOM_TO_TOP   4
#define SLIDER_RADIAL_CW       5
#define SLIDER_RADIAL_CCW      6

float4 PS_Slider(VS_OUT _in) : SV_Target
{
    float2 uv = _in.vUV;

    int sliderType = g_int_0;
    float ratio = saturate(g_float_0);

    switch (sliderType)
    {
        case SLIDER_LEFT_TO_RIGHT:
        {
            if (uv.x > ratio)
                discard;
            break;
        }

        case SLIDER_RIGHT_TO_LEFT:
        {
            if (uv.x < (1.f - ratio))
                discard;
            break;
        }

        case SLIDER_TOP_TO_BOTTOM:
        {
            if (uv.y > ratio)
                discard;
            break;
        }

        case SLIDER_BOTTOM_TO_TOP:
        {
            if (uv.y < (1.f - ratio))
                discard;
            break;
        }

        case SLIDER_RADIAL_CW:
        case SLIDER_RADIAL_CCW:
        {
            float2 centerUV = uv - float2(0.5f, 0.5f);

            float angle = atan2(-centerUV.y, centerUV.x);

            // -PI ~ PI -> 0 ~ 1
            float normalizedAngle = angle / (2.f * 3.14159265f) + 0.5f;

            // 시작점을 12시 방향으로 이동
            normalizedAngle = frac(normalizedAngle + 0.25f);

            if (sliderType == SLIDER_RADIAL_CW)
            {
                if (normalizedAngle > ratio)
                    discard;
            }
            else
            {
                if ((1.f - normalizedAngle) > ratio)
                    discard;
            }

            break;
        }
    }

    float4 color = (float4) 0.f;

    if (g_btex_0)
    {
        color = g_tex_0.Sample(g_sam_0, uv);
    }
    else
    {
        color = float4(1.f, 1.f, 1.f, 1.f);
    }

    if (color.a <= 0.f)
        discard;

    return color;
}

#endif
