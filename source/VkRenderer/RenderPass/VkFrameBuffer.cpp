/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "RenderPass/VkFrameBuffer.hpp"

#include "Command/VkCommandBuffer.hpp"
#include "Command/VkCommandPool.hpp"
#include "Command/VkQueue.hpp"
#include "Core/VkDevice.hpp"
#include "Image/VkTexture.hpp"
#include "Image/VkTextureView.hpp"
#include "RenderPass/VkRenderPass.hpp"
#include "Sync/VkFence.hpp"
#include "Sync/VkImageMemoryBarrier.hpp"

#include <Ashes/RenderPass/FrameBufferAttachment.hpp>

namespace vk_renderer
{
	namespace
	{
		TextureViewCRefArray convert( ashes::FrameBufferAttachmentArray const & attachs )
		{
			TextureViewCRefArray result;
			result.reserve( attachs.size() );

			for ( auto & attach : attachs )
			{
				result.emplace_back( static_cast< TextureView const & >( attach.getView() ) );
			}

			return result;
		}
	}

	FrameBuffer::FrameBuffer( Device const & device
		, RenderPass const & renderPass
		, ashes::Extent2D const & dimensions
		, ashes::FrameBufferAttachmentArray attachments )
		: ashes::FrameBuffer{ renderPass, dimensions, std::move( attachments ) }
		, m_device{ device }
		, m_views{ convert( m_attachments ) }
		, m_dimensions{ dimensions }
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
		auto res = m_device.vkCreateFramebuffer( device
			, &createInfo
			, nullptr
			, &m_framebuffer );
		checkError( res, "FrameBuffer creation" );
	}

	FrameBuffer::~FrameBuffer()
	{
		if ( m_framebuffer )
		{
			m_device.vkDestroyFramebuffer( m_device, m_framebuffer, nullptr );
		}
	}
}
