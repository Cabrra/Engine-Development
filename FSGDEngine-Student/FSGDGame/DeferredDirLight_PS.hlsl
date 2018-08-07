#ifndef DEFERRED_DIR_LIGHT_HLSL
#define DEFERRED_DIR_LIGHT_HLSL

#include "VertexLayouts.hlsli"
#include "TexturesAndSamplers.hlsli"
#include "LightSupport.hlsli"
#include "ShadowSupport.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"
#include "../EDRendererD3D/LightBuffers.h"

float4 main(TwoDVertexWithPositionOut inVert) : SV_TARGET0
{
	float4 specular, posWorld, lightSpacePos, diffuse,
	finalAmbient, finalDiffuse, finalSpecular;
	float3 normal, reflectionVector, directionToCamera;
	float2 texCoord;
	float depth, lightMapDepth, shadow, offset, nDotL, specMod;

	diffuse = diffuseGBuffer.Sample(anisoClampSampler, inVert.texCoord);
	specular = specularGBuffer.Sample(anisoClampSampler, inVert.texCoord);
	normal = normalGBuffer.Sample(linearClampSampler, inVert.texCoord).xyz;

	normal.x = normal.r * 2 - 1;
	normal.y = normal.g * 2 - 1;
	normal.z = normal.b * 2 - 1;

	depth = depthGBuffer.Sample(linearClampSampler, inVert.texCoord).x;

	nDotL = saturate(dot(normal, -DirLight.direction));
	finalDiffuse = nDotL * diffuse * float4(DirLight.diffuseColor, 1.0);

	finalAmbient = float4(diffuse.xyz * DirLight.ambientColor.xyz, 1.0);
	finalAmbient *= diffuse.w;

	posWorld = CalculateWorldSpacePosition(inVert.pixelPosition.xy, depth, gInvViewProj);
	reflectionVector = reflect(DirLight.direction, normal).xyz;

	directionToCamera = normalize(gCameraPos - posWorld);
	specMod = DirLight.specularIntensity * pow(saturate(dot(reflectionVector, directionToCamera)), DirLight.specularPower);
	finalSpecular = float4(specular * specMod * nDotL * DirLight.specularColor, 1.0);

	return finalAmbient + finalDiffuse + finalSpecular;
}
#endif //DEFERRED_DIR_LIGHT_HLSL
