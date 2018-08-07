#pragma once


namespace EDRendererD3D
{
	class RenderTarget;
}

/// Interface for handling GBuffer specific effect varialbes
class GBufferEffectHandler
{
public:
	GBufferEffectHandler();

	/// Activate will set the shader resource variables based on the RenderTargets passed in
	/// \param gBufferTarget - A RenderTarget containing the three GBuffer surfaces
	/// \param gDepthTarget - A RenderTarget containing the GBuffer depth surface
	void Activate(EDRendererD3D::RenderTarget &gBufferTarget, ID3D11ShaderResourceView *depthSRV);

	/// Revert will set the shader resource variables to null.  These is needed to prevent
	/// the textures being assigned to a resource and a OMRenderTarget
	void Revert();

private:
	/// Defines the starting register slot for GBuffer textures
	/// \note : This needs to be refactored to be abstracted, and hopefully detected based on the shader
	const static int GBufferStartSlot = 9;
};