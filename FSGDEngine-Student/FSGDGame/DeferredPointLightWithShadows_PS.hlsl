#ifndef DEFERRED_POINT_LIGHT_WITH_SHADOWS_HLSL
#define DEFERRED_POINT_LIGHT_WITH_SHADOWS_HLSL

#include "VertexLayouts.hlsli"
#include "TexturesAndSamplers.hlsli"
#include "LightSupport.hlsli"
#include "ShadowSupport.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"
#include "../EDRendererD3D/LightBuffers.h"

float4 main(VertexWithPositionOut inVert) : SV_TARGET0
{
	float4 diffuse, specular, posWorld, finalAmbient, finalDiffuse, finalSpecular;
	float3 toLight, normal;
	float2 texCoord;
	float toLightLength, attenuation, nDotL, lightMapDepth, shadow, depth, specMod;

	inVert.pixelPosition.x /= inVert.pixelPosition.w;
	inVert.pixelPosition.y /= inVert.pixelPosition.w;

	texCoord = float2(inVert.pixelPosition.x, -inVert.pixelPosition.y) * 0.5f + 0.5f;
	depth = depthGBuffer.Sample(linearClampSampler, texCoord).x;
	posWorld = CalculateWorldSpacePosition(inVert.pixelPosition.xy, depth, gInvViewProj);

	toLight = pointLight.position.xyz - posWorld.xyz;
	toLightLength = length(toLight);

	//shadow
	shadow = CalculatePointLightShadow(toLight, toLightLength);
	//

	toLight /= toLightLength;

	diffuse = diffuseGBuffer.Sample(anisoClampSampler, texCoord);
	specular = specularGBuffer.Sample(anisoClampSampler, texCoord);
	normal = normalGBuffer.Sample(linearClampSampler, texCoord).xyz;
	
	normal.x = normal.r * 2 - 1;
	normal.y = normal.g * 2 - 1;
	normal.z = normal.b * 2 - 1;

	attenuation = CalculateAttenuation(pointLight.attenuation, toLightLength, pointLight.range);

	nDotL = saturate(dot(normal, toLight));
	finalDiffuse = nDotL * diffuse * float4(pointLight.diffuseColor, 1.0);

	finalAmbient = float4(diffuse.xyz * pointLight.ambientColor.xyz, 1.0);
	finalAmbient *= diffuse.w;

	float3 reflectionVector = reflect(-toLight, normal).xyz;

	float3 directionToCamera = normalize(gCameraPos - posWorld.xyz);
	specMod = pointLight.specularIntensity * pow(saturate(dot(reflectionVector, directionToCamera)), pointLight.specularPower);
	finalSpecular = float4(specular * specMod * (nDotL>0) * pointLight.specularColor, 1.0);


	return attenuation * (finalAmbient + shadow * (finalDiffuse + finalSpecular));
}
#endif //DEFERRED_POINT_LIGHT_WITH_SHADOWS_HLSL
