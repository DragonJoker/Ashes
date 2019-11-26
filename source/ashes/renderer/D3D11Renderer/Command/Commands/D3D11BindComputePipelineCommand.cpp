/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BindComputePipelineCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Buffer/D3D11BufferView.hpp"
#include "Core/D3D11Device.hpp"
#include "Image/D3D11ImageView.hpp"
#include "Pipeline/D3D11ComputePipeline.hpp"
#include "Pipeline/D3D11PipelineLayout.hpp"
#include "Shader/D3D11ShaderModule.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
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
