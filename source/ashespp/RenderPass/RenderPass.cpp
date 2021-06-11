/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/RenderPass/RenderPass.hpp"

#include "ashespp/Core/Device.hpp"
#include "ashespp/Image/ImageView.hpp"
#include "ashespp/RenderPass/FrameBuffer.hpp"

namespace ashes
{
	RenderPass::RenderPass( Device const & device
		, RenderPassCreateInfo createInfo )
		: RenderPass{ device, "RenderPass", std::move( createInfo ) }
	{
	}

	RenderPass::RenderPass( Device const & device
		, std::string const & debugName
		, RenderPassCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		DEBUG_DUMP( m_createInfo );
		auto res = m_device.vkCreateRenderPass( m_device
			, &static_cast< VkRenderPassCreateInfo const & >( m_createInfo )
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "RenderPass creation" );
		registerObject( m_device, debugName, *this );
	}

	RenderPass::~RenderPass()
	{
		unregisterObject( m_device, *this );
		m_device.vkDestroyRenderPass( m_device
			, m_internal
			, m_device.getAllocationCallbacks() );
	}

	FrameBufferPtr RenderPass::createFrameBuffer( VkFramebufferCreateInfo info )const
	{
		return createFrameBuffer( "FrameBuffer", info );
	}

	FrameBufferPtr RenderPass::createFrameBuffer( std::string const & debugName
		, VkFramebufferCreateInfo info )const
	{
		info.renderPass = *this;
		return std::make_unique< FrameBuffer >( m_device
			, debugName
			, info );
	}

	FrameBufferPtr RenderPass::createFrameBuffer( VkExtent2D const & dimensions
		, ImageViewCRefArray views
		, uint32_t layers )const
	{
		return createFrameBuffer( "FrameBuffer"
			, dimensions
			, views
			, layers );
	}

	FrameBufferPtr RenderPass::createFrameBuffer( std::string const & debugName
		, VkExtent2D const & dimensions
		, ImageViewCRefArray views
		, uint32_t layers )const
	{
		return std::make_unique< FrameBuffer >( debugName
			, *this
			, dimensions
			, std::move( views )
			, layers );
	}
}
