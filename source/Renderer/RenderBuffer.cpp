#include "RenderBuffer.hpp"

#include "CommandBuffer.hpp"
#include "ImageMemoryBarrier.hpp"
#include "RenderingResources.hpp"
#include "StagingBuffer.hpp"

#include <VkLib/Queue.hpp>

namespace renderer
{
	RenderBuffer::RenderBuffer( Device const & device
		, utils::PixelFormat format
		, IVec2 const & size )
		: m_device{ device }
		, m_format{ format }
		, m_size{ size }
		, m_texture{ m_device.getDevice().createImage( convert( m_format )
			, m_size.x
			, m_size.y
			, convert( MemoryPropertyFlags{ MemoryPropertyFlag::eLazilyAllocated | MemoryPropertyFlag::eDeviceLocal } ) ) }
	{
		makeDepthStencilAttachment();
	}

	ImageMemoryBarrier RenderBuffer::makeColourAttachment()const
	{
		return ImageMemoryBarrier{ m_texture->makeColourAttachment() };
	}

	ImageMemoryBarrier RenderBuffer::makeDepthStencilAttachment()const
	{
		return ImageMemoryBarrier{ m_texture->makeDepthStencilAttachment() };
	}
}
