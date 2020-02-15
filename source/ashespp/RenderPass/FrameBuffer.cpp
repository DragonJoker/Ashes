/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/RenderPass/FrameBuffer.hpp"

#include "ashespp/Command/Queue.hpp"
#include "ashespp/Core/Device.hpp"
#include "ashespp/Image/ImageView.hpp"
#include "ashespp/RenderPass/RenderPass.hpp"

namespace ashes
{
	FrameBuffer::FrameBuffer( RenderPass const & renderPass
		, VkExtent2D const & dimensions
		, ImageViewCRefArray views
		, uint32_t layers )
		: m_device{ renderPass.getDevice() }
		, m_dimensions{ dimensions }
		, m_views{ std::move( views ) }
	{
		auto vkattachments = makeVkArray< VkImageView >( m_views );

		VkFramebufferCreateInfo createInfo
		{
			VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			nullptr,
			0u,                                                 // flags
			renderPass,                                         // renderPass
			static_cast< uint32_t >( vkattachments.size() ),    // attachmentCount
			vkattachments.data(),                               // pAttachments
			dimensions.width,                                   // width
			dimensions.height,                                  // height
			1u                                                  // layers
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateFramebuffer( m_device
			, &createInfo
			, nullptr
			, &m_internal );
		checkError( res, "FrameBuffer creation" );
		registerObject( m_device, "FrameBuffer", this );
	}

	FrameBuffer::~FrameBuffer()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroyFramebuffer( m_device, m_internal, nullptr );
	}
}
