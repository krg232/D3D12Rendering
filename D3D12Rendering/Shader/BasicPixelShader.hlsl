#include "CommonLibs.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState texSampler : register(s0);

float4 BasicPS(Output vsin) : SV_TARGET
{
    return float4(tex.Sample(texSampler,vsin.uv));
}