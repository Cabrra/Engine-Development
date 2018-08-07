#ifndef DRAW_EDGE_PS_HLSL
#define DRAW_EDGE_PS_HLSL

#include "VertexLayouts.hlsli"
#include "TexturesAndSamplers.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"

static float theNormalThreshold = 0.25;
static float theDepthThreshold = 0.005;

static float theNormalSensitivity = 1;
static float theDepthSensitivity = 10;

float4 main(TwoDVertexOut input) : SV_TARGET0
{
	float4 sceneColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 normalTL, normalTR, normalBR, normalBL, diagonalDeltaNorm;
	float depthTL, depthTR, depthBR, depthBL, normalDelta, depthDelta, edgeAmount;


    return float4(sceneColor.rgb, 1);
}
//87
#endif //DRAW_EDGE_PS_HLSL
