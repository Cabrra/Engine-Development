#ifndef DEPTH_OF_FIELD_PS_HLSL
#define DEPTH_OF_FIELD_PS_HLSL

#include "VertexLayouts.hlsli"
#include "TexturesAndSamplers.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"

//const static float gFocusDistance = 15.0f;
//const static float gFocusRange = 50.0f;

float4 main(TwoDVertexOut input) : SV_TARGET
{
	float4 sharpColor, blurColor, finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
float depth, camDepth, blur;

depth = depthGBuffer.Sample(linearClampSampler, input.texCoord).x;

sharpColor = prePostMap.Sample(linearClampSampler, input.texCoord);
blurColor = postSourceMap.Sample(linearClampSampler, input.texCoord);

camDepth = gPlaneY / (gPlaneX + depth);

blur = saturate(abs(camDepth - gFocusDistance) / gFocusRange);

finalColor = lerp(sharpColor, blurColor, blur);

return finalColor;
}

#endif //DEPTH_OF_FIELD_PS_HLSL
