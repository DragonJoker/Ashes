#include "Image/TextureView.hpp"

namespace renderer
{
	TextureView::TextureView( Device const & device
		, Texture const & image
		, PixelFormat format
		, uint32_t baseMipLevel
		, uint32_t levelCount
		, uint32_t baseArrayLayer
		, uint32_t layerCount )
		: m_device{ device }
		, m_format{ format }
		, m_image{ image }
		, m_subResourceRange
		{
			isDepthStencilFormat( m_format )
				? renderer::ImageAspectFlags( renderer::ImageAspectFlag::eDepth | renderer::ImageAspectFlag::eStencil )
				: isDepthFormat( m_format )
					? renderer::ImageAspectFlags( renderer::ImageAspectFlag::eDepth )
					: isStencilFormat( m_format )
						? renderer::ImageAspectFlags( renderer::ImageAspectFlag::eStencil )
						: renderer::ImageAspectFlags( renderer::ImageAspectFlag::eColour ),
			baseMipLevel,
			levelCount,
			baseArrayLayer,
			layerCount
		}
	{
	}
}
