#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	bool isRenderTarget( renderer::ImageUsageFlags const & flags )
	{
		return checkFlag( flags, renderer::ImageUsageFlag::eColourAttachment )
			|| checkFlag( flags, renderer::ImageUsageFlag::eDepthStencilAttachment )
			|| checkFlag( flags, renderer::ImageUsageFlag::eTransientAttachment )
			|| checkFlag( flags, renderer::ImageUsageFlag::eInputAttachment );
	}

	bool isStorage( renderer::ImageUsageFlags const & flags )
	{
		return checkFlag( flags, renderer::ImageUsageFlag::eStorage );
	}
}
