/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/RenderPass/RenderPass.hpp"

#include "AshesPP/Core/Device.hpp"
#include "AshesPP/Image/ImageView.hpp"
#include "AshesPP/RenderPass/FrameBuffer.hpp"

namespace ashes
{
	RenderPass::RenderPass( Device const & device
		, VkRenderPassCreateInfo const & createInfo )
		: m_device{ device }
		, m_createInfo{ createInfo }
	{
		DEBUG_DUMP( m_createInfo );
		auto res = m_device.vkCreateRenderPass( m_device
			, &m_createInfo
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
		, ImageViewPtrArray views )const
	{
		return std::make_unique< FrameBuffer >( *this
			, dimensions
			, std::move( views ) );
	}
}
