#ifndef GBUFFERS_BUMP_VS_HLSL
#define GBUFFERS_BUMP_VS_HLSL

#include "VertexLayouts.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"

GBufferBumpVertexOut main(VERTEX_POSNORMTANTEX input)
{
	GBufferBumpVertexOut output = (GBufferBumpVertexOut)0;
	//1
	output.texCoord = input.texCoord;
	output.position = mul(float4(input.position, 1.0), gMVP);
	output.depthDiv = output.position.zw;
	//2
	output.tbn[2] = mul(input.normal, (float3x3) gWorld);
	output.tbn[0] = mul(input.tangent, (float3x3) gWorld);
	output.tbn[1] = cross(output.tbn[2], output.tbn[0]) * input.determinant;

	return output;
}

#endif //GBUFFERS_BUMP_VS_HLSL
