/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBeginSubpassCommand.hpp"

#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
{
	void buildBeginSubpassCommand( VkRenderPass renderPass
		, VkFramebuffer frameBuffer
		, VkSubpassDescription subpass
		, CmdList & list )
	{
		glLogCommand( "BeginSubpassCommand" );

		if ( get( frameBuffer )->getInternal() != GL_INVALID_INDEX )
		{
			UInt32Array drawBuffers;
			auto references = ashes::makeArrayView( subpass.pColorAttachments
				, subpass.colorAttachmentCount );

			for ( auto & reference : references )
			{
				auto & attach = get( renderPass )->getAttachment( reference );
				drawBuffers.push_back( getAttachmentPoint( attach.format ) + reference.attachment );
			}

			list.push_back( makeCmd< OpType::eDrawBuffers >( drawBuffers ) );
		}
	}
}
