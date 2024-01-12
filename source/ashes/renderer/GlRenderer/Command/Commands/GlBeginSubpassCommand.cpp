/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBeginSubpassCommand.hpp"

#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void buildBeginSubpassCommand( ContextStateStack & stack
		, [[maybe_unused]] VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkSubpassDescription const & subpass
		, CmdList & list )
	{
		glLogCommand( list, "BeginSubpassCommand" );

		if ( get( frameBuffer )->getInternal() != GL_INVALID_INDEX )
		{
			auto references = ashes::makeArrayView( subpass.pColorAttachments
				, subpass.colorAttachmentCount );
			uint32_t index = 0u;

			for ( auto & reference : references )
			{
				auto attach = get( frameBuffer )->getAttachment( reference );

				if ( attach.point )
				{
					attach.bindIndex( 0u
						, GL_FRAMEBUFFER
						, ( attach.isDepthOrStencil()
							? 0u
							: index++ )
						, list );
				}
			}

			list.push_back( makeCmd< OpType::eDrawBuffers >( get( frameBuffer )->getDrawBuffers( references ) ) );
		}

		stack.applySRGBStatus( list, get( frameBuffer )->isSRGB() );
	}
}
