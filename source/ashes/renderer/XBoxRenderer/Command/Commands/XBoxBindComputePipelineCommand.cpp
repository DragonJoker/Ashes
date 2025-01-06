/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/XBoxBindComputePipelineCommand.hpp"

#include "Buffer/XBoxBuffer.hpp"
#include "Buffer/XBoxBufferView.hpp"
#include "Core/XBoxDevice.hpp"
#include "Image/XBoxImageView.hpp"
#include "Pipeline/XBoxComputePipeline.hpp"
#include "Pipeline/XBoxPipelineLayout.hpp"
#include "Shader/XBoxShaderModule.hpp"

#include "ashesxbox_api.hpp"

namespace ashes::xbox
{
	BindComputePipelineCommand::BindComputePipelineCommand( VkDevice device
		, VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint )
		: CommandBase{ device }
		, m_pipeline{ pipeline }
		, m_layout{ get( m_pipeline )->getLayout() }
		, m_program{ get( m_pipeline )->getShaderStage() }
		, m_bindingPoint{ bindingPoint }
	{
	}

	void BindComputePipelineCommand::apply( Context const & context )const
	{
		context.context->CSSetShader( m_program.getCSShader()
			, nullptr
			, 0u );
	}

	void BindComputePipelineCommand::remove( Context const & context )const
	{
		context.context->CSSetShader( nullptr, nullptr, 0u );
	}

	CommandPtr BindComputePipelineCommand::clone()const
	{
		return std::make_unique< BindComputePipelineCommand >( *this );
	}
}
