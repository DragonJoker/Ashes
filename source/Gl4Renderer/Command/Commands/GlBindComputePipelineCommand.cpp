/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBindComputePipelineCommand.hpp"

#include "Pipeline/GlComputePipeline.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	void buildBindComputePipelineCommand( VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint
		, CmdList & list )
	{
		glLogCommand( "BindComputePipelineCommand" );
		list.push_back( makeCmd< OpType::eUseProgram >( get( pipeline )->getProgram() ) );
	}
}
