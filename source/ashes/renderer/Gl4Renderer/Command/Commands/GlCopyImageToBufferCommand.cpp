/*
This file belongs to Ashes.
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
			VkImageViewType viewType{};

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

	void apply( ContextLock const & context
		, CmdReadPixels const & cmd )
	{
		glLogCall( context
			, glReadPixels
			, cmd.x
			, cmd.y
			, cmd.width
			, cmd.height
			, cmd.format
			, cmd.type
			, nullptr );
	}

	void buildCopyImageToBufferCommand( ContextStateStack & stack
		, VkDevice device
		, VkBufferImageCopy copyInfo
		, VkImage src
		, VkBuffer dst
		, CmdList & list )
	{
		glLogCommand( "CopyImageToBufferCommand" );
		auto internal = getInternalFormat( get( src )->getFormat() );
		auto format = getFormat( internal );
		auto type = getType( internal );
		auto view = createView( device, src, copyInfo );

		list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK
			, get( dst )->getInternal() ) );

		// Setup source FBO
		list.push_back( makeCmd< OpType::eInitFramebuffer >( &get( get( device )->getBlitSrcFbo() )->getInternal() ) );
		list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
			, get( device )->getBlitSrcFbo() ) );
		list.push_back( makeCmd< OpType::eFramebufferTexture2D >( GL_FRAMEBUFFER
			, GL_ATTACHMENT_POINT_COLOR0
			, GL_TEXTURE_2D
			, get( view )->getInternal()
			, 0u ) );
		list.push_back( makeCmd< OpType::eReadBuffer >( GL_ATTACHMENT_POINT_COLOR0 ) );
		list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
			, nullptr ) );

		// Read pixels
		list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_READ_FRAMEBUFFER
			, get( device )->getBlitSrcFbo() ) );
		list.push_back( makeCmd< OpType::eReadPixels >( copyInfo.imageOffset.x
			, copyInfo.imageOffset.y
			, copyInfo.imageExtent.width
			, copyInfo.imageExtent.height
			, format
			, type ) );
		list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_READ_FRAMEBUFFER
			, nullptr ) );

		list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK
			, 0u ) );

		if ( stack.hasCurrentFramebuffer() )
		{
			stack.setCurrentFramebuffer( VK_NULL_HANDLE );
		}
	}
}
