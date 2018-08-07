#ifndef GBUFFERS_TESSELLATED_TERRAIN_HS
#define GBUFFERS_TESSELLATED_TERRAIN_HS

#include "VertexLayouts.hlsli"
#include "TerrainSupport.hlsli"

#include "../EDRendererD3D/ConstantBuffers.h"

// Work assumes a "quad" like patch
#define NUM_CONTROL_POINTS 4

// Projects a sphere diameter large in screen space to calculate desired tesselation factor
float SphereToScreenSpaceTessellation(float3 p0, float3 p1, float diameter)
{
	int ret = 1;

	return ret;
}

// A conservative test if a patch is in the frustum viewing area, false positives are possible
int inFrustum(const float3 center, float diameter)
{
	int ret = 0;


	return ret;
}

// Patch Constant Function
// This method will cull patches outside of view, and set tesselation factotrs for in view patches
TESS_TERRAIN_HS_DATA_OUT CalcHSPatchConstants(
	InputPatch<TESS_TERRAIN_VS_OUT, NUM_CONTROL_POINTS> ip)
{
	TESS_TERRAIN_HS_DATA_OUT Output = (TESS_TERRAIN_HS_DATA_OUT)0;

	Output.inside[0] = 1;
	Output.inside[1] = 1;
	Output.edges[0] =  1;
	Output.edges[1] =  1;
	Output.edges[2] =  1;
	Output.edges[3] =  1;


	// Debug colors show the tesselation factors of a primitive. The below code will map
	// the tesselation factors to 0-5 based on their nearest power of 2 ranging 2-64
	Output.debugColor[0] = DEBUG_COLORS[clamp(log2(Output.edges[0]), 0, 5)];
	Output.debugColor[1] = DEBUG_COLORS[clamp(log2(Output.edges[1]), 0, 5)];
	Output.debugColor[2] = DEBUG_COLORS[clamp(log2(Output.edges[2]), 0, 5)];
	Output.debugColor[3] = DEBUG_COLORS[clamp(log2(Output.edges[3]), 0, 5)];
	Output.debugColor[4] = DEBUG_COLORS[clamp(log2(Output.inside[0]), 0, 5)];
	return Output;
}


[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("CalcHSPatchConstants")]
TESS_TERRAIN_HS_OUT main( InputPatch<TESS_TERRAIN_VS_OUT, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID)
{
	TESS_TERRAIN_HS_OUT Output;

	// This is just a pass through method
	// Tesselation factors are calculated in the patch-constant method
	Output.position = ip[i].position;

	return Output;
}

#undef NUM_CONTROL_POINTS
#endif //GBUFFERS_TESSELLATED_TERRAIN_HS
