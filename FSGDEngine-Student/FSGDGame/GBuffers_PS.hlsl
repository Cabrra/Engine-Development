#ifndef GBUFFERS_PS_HLSL
#define GBUFFERS_PS_HLSL

#include "VertexLayouts.hlsli"
#include "TexturesAndSamplers.hlsli"

GBufferFragOut main(GBufferVertexOut input)
{
	GBufferFragOut fragOut = (GBufferFragOut)0;

	fragOut.depth = input.depthDiv.x / input.depthDiv.y;
	fragOut.diffuse = diffuseMap.Sample(anisoClampSampler, input.texCoord);
	fragOut.specular = specularMap.Sample(pointClampSampler, input.texCoord);
	//fragOut.normal = (input.normalW.xyz + 1) / 2;

	float4 mycolor = ambientMap.Sample(linearClampSampler, input.texCoord);
	fragOut.diffuse.a = mycolor.r;

	fragOut.normal.r = (input.normalW.r + 1) / 2;
	fragOut.normal.g = (input.normalW.g + 1) / 2;
	fragOut.normal.b = (input.normalW.b + 1) / 2;
	fragOut.normal.a = 0;

	return fragOut;
}

#endif //GBUFFERS_PS_HLSL
