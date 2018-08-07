#ifndef SHADOW_SUPPORT_HLSLI
#define SHADOW_SUPPORT_HLSLI

float CalculatePointLightShadow(float3 notNormalizedToLight, float toLightLength)
{
	float sDepth = max(max(abs(notNormalizedToLight.x), abs(notNormalizedToLight.y)), abs(notNormalizedToLight.z));
	sDepth = pointLightShadowData.planeY / sDepth - pointLightShadowData.planeX;
	float3 texCoord = normalize(-notNormalizedToLight);
	return point0ShadowMap.SampleCmpLevelZero(linearBorderCompareSampler, texCoord, sDepth - pointLightShadowData.shadowBias);
}

// Tests if a given location is in shadow
// shadowMap - the shadow map to test against
// texCoord - The central coordinate in the shadow map representing this location
// depth - The depth of the fragment we are testing whether is in shadow
// (optional) texCoordOffset - An offset, in pixels, to test around texCoord
// returns A value between 0 and 1, where 0 represents a location completely in shadow.  
// Shadow comparisons use the SampleCmpLevelZero method which will blur multiple samples, 
// resulting in softer edges on shadows
float CalculateShadow(Texture2D shadowMap, float2 texCoord,
	float depth, int2 texCoordOffset = float2(0, 0))
{

	float samp = shadowMap.SampleCmpLevelZero(linearBorderCompareSampler, texCoord, depth - gShadowDepthOffset, texCoordOffset);
	//float shadow = (depth <= samp);
	return samp;
}

// Tests if a given location is in shadow, using a 3x3 kernal to average neighboring shadow 
// values resulting in softer smoother shadow penumbra
// shadowMap - the shadow map to test against
// texCoord - The central coordinate in the shadow map representing this location
// depth - The depth of the fragment we are testing whether is in shadow
float CalculateShadowPCF(Texture2D shadowMap, float2 texCoord, float depth)
{
	//return CalculateShadow(shadowMap, texCoord, depth);
	float sum = 0;

	sum += CalculateShadow(shadowMap, texCoord, depth, int2(-1, -1));
	sum += CalculateShadow(shadowMap, texCoord, depth, int2(-1, 0));
	sum += CalculateShadow(shadowMap, texCoord, depth, int2(-1, 1));
	sum += CalculateShadow(shadowMap, texCoord, depth, int2(0, -1));
	sum += CalculateShadow(shadowMap, texCoord, depth, int2(0, 0));
	sum += CalculateShadow(shadowMap, texCoord, depth, int2(0, 1));
	sum += CalculateShadow(shadowMap, texCoord, depth, int2(1, -1));
	sum += CalculateShadow(shadowMap, texCoord, depth, int2(1, 0));
	sum += CalculateShadow(shadowMap, texCoord, depth, int2(1, 1));

	sum /= 9;
	return sum;
}

#endif //SHADOW_SUPPORT_HLSLI
