#ifndef GBUFFERS_TESSELLATED_TERRAIN_DS_HLSL
#define GBUFFERS_TESSELLATED_TERRAIN_DS_HLSL

#include "VertexLayouts.hlsli"
#include "TerrainSupport.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"

#define NUM_CONTROL_POINTS 4

// Method for bilinear interpolation
float3 Bilerp(float3 v0, float3 v1, float3 v2, float3 v3, float2 i)
{

	return float3(0, 0, 0);
}

float3 TessellatedWorldPos(
	float2 UV : SV_DomainLocation,
	const OutputPatch<TESS_TERRAIN_HS_OUT, 4> terrainQuad)
{
	float3 worldPos;

	return worldPos;
}

// From nVidia
// The domain shader is run once per vertex and calculates the final vertex's position
// and attributes.  It receives the UVW from the fixed function tessellator and the
// control point outputs from the hull shader.  Since we are using the DirectX 11
// Tessellation pipeline, it is the domain shader's responsibility to calculate the
// final SV_POSITION for each vertex.

// The input SV_DomainLocation to the domain shader comes from fixed function
// tessellator.  And the OutputPatch comes from the hull shader.  From these, you
// must calculate the final vertex position, color, texcoords, and other attributes.

// The output from the domain shader will be a vertex that will go to the video card's
// rasterization pipeline and get drawn to the screen.
[domain("quad")]
TESS_TERRAIN_DS_OUT main(
	TESS_TERRAIN_HS_DATA_OUT input,
	float2 UV : SV_DomainLocation,
	const OutputPatch<TESS_TERRAIN_HS_OUT, NUM_CONTROL_POINTS> terrainQuad)
{
	TESS_TERRAIN_DS_OUT Output = (TESS_TERRAIN_DS_OUT)0;

	// Interpolate nearby debug transition colors
	Output.debugColor = lerpDebugColors(input.debugColor, UV);

	return Output;    
}

#endif //GBUFFERS_TESSELLATED_TERRAIN_DS_HLSL
