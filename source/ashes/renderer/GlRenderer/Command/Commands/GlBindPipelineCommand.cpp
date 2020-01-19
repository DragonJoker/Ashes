/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBindPipelineCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Pipeline/GlPipeline.hpp"
#include "Pipeline/GlPipelineLayout.hpp"

#include "ashesgl_api.hpp"

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl
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
		, CmdList & list
		, PreExecuteActions & preExecuteActions )
	{
		glLogCommand( list, "BindPipelineCommand" );
		GLuint program;
		auto glpipeline = get( pipeline );

		if ( !stack.hasCurrentFramebuffer() )
		{
			// Can happen in case of secondary command buffers
			stack.apply( list, glpipeline->getRtotContextState() );
			program = glpipeline->getRtotProgram();

			if ( !glpipeline->getViewports().empty() )
			{
				stack.setRenderArea( { uint32_t( glpipeline->getViewports().begin()->width )
					, uint32_t( glpipeline->getViewports().begin()->height ) } );
			}
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
			if ( isGl4( device ) )
			{
				list.push_back( makeCmd< OpType::eUseProgramPipeline >( program ) );
			}
			else
			{
				list.push_back( makeCmd< OpType::eUseProgram >( program ) );
			}

			stack.setCurrentProgram( program );
		}

		if ( !glpipeline->getViewports().empty() )
		{
			stack.apply( list, preExecuteActions, 0u, glpipeline->getViewports(), false );
		}

		if ( !glpipeline->getScissors().empty() )
		{
			stack.apply( list, preExecuteActions, 0u, glpipeline->getScissors(), false );
		}
	}

	void buildUnbindPipelineCommand( ContextStateStack & stack
		, VkDevice device
		, CmdList & list )
	{
		stack.setCurrentProgram( 0u );

		if ( isGl4( device ) )
		{
			list.push_back( makeCmd< OpType::eUseProgramPipeline >( 0u ) );
		}
		else
		{
			list.push_back( makeCmd< OpType::eUseProgram >( 0u ) );
		}
	}
}
