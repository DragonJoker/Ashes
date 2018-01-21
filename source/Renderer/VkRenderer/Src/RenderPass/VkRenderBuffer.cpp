#include "VkRenderBuffer.hpp"

#include "Core/VkDevice.hpp"
#include "Sync/VkImageMemoryBarrier.hpp"

namespace vk_renderer
{
	RenderBuffer::RenderBuffer( Device const & device
		, renderer::PixelFormat format
		, renderer::UIVec2 const & size )
		: renderer::RenderBuffer{ device, format, size }
		, m_format{ convert( format ) }
		, m_texture{ device
			, format
			, size
			, renderer::isDepthStencilFormat( format )
				? renderer::ImageUsageFlag::eDepthStencilAttachment
					: renderer::isDepthFormat( format )
						? renderer::ImageUsageFlag::eDepthStencilAttachment
						: renderer::isStencilFormat( format )
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
