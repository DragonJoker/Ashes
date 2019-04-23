/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBindPipelineCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Pipeline/GlPipeline.hpp"
#include "Pipeline/GlPipelineLayout.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	void apply( ContextLock const & context
		, CmdBindContextState const & cmd )
	{
		context->apply( context
			, *get( cmd.device )
			, *cmd.state );
	}

	void buildBindPipelineCommand( VkDevice device
		, VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint
		, bool isRtot
		, CmdList & list )
	{
		glLogCommand( "BindPipelineCommand" );

		if ( isRtot )
		{
			list.push_back( makeCmd< OpType::eBindContextState >( device
				, &get( pipeline )->getRtotContextState() ) );
			list.push_back( makeCmd< OpType::eUseProgram >( get( pipeline )->getRtotProgram() ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eBindContextState >( device
				, &get( pipeline )->getBackContextState() ) );
			list.push_back( makeCmd< OpType::eUseProgram >( get( pipeline )->getBackProgram() ) );
		}
	}

	void buildUnbindPipelineCommand( VkDevice device
		, VkPipeline pipeline
		, VkImageView view
		, CmdList & list )
	{
		//if ( view != VK_NULL_HANDLE )
		//{
		//	list.push_back( makeCmd< OpType::eBindContextState >( device
		//		, &get( device )->getRtocContextState() ) );
		//	list.push_back( makeCmd< OpType::eUseProgram >( get( device )->getRtocProgram() ) );
		//	list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
		//		, get( device )->getBlitDstFbo() ) );
		//	list.push_back( makeCmd< OpType::eFramebufferTexture2D >( GL_FRAMEBUFFER
		//		, GL_ATTACHMENT_POINT_COLOR0
		//		, GL_TEXTURE_2D
		//		, get( view )->getInternal()
		//		, 0u ) );
		//	list.push_back( makeCmd< OpType::eBindVextexArray >( get( device )->getRtocVao() ) );
		//	list.push_back( makeCmd< OpType::eActiveTexture >( 0u ) );
		//	list.push_back( makeCmd< OpType::eBindTexture >( GL_TEXTURE_2D
		//		, get( view )->getInternal() ) );
		//	list.push_back( makeCmd< OpType::eBindFramebuffer >( GL_FRAMEBUFFER
		//		, 0u ) );
		//}

		list.push_back( makeCmd< OpType::eUseProgram >( 0u ) );
	}
}
