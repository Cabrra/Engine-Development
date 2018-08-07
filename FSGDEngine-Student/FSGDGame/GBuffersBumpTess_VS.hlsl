#ifndef GBUFFERS_BUMP_TESS_VS_HLSL
#define GBUFFERS_BUMP_TESS_VS_HLSL

#include "VertexLayouts.hlsli"
#include "Tesselation.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"


GBufferBumpTessVertexOut main( VERTEX_POSNORMTANTEX input )
{
	GBufferBumpTessVertexOut output = (GBufferBumpTessVertexOut)0;
	return output;
}

#endif //GBUFFERS_BUMP_TESS_VS_HLSL
