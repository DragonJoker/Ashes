/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/GlCommandPool.hpp"

#include "Command/GlCommandBuffer.hpp"
#include "Core/GlDevice.hpp"

namespace gl_renderer
{
	CommandPool::CommandPool( ashes::Device const & device
		, uint32_t queueFamilyIndex
		, ashes::CommandPoolCreateFlags flags )
		: ashes::CommandPool{ device, queueFamilyIndex, flags }
	{
	}

	ashes::CommandBufferPtr CommandPool::createCommandBuffer( bool primary )const
	{
		return std::make_unique< CommandBuffer >( static_cast< Device const & >( m_device )
			, *this
			, primary );
	}
}
