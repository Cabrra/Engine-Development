#ifndef GBUFFERS_TESS_VS_HLSL
#define GBUFFERS_TESS_VS_HLSL

#include "VertexLayouts.hlsli"
#include "Tesselation.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"

GBufferTessVertexOut main( VERTEX_POSNORMTEX input )
{
	GBufferTessVertexOut output = (GBufferTessVertexOut)0;

	return output;
}

#endif //GBUFFERS_TESS_VS_HLSL
