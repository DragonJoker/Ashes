/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBeginSubpassCommand.hpp"

#include "RenderPass/GlFrameBuffer.hpp"
#include "RenderPass/GlRenderPass.hpp"

#include "ashesgl4_api.hpp"

namespace ashes
{
	inline VkAttachmentReference deepCopy( VkAttachmentReference const & rhs )
	{
		return rhs;
	}
}

namespace ashes::gl4
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
			auto attaches = makeVector( subpass.pColorAttachments
				, subpass.colorAttachmentCount );

			for ( auto & fboAttach : get( frameBuffer )->getColourAttaches() )
			{
				drawBuffers.push_back( fboAttach.point + fboAttach.index );
			}

			list.push_back( makeCmd< OpType::eDrawBuffers >( drawBuffers ) );
		}
	}
}
