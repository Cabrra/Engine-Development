#ifndef GBUFFERS_BUMP_PS_HLSL
#define GBUFFERS_BUMP_PS_HLSL

#include "VertexLayouts.hlsli"
#include "TexturesAndSamplers.hlsli"
#include "../EDRendererD3D/ConstantBuffers.h"

GBufferFragOut main(GBufferBumpVertexOut input)
{
	GBufferFragOut output = (GBufferFragOut)0;

	output.depth = input.depthDiv.x / input.depthDiv.y;
	output.diffuse = diffuseMap.Sample(anisoClampSampler, input.texCoord);
	output.specular = specularMap.Sample(pointClampSampler, input.texCoord);

	float4 mycolor = ambientMap.Sample(linearClampSampler, input.texCoord);
	output.diffuse.a = mycolor.r;

	float4 colornormal = normalMap.Sample(linearClampSampler, input.texCoord);
	output.normal.x = colornormal.r * 2 - 1;
	output.normal.y = colornormal.g * 2 - 1;
	output.normal.z = colornormal.b * 2 - 1;

	output.normal.xyz = mul((float3)output.normal, input.tbn);

	output.normal.r = (output.normal.x + 1) / 2;
	output.normal.g = (output.normal.y + 1) / 2;
	output.normal.b = (output.normal.z + 1) / 2;
	return output;
}

#endif //GBUFFERS_BUMP_PS_HLSL
