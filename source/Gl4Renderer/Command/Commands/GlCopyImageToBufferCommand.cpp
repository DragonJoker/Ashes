/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCopyImageToBufferCommand.hpp"

#include "Buffer/GlBuffer.hpp"
#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	namespace
	{
		VkImageView createView( VkDevice device
			, VkImage texture
			, VkBufferImageCopy const & copy )
		{
			VkImageType type = get( texture )->getType();
			VkImageViewType viewType;

			if ( type == VK_IMAGE_TYPE_3D )
			{
				viewType = VK_IMAGE_VIEW_TYPE_3D;
			}
			else if ( type == VK_IMAGE_TYPE_2D )
			{
				viewType = VK_IMAGE_VIEW_TYPE_2D;
			}
			else if ( type == VK_IMAGE_TYPE_1D )
			{
				viewType = VK_IMAGE_VIEW_TYPE_1D;
			}

			VkImageViewCreateInfo createInfo{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, nullptr, 0u };
			createInfo.viewType = viewType;
			createInfo.format = get( texture )->getFormat();
			createInfo.subresourceRange.aspectMask = getAspectMask( createInfo.format );
			createInfo.subresourceRange.baseArrayLayer = copy.imageSubresource.baseArrayLayer;
			createInfo.subresourceRange.layerCount = copy.imageSubresource.layerCount;
			createInfo.subresourceRange.baseMipLevel = copy.imageSubresource.mipLevel;
			createInfo.subresourceRange.levelCount = 1u;
			VkImageView result;
			allocate( result
				, nullptr
				, device
				, createInfo );

			return result;
		}
	}

	CopyImageToBufferCommand::CopyImageToBufferCommand( VkDevice device
		, VkBufferImageCopy m_copyInfo
		, VkImage src
		, VkBuffer dst )
		: CommandBase{ device }
		, m_src{ src }
		, m_dst{ dst }
		, m_copyInfo{ std::move( m_copyInfo ) }
		, m_internal{ getInternalFormat( get( m_src )->getFormat() ) }
		, m_format{ getFormat( m_internal ) }
		, m_type{ getType( m_internal ) }
		, m_target{ convert( get( m_src )->getType(), 1u ) }
		, m_view{ createView( m_device, m_src, m_copyInfo ) }
		, m_srcFbo{ get( m_device )->getBlitSrcFbo() }
	{
	}

	CopyImageToBufferCommand::CopyImageToBufferCommand( CopyImageToBufferCommand const & rhs )
		: CommandBase{ rhs.m_device }
		, m_src{ rhs.m_src }
		, m_dst{ rhs.m_dst }
		, m_copyInfo{ rhs.m_copyInfo }
		, m_internal{ rhs.m_internal }
		, m_format{ rhs.m_format }
		, m_type{ rhs.m_type }
		, m_target{ rhs.m_target }
		, m_view{ createView( m_device, m_src, m_copyInfo ) }
		, m_srcFbo{ rhs.m_srcFbo }
	{
	}

	void CopyImageToBufferCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "CopyImageToBufferCommand" );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_PIXEL_PACK
			, get( m_dst )->getInternal() );

		// Setup source FBO
		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, m_srcFbo );
		glLogCall( context
			, glFramebufferTexture2D
			, GL_FRAMEBUFFER
			, GL_ATTACHMENT_POINT_COLOR0
			, GL_TEXTURE_2D
			, get( m_view )->getInternal()
			, 0u );
		glLogCall( context
			, glReadBuffer
			, GL_ATTACHMENT_POINT_COLOR0 );
		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, context->getCurrentFramebuffer() );

		// Read pixels
		glLogCall( context
			, glBindFramebuffer
			, GL_READ_FRAMEBUFFER
			, m_srcFbo );
		glLogCall( context
			, glReadPixels
			, m_copyInfo.imageOffset.x
			, m_copyInfo.imageOffset.y
			, m_copyInfo.imageExtent.width
			, m_copyInfo.imageExtent.height
			, m_format
			, m_type
			, nullptr );
		glLogCall( context
			, glBindFramebuffer
			, GL_READ_FRAMEBUFFER
			, 0u );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_PIXEL_PACK
			, 0u );
	}

	CommandPtr CopyImageToBufferCommand::clone()const
	{
		return std::make_unique< CopyImageToBufferCommand >( *this );
	}
}
