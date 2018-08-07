#ifndef DEPTH_SPOT_VS_HLSL
#define DEPTH_SPOT_VS_HLSL

#include "../EDRendererD3D/ConstantBuffers.h"
#include "VertexLayouts.hlsli"
#include "../EDRendererD3D/LightBuffers.h"

DepthVertOut main( VERTEX_POS input )
{
	DepthVertOut outVert;
	
	outVert.position = mul(float4(input.position, 1.0), gWorld);
	outVert.position = mul(outVert.position, spotLightShadowData.viewProjection);

    outVert.depth.xy = outVert.position.zw;

	return outVert;
}

#endif //DEPTH_SPOT_VS_HLSL