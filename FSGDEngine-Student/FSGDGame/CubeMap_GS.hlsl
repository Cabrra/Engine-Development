#ifndef CUBE_MAP_GS_HLSL
#define CUBE_MAP_GS_HLSL

#include "VertexLayouts.hlsli"
#include "../EDRendererD3D/LightBuffers.h"
#include "../EDRendererD3D/ConstantBuffers.h"

[maxvertexcount(18)]
void main( triangle float4 input[3] : SV_POSITION, 
		  inout TriangleStream<DepthGeoOut> CubeMapStream )
{
    for( int f = 0; f < 6; ++f )
    {
        // Compute screen coordinates
        DepthGeoOut output;
        output.RTIndex = f;
        for( int v = 0; v < 3; v++ )
        {
            output.position = mul( input[v], pointLightShadowData.viewProjection[f] );

			output.depth.xy = output.position.zw;

            CubeMapStream.Append( output );
        }
        CubeMapStream.RestartStrip();
    }
}

#endif //CUBE_MAP_GS_HLSL