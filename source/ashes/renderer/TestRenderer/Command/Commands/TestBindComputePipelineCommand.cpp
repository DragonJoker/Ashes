/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestBindComputePipelineCommand.hpp"

#include "Buffer/TestBuffer.hpp"
#include "Buffer/TestBufferView.hpp"
#include "Core/TestDevice.hpp"
#include "Image/TestImageView.hpp"
#include "Pipeline/TestComputePipeline.hpp"
#include "Pipeline/TestPipelineLayout.hpp"
#include "Shader/TestShaderModule.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	BindComputePipelineCommand::BindComputePipelineCommand( VkDevice device
		, VkPipeline pipeline
		, VkPipelineBindPoint )
		: CommandBase{ device }
		, m_pipeline{ pipeline }
		, m_layout{ get( m_pipeline )->getLayout() }
	{
	}

	void BindComputePipelineCommand::apply()const
	{
	}

	CommandPtr BindComputePipelineCommand::clone()const
	{
		return std::make_unique< BindComputePipelineCommand >( *this );
	}
}
