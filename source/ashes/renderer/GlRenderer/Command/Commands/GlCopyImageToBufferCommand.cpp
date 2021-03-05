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

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void buildCopyImageToBufferCommand( ContextStateStack & stack
		, VkDevice device
		, VkBufferImageCopy copyInfo
		, VkImage src
		, VkBuffer dst
		, CmdList & list
		, VkImageViewArray & views )
	{
		glLogCommand( list, "CopyImageToBufferCommand" );
		auto internal = getInternalFormat( get( src )->getFormat() );
		auto format = getFormat( internal );
		auto type = getType( internal );

		VkImageView srcView{ VK_NULL_HANDLE };
		FboAttachment srcAttach{ initialiseAttachment( device, copyInfo.imageSubresource, src, 0u, srcView ) };

		if ( srcView != VK_NULL_HANDLE )
		{
			views.push_back( srcView );
			copyInfo.imageSubresource.mipLevel = 0u;
		}

		// Setup source FBO
		list.push_back( makeCmd< OpType::eBindSrcFramebuffer >( GL_FRAMEBUFFER ) );
		srcAttach.bind( copyInfo.imageSubresource, 0u, list );
		list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
			, nullptr ) );

		// Read pixels
		list.push_back( makeCmd< OpType::eBindBuffer >( GL_BUFFER_TARGET_PIXEL_PACK
			, get( dst )->getInternal() ) );
		list.push_back( makeCmd< OpType::eBindSrcFramebuffer >( GL_READ_FRAMEBUFFER ) );

		if ( srcAttach.point != GL_ATTACHMENT_POINT_DEPTH_STENCIL
			&& srcAttach.point != GL_ATTACHMENT_POINT_DEPTH
			&& srcAttach.point != GL_ATTACHMENT_POINT_STENCIL )
		{
			list.push_back( makeCmd< OpType::eReadBuffer >( uint32_t( srcAttach.point ) ) );
		}

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
