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
	BindPipelineCommand::BindPipelineCommand( VkDevice device
		, VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint )
		: CommandBase{ device }
		, m_pipeline{ pipeline }
		, m_layout{ get( m_pipeline )->getLayout() }
		, m_program{ get( m_pipeline )->getProgram() }
		, m_bindingPoint{ bindingPoint }
		, m_dynamicLineWidth{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_LINE_WIDTH ) }
		, m_dynamicDepthBias{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_DEPTH_BIAS ) }
		, m_dynamicScissor{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_SCISSOR ) }
		, m_dynamicViewport{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_VIEWPORT ) }
	{
	}

	void BindPipelineCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "BindPipelineCommand" );
		auto pipeline = get( m_pipeline );
		context->apply( context
			, *get( m_device )
			, pipeline->getContextState() );

		if ( m_program != context->getCurrentProgram() )
		{
			glLogCall( context
				, glUseProgram
				, m_program );
			context->setCurrentProgram( m_program );
		}
	}

	CommandPtr BindPipelineCommand::clone()const
	{
		return std::make_unique< BindPipelineCommand >( *this );
	}
}
