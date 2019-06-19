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
		cmd.stack->apply( context, *cmd.state );
	}

	void buildBindPipelineCommand( ContextStateStack & stack
		, VkDevice device
		, VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint
		, CmdList & list )
	{
		glLogCommand( "BindPipelineCommand" );
		GLuint program;

		if ( get( stack.getCurrentFramebuffer() )->hasSwapchainImage() )
		{
			stack.apply( list, get( pipeline )->getBackContextState() );
			program = get( pipeline )->getBackProgram();
		}
		else
		{
			stack.apply( list, get( pipeline )->getRtotContextState() );
			program = get( pipeline )->getRtotProgram();
		}

		if ( stack.getCurrentProgram() != program )
		{
			list.push_back( makeCmd< OpType::eUseProgram >( program ) );
			stack.setCurrentProgram( program );
		}
	}

	void buildUnbindPipelineCommand( ContextStateStack & stack
		, VkDevice device
		, VkPipeline pipeline
		, VkImageView view
		, CmdList & list )
	{
		stack.setCurrentProgram( 0u );
		list.push_back( makeCmd< OpType::eUseProgram >( 0u ) );
	}
}
