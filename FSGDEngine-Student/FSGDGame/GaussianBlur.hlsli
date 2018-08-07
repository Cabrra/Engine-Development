#ifndef GAUSSIAN_BLUR_HLSLI
#define GAUSSIAN_BLUR_HLSLI

#include "TexturesAndSamplers.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"

const static float3 gBlurWeights = float3(0.24197073, 0.053990975, 0.0044318493);
const static float gCenterBlurWeights = 0.39894229f;

struct BlurVertexOut
{
	float4 position : SV_POSITION0;
	float2 texCoord : TEXCOORD0;
	float2 positiveOffsets[3] : TEXCOORD1;
    float2 negativeOffsets[3] : TEXCOORD4;
};

BlurVertexOut BlurVS(float3 position : POSITION0, float2 texCoord : TEXCOORD0,
	uniform float2 direction)
{
	BlurVertexOut outVert = (BlurVertexOut)0;
	return outVert;
}

float4 BlurPS(BlurVertexOut inVert, uniform float ScaleUp) : COLOR0
{
	float3x4 positiveSamples, negativeSamples;
	float4 color = float4(0, 0, 0, 0);

	return color * ScaleUp;
}


#endif //GAUSSIAN_BLUR_HLSLI
