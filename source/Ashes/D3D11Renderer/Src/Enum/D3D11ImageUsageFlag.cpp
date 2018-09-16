#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	bool isRenderTarget( ashes::ImageUsageFlags const & flags )
	{
		return checkFlag( flags, ashes::ImageUsageFlag::eColourAttachment )
			|| checkFlag( flags, ashes::ImageUsageFlag::eDepthStencilAttachment )
			|| checkFlag( flags, ashes::ImageUsageFlag::eTransientAttachment )
			|| checkFlag( flags, ashes::ImageUsageFlag::eInputAttachment );
	}

	bool isStorage( ashes::ImageUsageFlags const & flags )
	{
		return checkFlag( flags, ashes::ImageUsageFlag::eStorage );
	}

	bool isSampled( ashes::ImageUsageFlags const & flags )
	{
		return checkFlag( flags, ashes::ImageUsageFlag::eSampled );
	}
}
