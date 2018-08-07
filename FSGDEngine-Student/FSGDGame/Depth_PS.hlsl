#ifndef DEPTH_PS_HLSL
#define DEPTH_PS_HLSL

#include "VertexLayouts.hlsli"

float4 main(DepthVertOut input) : SV_TARGET0
{
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}

#endif //DEPTH_PS_HLSL
