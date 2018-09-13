/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBindComputePipelineCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Pipeline/GlComputePipeline.hpp"
#include "Pipeline/GlPipelineLayout.hpp"

namespace gl_renderer
{
	BindComputePipelineCommand::BindComputePipelineCommand( Device const & device
		, ashes::ComputePipeline const & pipeline
		, ashes::PipelineBindPoint bindingPoint )
		: CommandBase{ device }
		, m_pipeline{ static_cast< ComputePipeline const & > ( pipeline ) }
		, m_layout{ static_cast< PipelineLayout const & > ( m_pipeline.getLayout() ) }
		, m_program{ m_pipeline.getProgram() }
		, m_bindingPoint{ bindingPoint }
	{
	}

	void BindComputePipelineCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "BindComputePipelineCommand" );
		auto & save = m_device.getCurrentProgram();

		if ( m_program != save )
		{
			glLogCall( context
				, glUseProgram
				, m_program );
			save = m_program;
		}
	}

	CommandPtr BindComputePipelineCommand::clone()const
	{
		return std::make_unique< BindComputePipelineCommand >( *this );
	}
}
