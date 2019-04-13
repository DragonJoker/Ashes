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
		, CmdList & list
		, bool isRtot )
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
}
