#ifndef DEPTH_POINT_PS_HLSL
#define DEPTH_POINT_PS_HLSL

#include "VertexLayouts.hlsli"

float4 main(DepthVertOut input) : SV_TARGET0
{
	return input.depth.x / input.depth.y;
}

#endif //DEPTH_POINT_PS_HLSL
