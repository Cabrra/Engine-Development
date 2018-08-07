#ifndef DEPTH_DIRECTIONAL_VS_HLSL
#define DEPTH_DIRECTIONAL_VS_HLSL

#include "VertexLayouts.hlsli"
#include "../EDRendererD3D/LightBuffers.h"
#include "../EDRendererD3D/ConstantBuffers.h"

DepthVertOut main( VERTEX_POS input )
{
	DepthVertOut outVert;
	
	outVert.position = mul(float4(input.position, 1.0), gWorld);
	outVert.position = mul(outVert.position, DirLight.viewProjection[DirLight.currentIndex]);
	//outVert.position = mul(outVert.position, DirLight.viewProjection[1]);

	outVert.position.z = max(0, outVert.position.z);
    outVert.depth.xy = outVert.position.zw;
	//outVert.depth.x = max(0, outVert.depth.x);
	return outVert;
}

#endif //DEPTH_DIRECTIONAL_VS_HLSL