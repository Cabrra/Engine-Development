#ifndef DEFERRED_SPOT_LIGHT_WITH_SHADOWS_HLSL
#define DEFERRED_SPOT_LIGHT_WITH_SHADOWS_HLSL

#include "VertexLayouts.hlsli"
#include "TexturesAndSamplers.hlsli"
#include "LightSupport.hlsli"
#include "ShadowSupport.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"
#include "../EDRendererD3D/LightBuffers.h"

float4 main(VertexWithPositionOut inVert) : SV_TARGET0 
{
	float4 diffuse, specular, posWorld, 
		lightSpacePos, finalAmbient, finalDiffuse, finalSpecular;
	float3 toLight, texCubeCoords, normal;
	float2 texCoord;
	float toLightLength, attenuation = 0, depth, nDotL, specMod,
		spotEffect, shadow;

	return 0;
}
#endif //DEFERRED_SPOT_LIGHT_WITH_SHADOWS_HLSL
