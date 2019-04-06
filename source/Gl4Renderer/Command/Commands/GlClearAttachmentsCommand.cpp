/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearAttachmentsCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImageView.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	namespace
	{
		void doClear( ContextLock const & context
			, VkClearAttachment const & clearAttach )
		{
			if ( ashes::checkFlag( clearAttach.aspectMask, VK_IMAGE_ASPECT_COLOR_BIT ) )
			{
				auto & colour = clearAttach.clearValue.color;
				glLogCall( context
					, glClearBufferfv
					, GL_CLEAR_TARGET_COLOR
					, clearAttach.colorAttachment
					, colour.float32 );
			}
			else
			{
				auto & depthStencil = clearAttach.clearValue.depthStencil;
				auto stencil = GLint( depthStencil.stencil );

				if ( ashes::checkFlag( clearAttach.aspectMask, VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT ) )
				{
					glLogCall( context
						, glClearBufferfi
						, GL_CLEAR_TARGET_DEPTH_STENCIL
						, 0u
						, depthStencil.depth
						, stencil );
				}
				else if ( ashes::checkFlag( clearAttach.aspectMask, VK_IMAGE_ASPECT_DEPTH_BIT ) )
				{
					glLogCall( context
						, glClearBufferfv
						, GL_CLEAR_TARGET_DEPTH
						, 0u
						, &depthStencil.depth );
				}
				else if ( ashes::checkFlag( clearAttach.aspectMask, VK_IMAGE_ASPECT_STENCIL_BIT ) )
				{
					glLogCall( context
						, glClearBufferiv
						, GL_CLEAR_TARGET_STENCIL
						, 0u
						, &stencil );
				}
			}
		}

	}

	ClearAttachmentsCommand::ClearAttachmentsCommand( VkDevice device
		, VkClearAttachmentArray clearAttaches
		, VkClearRectArray clearRects )
		: CommandBase{ device }
		, m_clearAttaches{ clearAttaches }
		, m_clearRects{ clearRects }
	{
	}

	void ClearAttachmentsCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "ClearAttachmentsCommand" );
		auto scissor = get( m_device )->getCurrentScissor();

		for ( auto & clearAttach : m_clearAttaches )
		{
			for ( auto & rect : m_clearRects )
			{
				glLogCall( context
					, glScissor
					, rect.rect.offset.x
					, rect.rect.offset.x
					, rect.rect.extent.width
					, rect.rect.extent.height );
				doClear( context, clearAttach );
			}
		}

		glLogCall( context
			, glScissor
			, scissor.offset.x
			, scissor.offset.y
			, scissor.extent.width
			, scissor.extent.height );
	}

	CommandPtr ClearAttachmentsCommand::clone()const
	{
		return std::make_unique< ClearAttachmentsCommand >( *this );
	}
}
