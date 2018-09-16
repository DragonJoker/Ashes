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
		return checkFlag( flags, ashes::ImageUsageFlag::eSampled )
			/*|| checkFlag( flags, ashes::ImageUsageFlag::eTransferSrc )*/;
	}

	bool isRenderable( ashes::ImageUsageFlags const & flags
		, ashes::Format format
		, uint32_t mipLevels )
	{
		return isRenderTarget( flags )
			|| ( mipLevels > 1 && !isCompressedFormat( format ) );
	}

	bool isMipmapped( ashes::ImageUsageFlags const & flags
		, ashes::Format format
		, uint32_t mipLevels )
	{
		return isRenderable( flags, format, mipLevels )
			&& isSampled( flags );
	}
}
