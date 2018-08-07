#ifndef GBUFFERS_VS_HLSL
#define GBUFFERS_VS_HLSL

#include "VertexLayouts.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"

GBufferVertexOut main(VERTEX_POSNORMTEX input)
{
	GBufferVertexOut output = (GBufferVertexOut)0;

	output.texCoord = input.texCoord;
	output.position = mul(float4(input.position, 1.0), gMVP);
	output.normalW = mul(input.normal, (float3x3) gWorld);
	output.depthDiv = output.position.zw;

	return output;
}

#endif //GBUFFERS_VS_HLSL
