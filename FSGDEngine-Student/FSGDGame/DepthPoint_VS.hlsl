#include "DepthPoint_VS.hlsli"

float4 main( VERTEX_POS input ) : SV_POSITION
{
	return DepthPoint(input, 0);
}