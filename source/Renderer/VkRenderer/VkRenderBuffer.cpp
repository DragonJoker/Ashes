#include "VkRenderBuffer.hpp"

#include "VkDevice.hpp"
#include "VkImageMemoryBarrier.hpp"

namespace vk_renderer
{
	RenderBuffer::RenderBuffer( Device const & device
		, renderer::PixelFormat format
		, renderer::IVec2 const & size )
		: renderer::RenderBuffer{ device, format, size }
		, m_format{ convert( format ) }
		, m_texture{ device
			, format
			, size
			, isDepthStencilFormat( m_format )
				? renderer::ImageUsageFlag::eDepthStencilAttachment
					: isDepthFormat( m_format )
						? renderer::ImageUsageFlag::eDepthStencilAttachment
						: isStencilFormat( m_format )
							? renderer::ImageUsageFlag::eDepthStencilAttachment
							: renderer::ImageUsageFlag::eColourAttachment
			, renderer::ImageTiling::eOptimal
			, renderer::MemoryPropertyFlag::eLazilyAllocated | renderer::MemoryPropertyFlag::eDeviceLocal }
	{
		makeDepthStencilAttachment();
	}

	renderer::ImageMemoryBarrier RenderBuffer::makeColourAttachment()const
	{
		return m_texture.makeColourAttachment();
	}

	renderer::ImageMemoryBarrier RenderBuffer::makeDepthStencilAttachment()const
	{
		return m_texture.makeDepthStencilAttachment();
	}
}
