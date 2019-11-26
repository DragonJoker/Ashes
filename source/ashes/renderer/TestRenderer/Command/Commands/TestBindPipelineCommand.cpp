/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestBindPipelineCommand.hpp"

#include "Core/TestDevice.hpp"
#include "Pipeline/TestPipeline.hpp"
#include "Pipeline/TestPipelineLayout.hpp"
#include "Shader/TestShaderModule.hpp"

#include <array>

#include "ashestest_api.hpp"

namespace ashes::test
{
	BindPipelineCommand::BindPipelineCommand( VkDevice device
		, VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint )
		: CommandBase{ device }
		, m_pipeline{ pipeline }
		, m_layout{ get( m_pipeline )->getLayout() }
		, m_bindingPoint{ bindingPoint }
		, m_dynamicLineWidth{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_LINE_WIDTH ) }
		, m_dynamicDepthBias{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_DEPTH_BIAS ) }
		, m_dynamicScissor{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_SCISSOR ) }
		, m_dynamicViewport{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_VIEWPORT ) }
	{
	}

	void BindPipelineCommand::apply()const
	{
	}

	CommandPtr BindPipelineCommand::clone()const
	{
		return std::make_unique< BindPipelineCommand >( *this );
	}
}
