/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Core/SwapChain.hpp"

#include "Ashes/Command/CommandBuffer.hpp"
#include "Ashes/Core/Device.hpp"
#include "Ashes/Core/Exception.hpp"
#include "Ashes/RenderPass/AttachmentDescription.hpp"
#include "Ashes/RenderPass/FrameBuffer.hpp"
#include "Ashes/RenderPass/FrameBufferAttachment.hpp"
#include "Ashes/RenderPass/RenderPass.hpp"

namespace ashes
{
	SwapChain::SwapChain( Device const & device
		, SwapChainCreateInfo createInfo )
		: m_device{ device }
		, m_surface{ device.getSurface() }
		, m_createInfo{ std::move( createInfo ) }
	{
		registerObject( m_device, "SwapChain", this );
	}

	SwapChain::~SwapChain()
	{
		unregisterObject( m_device, this );
	}

	Result SwapChain::acquireNextImage( uint64_t timeout
		, Semaphore const & semaphore
		, Fence const & fence
		, uint32_t & imageIndex )const
	{
		return acquireNextImage( timeout
			, &semaphore
			, &fence
			, imageIndex );
	}

	Result SwapChain::acquireNextImage( uint64_t timeout
		, Fence const & fence
		, uint32_t & imageIndex )const
	{
		return acquireNextImage( timeout
			, nullptr
			, &fence
			, imageIndex );
	}

	Result SwapChain::acquireNextImage( uint64_t timeout
		, Semaphore const & semaphore
		, uint32_t & imageIndex )const
	{
		return acquireNextImage( timeout
			, &semaphore
			, nullptr
			, imageIndex );
	}

	Result SwapChain::acquireNextImage( uint64_t timeout
		, uint32_t & imageIndex )const
	{
		return acquireNextImage( timeout
			, nullptr
			, nullptr
			, imageIndex );
	}
}
