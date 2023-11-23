#include "CommonLibs.hlsli"

cbuffer cbuff : register(b0)
{
    matrix mat;
};

Output BasicVS(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    Output vsout;
    
    vsout.svpos = mul(mat, pos);
    vsout.pos = pos;
    vsout.uv = uv;
    
    return vsout;
}