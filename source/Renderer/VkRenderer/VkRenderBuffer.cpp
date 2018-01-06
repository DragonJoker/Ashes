#include "VkRenderBuffer.hpp"

#include "VkDevice.hpp"
#include "VkImageMemoryBarrier.hpp"

#include <VkLib/Queue.hpp>

namespace vk_renderer
{
	RenderBuffer::RenderBuffer( renderer::Device const & device
		, utils::PixelFormat format
		, renderer::IVec2 const & size )
		: renderer::RenderBuffer{ device, format, size }
		, m_image{ static_cast< Device const & >( device ).getDevice().createImage( convert( format )
			, size.x
			, size.y
			, convert( renderer::MemoryPropertyFlags{ renderer::MemoryPropertyFlag::eLazilyAllocated | renderer::MemoryPropertyFlag::eDeviceLocal } ) ) }
		, m_texture{ device, *m_image }
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
