/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBindComputePipelineCommand.hpp"

#include "Pipeline/GlPipeline.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	void buildBindComputePipelineCommand( ContextStateStack & stack
		, VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint
		, CmdList & list )
	{
		glLogCommand( "BindComputePipelineCommand" );

		if ( stack.getCurrentProgram() != get( pipeline )->getCompProgram() )
		{
			list.push_back( makeCmd< OpType::eUseProgram >( get( pipeline )->getCompProgram() ) );
			stack.setCurrentProgram( get( pipeline )->getCompProgram() );
		}
	}

	void buildUnbindComputePipelineCommand( ContextStateStack & stack
		, CmdList & list )
	{
		stack.setCurrentProgram( 0u );
		list.push_back( makeCmd< OpType::eUseProgram >( 0u ) );
	}
}
