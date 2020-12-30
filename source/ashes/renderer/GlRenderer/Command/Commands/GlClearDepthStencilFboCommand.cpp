/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearDepthStencilFboCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImageView.hpp"
#include "Image/GlImage.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void buildClearDepthStencilFboCommand( VkDevice device
		, ContextStateStack const & stack
		, VkImage image
		, VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, ArrayView< VkImageSubresourceRange const > ranges
		, CmdList & list )
	{
		glLogCommand( list, "ClearDepthStencilFboCommand" );
		auto & glimage = *get( image );
		auto target = GL_TEXTURE_2D;

		if ( glimage.getSamples() > VK_SAMPLE_COUNT_1_BIT )
		{
			target = GL_TEXTURE_2D_MULTISAMPLE;
		}

		bool hadFbo = stack.hasCurrentFramebuffer();
		list.push_back( makeCmd< OpType::eBindDstFramebuffer >( GL_FRAMEBUFFER ) );
		auto point = getAttachmentPoint( glimage.getFormat() );

		for ( auto range : ranges )
		{
			if ( range.levelCount == RemainingArrayLayers )
			{
				range.levelCount = ashes::getMaxMipCount( get( image )->getDimensions() );
			}

			if ( range.layerCount == RemainingArrayLayers )
			{
				range.layerCount = get( device )->getLimits().maxImageArrayLayers;
			}

			for ( auto level = range.baseMipLevel; level < range.baseMipLevel + range.levelCount; ++level )
			{
				if ( get( image )->getArrayLayers() > 1u )
				{
					for ( auto layer = range.baseArrayLayer; layer < range.baseArrayLayer + range.layerCount; ++layer )
					{
						list.push_back( makeCmd< OpType::eFramebufferTextureLayer >( GL_FRAMEBUFFER
							, point
							, get( image )->getInternal()
							, level
							, layer ) );
						auto format = get( image )->getFormat();

						if ( isDepthStencilFormat( format ) )
						{
							list.push_back( makeCmd< OpType::eClearDepthStencil >( value ) );
						}
						else if ( isDepthFormat( format ) )
						{
							list.push_back( makeCmd< OpType::eClearDepth >( value.depth ) );
						}
						else if ( isStencilFormat( format ) )
						{
							list.push_back( makeCmd< OpType::eClearStencil >( int32_t( value.stencil ) ) );
						}
					}
				}
				else
				{
					list.push_back( makeCmd< OpType::eFramebufferTexture2D >( GL_FRAMEBUFFER
						, point
						, target
						, get( image )->getInternal()
						, level ) );
					auto format = get( image )->getFormat();

					if ( isDepthStencilFormat( format ) )
					{
						list.push_back( makeCmd< OpType::eClearDepthStencil >( value ) );
					}
					else if ( isDepthFormat( format ) )
					{
						list.push_back( makeCmd< OpType::eClearDepth >( value.depth ) );
					}
					else if ( isStencilFormat( format ) )
					{
						list.push_back( makeCmd< OpType::eClearStencil >( int32_t( value.stencil ) ) );
					}
				}
			}
		}

		if ( hadFbo )
		{
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, stack.getCurrentFramebuffer() ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
				, nullptr ) );
		}
	}
}
