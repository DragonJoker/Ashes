/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBindComputePipelineCommand.hpp"

#include "Pipeline/GlPipeline.hpp"

#include "ashesgl3_api.hpp"

namespace ashes::gl3
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
}
