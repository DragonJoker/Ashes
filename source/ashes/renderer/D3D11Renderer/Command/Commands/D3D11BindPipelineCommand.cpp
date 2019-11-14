/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BindPipelineCommand.hpp"

#include "Core/D3D11Device.hpp"
#include "Pipeline/D3D11Pipeline.hpp"
#include "Pipeline/D3D11PipelineLayout.hpp"
#include "Shader/D3D11ShaderModule.hpp"

#include <array>

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	void apply( ID3D11DeviceContext * context
		, ID3D11InputLayout * state )
	{
		if ( state )
		{
			context->IASetInputLayout( state );
		}
	}

	void apply( ID3D11DeviceContext * context
		, ID3D11RasterizerState * state )
	{
		context->RSSetState( state );
	}

	void apply( ID3D11DeviceContext * context
		, ID3D11BlendState * state
		, float const * blendFactor
		, UINT sampleMask )
	{
		context->OMSetBlendState( state
			, blendFactor
			, sampleMask );
	}

	void apply( ID3D11DeviceContext * context
		, VkPipelineMultisampleStateCreateInfo const & state )
	{
	}

	void apply( ID3D11DeviceContext * context
		, ID3D11DepthStencilState * state
		, UINT stencilRef )
	{
		context->OMSetDepthStencilState( state, stencilRef );
	}

	void apply( ID3D11DeviceContext * context
		, VkPipelineTessellationStateCreateInfo const & state )
	{
	}

	void apply( ID3D11DeviceContext * context
		, std::vector< D3D11_VIEWPORT > const & state )
	{
		context->RSSetViewports( UINT( state.size() ), state.data() );
	}

	void apply( ID3D11DeviceContext * context
		, std::vector< RECT > const & state )
	{
		context->RSSetScissorRects( UINT( state.size() ), state.data() );
	}

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

	void BindPipelineCommand::apply( Context const & context )const
	{
		ashes::d3d11::apply( context.context
			, get( m_pipeline )->getIAState() );
		ashes::d3d11::apply( context.context
			, get( m_pipeline )->getBDState()
			, get( m_pipeline )->getBlendFactor()
			, get( m_pipeline )->getSampleMask() );
		ashes::d3d11::apply( context.context
			, get( m_pipeline )->getRSState() );
		ashes::d3d11::apply( context.context
			, get( m_pipeline )->getDSState()
			, get( m_pipeline )->getStencilRef() );
		//ashes::d3d11::apply( context
		//	, get( m_pipeline )->getMultisampleState() );
		//ashes::d3d11::apply( context
		//	, get( m_pipeline )->getTessellationState() );

		if ( !m_dynamicViewport )
		{
			assert( get( m_pipeline )->hasViewport() );
			ashes::d3d11::apply( context.context
				, get( m_pipeline )->getViewports() );
		}

		if ( !m_dynamicScissor )
		{
			assert( get( m_pipeline )->hasScissor() );
			ashes::d3d11::apply( context.context
				, get( m_pipeline )->getScissors() );
		}

		// Bind program
		for ( auto & stage : get( m_pipeline )->getShaderStages() )
		{
			switch ( stage.getStage() )
			{
			case VK_SHADER_STAGE_VERTEX_BIT:
				context.context->VSSetShader( stage.getVSShader()
					, nullptr
					, 0u );
				break;
			case VK_SHADER_STAGE_GEOMETRY_BIT:
				context.context->GSSetShader( stage.getGSShader()
					, nullptr
					, 0u );
				break;
			case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
				context.context->HSSetShader( stage.getHSShader()
					, nullptr
					, 0u );
				break;
			case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
				context.context->DSSetShader( stage.getDSShader()
					, nullptr
					, 0u );
				break;
			case VK_SHADER_STAGE_FRAGMENT_BIT:
				context.context->PSSetShader( stage.getPSShader()
					, nullptr
					, 0u );
				break;
			case VK_SHADER_STAGE_COMPUTE_BIT:
				context.context->CSSetShader( stage.getCSShader()
					, nullptr
					, 0u );
				break;
			default:
				assert( false && "Unsupported shader stage flag" );
				break;
			}
		}
	}

	void BindPipelineCommand::remove( Context const & context )const
	{
		ashes::d3d11::apply( context.context
			, ( ID3D11InputLayout * )nullptr );
		ashes::d3d11::apply( context.context
			, nullptr
			, get( m_pipeline )->getBlendFactor()
			, get( m_pipeline )->getSampleMask() );
		ashes::d3d11::apply( context.context
			, ( ID3D11RasterizerState * )nullptr );
		ashes::d3d11::apply( context.context
			, ( ID3D11DepthStencilState * )nullptr
			, get( m_pipeline )->getStencilRef() );
		//ashes::d3d11::apply( context
		//	, get( m_pipeline )->getMultisampleState() );
		//ashes::d3d11::apply( context
		//	, get( m_pipeline )->getTessellationState() );

		// Bind program
		for ( auto & stage : get( m_pipeline )->getShaderStages() )
		{
			switch ( stage.getStage() )
			{
			case VK_SHADER_STAGE_VERTEX_BIT:
				context.context->VSSetShader( nullptr, nullptr, 0u );
				break;
			case VK_SHADER_STAGE_GEOMETRY_BIT:
				context.context->GSSetShader( nullptr, nullptr, 0u );
				break;
			case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
				context.context->HSSetShader( nullptr, nullptr, 0u );
				break;
			case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
				context.context->DSSetShader( nullptr, nullptr, 0u );
				break;
			case VK_SHADER_STAGE_FRAGMENT_BIT:
				context.context->PSSetShader( nullptr, nullptr, 0u );
				break;
			case VK_SHADER_STAGE_COMPUTE_BIT:
				context.context->CSSetShader( nullptr, nullptr, 0u );
				break;
			default:
				assert( false && "Unsupported shader stage flag" );
				break;
			}
		}
	}

	CommandPtr BindPipelineCommand::clone()const
	{
		return std::make_unique< BindPipelineCommand >( *this );
	}
}
