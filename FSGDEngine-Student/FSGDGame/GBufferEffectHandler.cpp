#include "GBufferEffectHandler.h"
#include "..\EDRendererD3D\RenderTarget.h"
#include "..\EDRendererD3D\Renderer.h"

using EDRendererD3D::RenderTarget;
using EDRendererD3D::Renderer;

GBufferEffectHandler::GBufferEffectHandler()
{

}

void GBufferEffectHandler::Activate(RenderTarget &gBufferTarget, ID3D11ShaderResourceView *depthSRV)
{
	ID3D11ShaderResourceView *srvs[4] = {
		gBufferTarget.GetShaderResourceView(0),
		gBufferTarget.GetShaderResourceView(2),
		gBufferTarget.GetShaderResourceView(1),
		gBufferTarget.GetShaderResourceView(3) };

	Renderer::theContextPtr->PSSetShaderResources(GBufferStartSlot, 4, srvs);
	Renderer::theContextPtr->DSSetShaderResources(GBufferStartSlot, 4, srvs);
}

void GBufferEffectHandler::Revert()
{
	ID3D11ShaderResourceView *srvs[4] = { 0, 0, 0, 0 };

	Renderer::theContextPtr->PSSetShaderResources(GBufferStartSlot, 4, srvs);
	Renderer::theContextPtr->DSSetShaderResources(GBufferStartSlot, 4, srvs);
}