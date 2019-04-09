/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBindComputePipelineCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Pipeline/GlComputePipeline.hpp"
#include "Pipeline/GlPipelineLayout.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	BindComputePipelineCommand::BindComputePipelineCommand( VkDevice device
		, VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint )
		: CommandBase{ device }
		, m_pipeline{ ( pipeline ) }
		, m_layout{ get( m_pipeline )->getLayout() }
		, m_program{ get( m_pipeline )->getProgram() }
		, m_bindingPoint{ bindingPoint }
	{
	}

	void BindComputePipelineCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "BindComputePipelineCommand" );

		if ( m_program != context->getCurrentProgram() )
		{
			glLogCall( context
				, glUseProgram
				, m_program );
			context->setCurrentProgram( m_program );
		}
	}

	CommandPtr BindComputePipelineCommand::clone()const
	{
		return std::make_unique< BindComputePipelineCommand >( *this );
	}
}
