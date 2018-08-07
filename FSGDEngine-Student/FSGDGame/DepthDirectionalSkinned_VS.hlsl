#ifndef DEPTH_DIRECTIONAL_SKINNED_VS_HLSL
#define DEPTH_DIRECTIONAL_SKINNED_VS_HLSL

#include "VertexLayouts.hlsli"
#include "../EDRendererD3D/LightBuffers.h"
#include "../EDRendererD3D/ConstantBuffers.h"

DepthVertOut main( VERTEX_POSBONEWEIGHT input )
{
	DepthVertOut output;
	
	//outVert.position = mul(float4(input.position, 1.0), gWorld);
	float4x4 skinMat = gSkinnedMatrices[input.bone.x] * input.weights.x;
	skinMat += gSkinnedMatrices[input.bone.y] * input.weights.y;
	skinMat += gSkinnedMatrices[input.bone.z] * input.weights.z;
	skinMat += gSkinnedMatrices[input.bone.w] * input.weights.w;

	output.position = mul(float4(input.position, 1.0), skinMat);

	output.position = mul(output.position, DirLight.viewProjection[DirLight.currentIndex]);

    output.depth.xy = output.position.zw;

	return output;
}

#endif //DEPTH_DIRECTIONAL_SKINNED_VS_HLSL