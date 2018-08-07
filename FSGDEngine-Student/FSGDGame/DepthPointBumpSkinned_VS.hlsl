#ifndef DEPTH_POINT_BUMP_SKINNED_VS_HLSL
#define DEPTH_POINT_BUMP_SKINNED_VS_HLSL

#include "VertexLayouts.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"

float4 main( VERTEX_POSBONEWEIGHTNORMTANTEX input ) : SV_POSITION
{
	float4 outPos;

	float4x4 skinMat = gSkinnedMatrices[input.bone.x] * input.weights.x;
	skinMat += gSkinnedMatrices[input.bone.y] * input.weights.y;
	skinMat += gSkinnedMatrices[input.bone.z] * input.weights.z;
	skinMat += gSkinnedMatrices[input.bone.w] * input.weights.w;

	outPos = mul(float4(input.position, 1.0), skinMat);

	return outPos;
}

#endif //DEPTH_POINT_BUMP_SKINNED_VS_HLSL