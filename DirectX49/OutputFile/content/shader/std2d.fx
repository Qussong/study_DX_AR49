#ifndef _STD2D
#define _STD2D

// 4096 크기제한
cbuffer TRANSFORM : register(b0)
{
    row_major Matrix g_matWorld;
    row_major Matrix g_matWorldInv;

    row_major Matrix g_matView;
    row_major Matrix g_matViewInv;

    row_major Matrix g_matProj;
    row_major Matrix g_matProjInv;

    row_major Matrix g_matWV;
    row_major Matrix g_matWVP;
}

cbuffer MATERIAL_CONST : register(b1)
{
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;
    
    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    
    row_major matrix g_mat_0;
    row_major matrix g_mat_1;
    row_major matrix g_mat_2;
    row_major matrix g_mat_3;
    
    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
    int g_btexcube_0;
    int g_btexcube_1;
    int g_btexarr_0;
    int g_btexarr_1;
}

Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);

TextureCube g_texcube_0 : register(t6);
TextureCube g_texcube_1 : register(t7);

Texture2DArray g_texarr_0 : register(t8);
Texture2DArray g_texarr_1 : register(t9);

SamplerState g_sam_0 : register(s0);
SamplerState g_sam_1 : register(s1);

struct VS_IN
{
    float4 vColor : COLOR;
    float3 vPos : POSITION; // Sementic
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    //uint width = 0;
    //uint height = 0;
    //g_tex_1.GetDimensions(width, height);
    
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (g_btex_0)
    {
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        
        //saturate 0 ~ 1 을 넘지 않게 보정
        float fAlpha = 1.f - saturate(dot(vColor.rb, vColor.rb) / 2.f);
        
        if (fAlpha < 0.1f)
        {
            // 픽셀 쉐이더를 중간에 폐기처리
            discard; //clip(-1);            
        }
    }
    
    return vColor;
}
#endif