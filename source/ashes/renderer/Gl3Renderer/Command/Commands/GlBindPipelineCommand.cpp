/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBindPipelineCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Pipeline/GlPipeline.hpp"
#include "Pipeline/GlPipelineLayout.hpp"

#include "ashesgl3_api.hpp"

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl3
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
		auto glpipeline = get( pipeline );

		if ( !stack.hasCurrentFramebuffer() )
		{
			// Can happen in case of secondary command buffers
			stack.apply( list, glpipeline->getRtotContextState() );
			program = glpipeline->getRtotProgram();
		}
		else if ( !get( stack.getCurrentFramebuffer() )->hasSwapchainImage() )
		{
			stack.apply( list, glpipeline->getRtotContextState() );
			program = glpipeline->getRtotProgram();
		}
		else
		{
			stack.apply( list, glpipeline->getBackContextState() );
			program = glpipeline->getBackProgram();
		}

		if ( stack.getCurrentProgram() != program )
		{
			list.push_back( makeCmd< OpType::eUseProgram >( program ) );
			stack.setCurrentProgram( program );
		}

		if ( !glpipeline->getViewports().empty() )
		{
			stack.apply( list, glpipeline->getViewports(), false );
		}

		if ( !glpipeline->getScissors().empty() )
		{
			stack.apply( list, glpipeline->getScissors(), false );
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
