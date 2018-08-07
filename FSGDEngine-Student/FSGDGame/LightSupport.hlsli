#ifndef LIGHT_SUPPORT_HLSLI
#define LIGHT_SUPPORT_HLSLI

#include "../EDRendererD3D/ConstantBuffers.h"

float CalculateAttenuation(float3 attenuationFactors, float distanceToLight, float lightRange,
	float numerator = 1.0)
{
	float atten = numerator / (attenuationFactors.x + attenuationFactors.y * distanceToLight * attenuationFactors.z * distanceToLight * distanceToLight);
	float ratio = distanceToLight / lightRange;
	float damp = saturate(1 - pow(ratio, 2));
	atten *= damp;
	return atten;
}

float4 CalculateWorldSpacePosition(float2 pixelPosition, float pixelDepth,
	float4x4 inverseViewProjection)
{
	float4 mein = float4(pixelPosition.xy, pixelDepth, 1.0);
	mein = mul(mein, inverseViewProjection);
	mein /= mein.w;

	return mein;
}

// Calculates how bright of a specualr highlight/reflection a given location should have
// normalizedToLight - a direction vector pointing to the light
// normal - a direction vector representing the orientation of the surface
// worldSpaceFragPos - The location of the fragment in world space
// specularLightIntensisty - The lights specular intensisty
// specularLightPower - The lights specular power
float CalculateSpecularAmount(float3 normalizedToLight, float3 normal, float3 worldSpaceFragPos,
							  float specularLightIntensisty, float specularLightPower)
{
	return 0;
}
#endif //LIGHT_SUPPORT_HLSLI
