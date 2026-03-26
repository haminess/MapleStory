#ifndef _MTILEMAP
#define _MTILEMAP

#include "func.fx"
#include "value.fx"

struct tTexArraySize
{
    float2 SizeUV;
    float2 Resolution;
};

StructuredBuffer<tMTileData> TileBuffer : register(t16);
StructuredBuffer<tTexArraySize> SizeBuffer : register(t17);

struct VS_IN
{
    float3 vPos : POSITION;
    uint ID : SV_InstanceID;
};

struct VS_OUT
{
    float3 vLocalPos : POSITION;
    uint ID : FOG;
};

VS_OUT VS_MTile(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
       
    output.vLocalPos = _in.vPos;
    output.ID = (float)_in.ID;
    
    return output;
}


struct GS_OUT
{
    float4 vPosition : SV_Position;
    float3 vWorldPos : POSITION;
    float2 vUV : TEXCOORD;
    uint InstID : FOG;
};

// Geometry Shader
// 1. 비활성화 된 파티클을 Rasterizer 로 보내지 않는다.
// 2. Billboard 효과

#define Tile TileBuffer[idx]

[maxvertexcount(32)]
void GS_MTile(point VS_OUT _in[1], inout TriangleStream<GS_OUT> _OutStream)
{
    // 1. 타일 텍스쳐의 해상도에 맞게 출력하기
    // 2. ObjectPos + TilePos에 개별 타일 출력하기
    // 3. TileScale에 맞게 타일 출력하기
    
    
    // 정점 하나를 사각형으로 만듬
    GS_OUT output[4] = { (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f, (GS_OUT) 0.f };
    
    //  0 -- 1
    //  | \  |
    //  3 -- 2
    int idx = _in[0].ID;
    float2 scale = Tile.TileScale;
    //scale = SizeBuffer[Tile.TexIdx].Resolution;
    float2 resolution = SizeBuffer[Tile.TexIdx].Resolution;
    float2 localTilePos = Tile.TilePos;
    float3 vViewPos = mul(float4(Tile.TilePos, 10.f, 1.f), g_matView).xyz;
    
    // 정점 1)
    // 타일의 로컬 위치를 기준으로 사각형 정점 생성
    output[0].vPosition = float4(localTilePos + float2(-scale.x / 2.f, scale.y / 2.f), 0.f, 1.f);
    output[1].vPosition = float4(localTilePos + float2(scale.x / 2.f, scale.y / 2.f), 0.f, 1.f);
    output[2].vPosition = float4(localTilePos + float2(scale.x / 2.f, -scale.y / 2.f), 0.f, 1.f);
    output[3].vPosition = float4(localTilePos + float2(-scale.x / 2.f, -scale.y / 2.f), 0.f, 1.f);
    
    //// 정점 2)
    //// resolution을 사용하여 텍스처 해상도 크기로 정점 생성
    //output[0].vPosition = float4(float2(-resolution.x / 2.f, resolution.y / 2.f), 10.f, 1.f);
    //output[1].vPosition = float4(float2(resolution.x / 2.f, resolution.y / 2.f), 10.f, 1.f);
    //output[2].vPosition = float4(float2(resolution.x / 2.f, -resolution.y / 2.f), 10.f, 1.f);
    //output[3].vPosition = float4(float2(-resolution.x / 2.f, -resolution.y / 2.f), 10.f, 1.f);
    
    
    //// 변환 1) 월드 변환
    //for (int i = 0; i < 4; ++i)
    //{
    //    // 월드 변환 (오브젝트의 위치, 회전, 스케일 적용)
    //    float4 worldPos = mul(output[i].vPosition, g_matWorld);
    //    output[i].vWorldPos = worldPos.xyz;
        
    //    // 카메라, 투영변환
    //    output[i].vPosition = mul(worldPos, g_matView);
    //    output[i].vPosition = mul(output[i].vPosition, g_matProj);
    //    output[i].InstID = idx;
    //}
    
    // 변환 2) 위치만 이동
    for (int i = 0; i < 4; ++i)
    {
        // 위치만 이동
        float4 worldPos = float4(output[i].vPosition.xyz + float3(g_matWorld._41, g_matWorld._42, g_matWorld._43), 1.0f);
    
        output[i].vWorldPos = worldPos.xyz;
    
        // 카메라, 투영변환
        output[i].vPosition = mul(worldPos, g_matView);
        output[i].vPosition = mul(output[i].vPosition, g_matProj);
        output[i].InstID = idx;
    }
    
    //// 변환3) 파티클 따라하기
    //for (int i = 0; i < 4; ++i)
    //{
    //    output[i].vPosition.xyz += vViewPos;
    //    output[i].vPosition.w = 1.f;
    //    output[i].vPosition = mul(output[i].vPosition, g_matProj);
    //    output[i].InstID = _in[0].ID;
    //}
    
    
    // 생성시킨 정점의 UV 세팅
    output[0].vUV = float2(0.f, 0.f);
    output[1].vUV = float2(1.f, 0.f);
    output[2].vUV = float2(1.f, 1.f);
    output[3].vUV = float2(0.f, 1.f);
    
    
    // Stream 출력
    _OutStream.Append(output[0]);
    _OutStream.Append(output[1]);
    _OutStream.Append(output[2]);
    _OutStream.RestartStrip();
    
    _OutStream.Append(output[0]);
    _OutStream.Append(output[2]);
    _OutStream.Append(output[3]);
    _OutStream.RestartStrip();
    
}


float4 PS_MTile(GS_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 0.f, 1.f);
   
    if (g_btexarr_0)
    {
       // UV 좌표를 텍스처 크기에 맞게 조정
        float2 adjustedUV;
        int texIndex = TileBuffer[_in.InstID].TexIdx;
        adjustedUV.x = _in.vUV.x * SizeBuffer[texIndex].SizeUV.x;
        adjustedUV.y = _in.vUV.y * SizeBuffer[texIndex].SizeUV.y;
        
        vColor = g_texarr_0.Sample(g_sam_0, float3(_in.vUV, texIndex));
    }
        
   //// 광원처리
   // float3 LightColor = float3(0.f, 0.f, 0.f);
   // for (int i = 0; i < g_Light2DCount; ++i)
   // {
   //     CalcLight2D(i, _in.vWorldPos, LightColor);
   // }
   // vColor.rgb *= LightColor;
   
    return vColor;
}

#endif