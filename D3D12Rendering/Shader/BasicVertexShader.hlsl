#include "CommonLibs.hlsli"

cbuffer cbuff : register(b0)
{
    matrix mat;
};

struct VInput
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

Output BasicVS(VInput input)
{
    Output vsout;
    
    vsout.svpos = mul(mat, input.pos);
    vsout.pos = input.pos;
    vsout.uv = input.uv;
    
    return vsout;
}