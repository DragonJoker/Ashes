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
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		DEBUG_DUMP( m_createInfo );
		auto res = m_device.vkCreateRenderPass( m_device
			, &static_cast< VkRenderPassCreateInfo const & >( m_createInfo )
			, nullptr
			, &m_internal );
		checkError( res, "RenderPass creation" );
		registerObject( m_device, "RenderPass", this );
	}

	RenderPass::~RenderPass()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroyRenderPass( m_device
			, m_internal
			, nullptr );
	}

	FrameBufferPtr RenderPass::createFrameBuffer( VkExtent2D const & dimensions
		, ImageViewCRefArray views )const
	{
		return std::make_unique< FrameBuffer >( *this
			, dimensions
			, std::move( views ) );
	}
}
