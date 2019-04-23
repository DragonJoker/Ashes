/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBeginRenderPassCommand.hpp"

#include "Core/GlDevice.hpp"
#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	void apply( ContextLock const & context
		, CmdClearBack const & cmd )
	{
		glLogCall( context
			, glClear
			, cmd.mask );
	}

	void apply( ContextLock const & context
		, CmdClearBackColour const & cmd )
	{
		glLogCall( context
			, glClearColor
			, cmd.color.float32[0]
			, cmd.color.float32[1]
			, cmd.color.float32[2]
			, cmd.color.float32[3] );
	}

	void apply( ContextLock const & context
		, CmdClearBackDepth const & cmd )
	{
		glLogCall( context
			, glClearDepth
			, cmd.depth );
	}

	void apply( ContextLock const & context
		, CmdClearBackStencil const & cmd )
	{
		glLogCall( context
			, glClearStencil
			, cmd.stencil );
	}

	void apply( ContextLock const & context
		, CmdClearBackDepthStencil const & cmd )
	{
		glLogCall( context
			, glClearDepth
			, cmd.depth );
		glLogCall( context
			, glClearStencil
			, cmd.stencil );
	}

	void buildBeginRenderPassCommand( VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkClearValueArray clearValues
		, VkSubpassContents contents
		, CmdList & list )
	{
		VkClearValueArray rtClearValues;
		VkClearValue dsClearValue;
		assert( clearValues.size() == get( renderPass )->getAttachments().size() );
		auto it = get( renderPass )->getAttachments().begin();

		for ( auto & clearValue : clearValues )
		{
			if ( ashes::isDepthOrStencilFormat( it->format ) )
			{
				dsClearValue = clearValue;
			}
			else
			{
				rtClearValues.push_back( clearValue );
			}

			++it;
		}

		glLogCommand( "BeginRenderPassCommand" );
		list.push_back( makeCmd< OpType::eApplyScissor >( VkRect2D
			{
				{ 0, 0 },
				{ get( frameBuffer )->getWidth(), get( frameBuffer )->getHeight() }
			} ) );

		if ( get( frameBuffer )->isSRGB() )
		{
			list.push_back( makeCmd< OpType::eEnable >( GL_FRAMEBUFFER_SRGB ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eDisable >( GL_FRAMEBUFFER_SRGB ) );
		}
		
		list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
			, get( frameBuffer )->getInternal() ) );

		uint32_t clearIndex = 0u;
		auto & attaches = get( frameBuffer )->getAllAttaches();
		UInt32Array drawBuffers;

		if ( get( frameBuffer )->getInternal() )
		{
			auto & attachments = get( frameBuffer )->getAttachments();

			for ( auto & attach : get( renderPass )->getColourAttaches() )
			{
				auto fboAttach = attachments[attach.index];
				auto fboView = get( fboAttach );
				auto fboImage = get( fboView->getImage() );

				if ( fboImage->hasInternal() )
				{
					drawBuffers.push_back( getAttachmentPoint( attach.attach.get().format ) + attach.index );
				}
				else if ( attaches.size() == 1 )
				{
					drawBuffers.push_back( GL_ATTACHMENT_POINT_BACK );
				}
			}

			list.push_back( makeCmd< OpType::eDrawBuffers >( std::move( drawBuffers ) ) );

			for ( auto viewIndex = 0u; viewIndex < attaches.size(); ++viewIndex )
			{
				auto & attach = attaches[viewIndex];
				auto & attachDesc = get( renderPass )->getAttachments()[viewIndex];

				if ( attachDesc.loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR )
				{
					if ( getAspectMask( attachDesc.format ) == VK_IMAGE_ASPECT_COLOR_BIT )
					{
						list.push_back( makeCmd< OpType::eClearColour >( rtClearValues[clearIndex].color
								, clearIndex) );
						++clearIndex;
					}
					else
					{
						if ( isDepthStencilFormat( attachDesc.format ) )
						{
							list.push_back( makeCmd< OpType::eClearDepthStencil >( dsClearValue.depthStencil ) );
						}
						else if ( isDepthFormat( attachDesc.format ) )
						{
							list.push_back( makeCmd< OpType::eClearDepth >( dsClearValue.depthStencil.depth ) );
						}
						else if ( isStencilFormat( attachDesc.format ) )
						{
							list.push_back( makeCmd< OpType::eClearStencil >( int32_t( dsClearValue.depthStencil.stencil ) ) );
						}
					}
				}
			}
		}
		else
		{
			GLbitfield mask{ 0u };

			for ( auto viewIndex = 0u; viewIndex < attaches.size(); ++viewIndex )
			{
				auto & attach = attaches[viewIndex];
				auto & attachDesc = get( renderPass )->getAttachments()[viewIndex];

				if ( attachDesc.loadOp == VK_ATTACHMENT_LOAD_OP_CLEAR )
				{
					if ( getAspectMask( attachDesc.format ) == VK_IMAGE_ASPECT_COLOR_BIT )
					{
						list.push_back( makeCmd< OpType::eClearBackColour >( rtClearValues[clearIndex].color
							, clearIndex ) );
						mask |= GL_COLOR_BUFFER_BIT;
						++clearIndex;
					}
					else
					{
						if ( isDepthStencilFormat( attachDesc.format ) )
						{
							list.push_back( makeCmd< OpType::eClearBackDepthStencil >( dsClearValue.depthStencil.depth
								, int32_t( dsClearValue.depthStencil.stencil ) ) );
							mask |= GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
						}
						else if ( isDepthFormat( attachDesc.format ) )
						{
							list.push_back( makeCmd< OpType::eClearBackDepth >( dsClearValue.depthStencil.depth ) );
							mask |= GL_DEPTH_BUFFER_BIT;
						}
						else if ( isStencilFormat( attachDesc.format ) )
						{
							list.push_back( makeCmd< OpType::eClearBackStencil >( int32_t( dsClearValue.depthStencil.stencil ) ) );
							mask |= GL_STENCIL_BUFFER_BIT;
						}
					}
				}
			}

			list.push_back( makeCmd< OpType::eClearBack >( mask ) );
		}
	}
}
