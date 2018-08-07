#include "precompiled.h"
#include "RenderTarget.h"
#include "Renderer.h"

#include "../EDMemoryManager/MemoryManager.h"
using namespace EDMemoryManager;



namespace EDRendererD3D
{
	RenderTargetSurface::RenderTargetSurface()
	{
		textureTargetPtr = 0;
	}

	RenderTargetSurface::~RenderTargetSurface()
	{
		Destroy();
	}

	void RenderTargetSurface::Destroy()
	{
		if (textureTargetPtr)
		{
			delete textureTargetPtr;
			textureTargetPtr = 0;
		}
	}

	void RenderTargetSurface::Create(UINT _width, UINT _height, DXGI_FORMAT format, const char * name)
	{
		Destroy();
		width = _width;
		height = _height;
		// Implement a solution for the Deferred Renderer Lab
		return CreateSolution(_width, _height, format, name);

		/*textureTargetPtr = new RenderTexture2D;
		D3D11_TEXTURE2D_DESC sz;
		ZeroMemory(&sz, sizeof(sz));
		sz.ArraySize = 1;
		sz.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		sz.CPUAccessFlags = 0;
		sz.Format = format;
		sz.Height = height;
		sz.MipLevels = 1;
		sz.MiscFlags = 0;
		sz.SampleDesc.Count = 1;
		sz.SampleDesc.Quality = 0;
		sz.Usage = D3D11_USAGE_DEFAULT;
		sz.Width = width;

		textureTargetPtr->Create(sz, name);*/
	}

	RenderTarget::RenderTarget()
	{
		depthStecilView = 0;
		depthTexturePtr = 0;

		bShouldDeleteDepth = true;
	}

	RenderTarget::~RenderTarget()
	{
		Destroy();
	}

	void RenderTarget::Create(UINT _width, UINT _height, DXGI_FORMAT format)
	{
		Destroy();
		bShouldDeleteDepth = true;

		// Implement a solution for the Deferred Renderer Lab
		return CreateSolution(_width, _height, format);

		/*D3D11_TEXTURE2D_DESC dbDesc;

		ZeroMemory(&dbDesc, sizeof(dbDesc));

		dbDesc.Width = _width;
		dbDesc.Height = _height;
		dbDesc.MipLevels = 1;
		dbDesc.ArraySize = 1;
		dbDesc.Format = format;
		dbDesc.SampleDesc.Count = 1;
		dbDesc.SampleDesc.Quality = 0;
		dbDesc.Usage = D3D11_USAGE_DEFAULT;
		dbDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		dbDesc.CPUAccessFlags = 0;
		dbDesc.MiscFlags = 0;

		Renderer::theDevicePtr->CreateTexture2D(&dbDesc, nullptr, &depthTexturePtr);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		Renderer::theDevicePtr->CreateDepthStencilView(depthTexturePtr, &depthStencilViewDesc, &depthStecilView);*/

	}

	void RenderTarget::Create(ID3D11DepthStencilView * _depthStecilView,
		ID3D11Texture2D *_depthTexturePtr)
	{
		Destroy();
		bShouldDeleteDepth = false;

		depthTexturePtr = _depthTexturePtr;
		depthStecilView = _depthStecilView;
	}

	void RenderTarget::ActivateTarget(DWORD ignored)
	{
		// Implement a solution for the Deferred Renderer Lab
		return ActivateTargetSolution();

		/*ID3D11RenderTargetView* help[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];

		for (int i = 0; i < renderTargets.size(); i++)
		help[i] = renderTargets[i]->GetRenderTargetView();

		if (renderTargets.size() > 0)
		Renderer::theContextPtr->OMSetRenderTargets((UINT)renderTargets.size(), help, depthStecilView);*/
	}

	bool RenderTarget::AddTarget(RenderTargetSurface *view)
	{
		renderTargets.push_back(view);
		return true;
	}

	void RenderTarget::Destroy()
	{
		renderTargets.clear();
	}

	void RenderTarget::ClearRenderTargetView(const FLOAT colorRGBA[4])
	{
		size_t size = renderTargets.size();

		for (size_t i = 0; i < size; ++i)
		{
			Renderer::theContextPtr->ClearRenderTargetView(renderTargets[i]->GetRenderTargetView(),
				colorRGBA);
		}
	}

	void RenderTarget::ClearDepthStencilView(UINT clearFlags, FLOAT depth, UINT8 stencil)
	{
		if (depthStecilView)
			Renderer::theContextPtr->ClearDepthStencilView(depthStecilView, clearFlags, depth,
				stencil);
	}
}