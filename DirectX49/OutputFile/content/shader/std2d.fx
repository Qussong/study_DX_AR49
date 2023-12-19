#ifndef _STD2D
#define _STD2D

// 4096 크기제한
cbuffer TRANSFORM : register(b0)
{
    row_major float4x4 g_matWorld;
    row_major matrix g_matView;
    row_major matrix g_matProj;
}

struct VS_IN
{
    float3 vPos     : POSITION; // Sementic
    float4 vColor   : COLOR;
    float2 vUV      : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition    : SV_Position;
    float4 vColor       : COLOR;
    float2 vUV          : TEXCOORD;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
   
    // 로컬(모델) 좌표를 -> 월드 -> 뷰 -> 투영 좌표계로 순차적으로 변환
    float4 vWorldPos = mul(float4(_in.vPos, 1.f), g_matWorld);  // W
    float4 vViewPos = mul(vWorldPos, g_matView);                // V
    float4 vProjPos = mul(vWorldPos, g_matProj);                // P
    
    output.vPosition = vProjPos;;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    return _in.vColor;
}

#endif