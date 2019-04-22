/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BindPipelineCommand.hpp"

#include "Core/D3D11Device.hpp"
#include "Pipeline/D3D11Pipeline.hpp"
#include "Pipeline/D3D11PipelineLayout.hpp"
#include "Shader/D3D11ShaderModule.hpp"

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
		, std::array< FLOAT, 4u > const & blendFactor
		, UINT sampleMask )
	{
		context->OMSetBlendState( state
			, blendFactor.data()
			, sampleMask );
	}

	void apply( ID3D11DeviceContext * context
		, ashes::MultisampleState const & state )
	{
	}

	void apply( ID3D11DeviceContext * context
		, ID3D11DepthStencilState * state
		, UINT stencilRef )
	{
		context->OMSetDepthStencilState( state, stencilRef );
	}

	void apply( ID3D11DeviceContext * context
		, ashes::TessellationState const & state )
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

	BindPipelineCommand::BindPipelineCommand( Device const & device
		, ashes::Pipeline const & pipeline
		, ashes::PipelineBindPoint bindingPoint )
		: CommandBase{ device }
		, m_pipeline{ static_cast< Pipeline const & > ( pipeline ) }
		, m_layout{ static_cast< PipelineLayout const & > ( m_pipeline.getLayout() ) }
		, m_bindingPoint{ bindingPoint }
		, m_dynamicLineWidth{ m_pipeline.hasDynamicStateEnable( ashes::DynamicStateEnable::eLineWidth ) }
		, m_dynamicDepthBias{ m_pipeline.hasDynamicStateEnable( ashes::DynamicStateEnable::eDepthBias ) }
		, m_dynamicScissor{ m_pipeline.hasDynamicStateEnable( ashes::DynamicStateEnable::eScissor ) }
		, m_dynamicViewport{ m_pipeline.hasDynamicStateEnable( ashes::DynamicStateEnable::eViewport ) }
	{
	}

	void BindPipelineCommand::apply( Context const & context )const
	{
		ashes::d3d11::apply( context.context
			, m_pipeline.getIAState() );
		ashes::d3d11::apply( context.context
			, m_pipeline.getBDState()
			, m_pipeline.getBlendFactor()
			, m_pipeline.getSampleMask() );
		ashes::d3d11::apply( context.context
			, m_pipeline.getRSState() );
		ashes::d3d11::apply( context.context
			, m_pipeline.getDSState()
			, m_pipeline.getStencilRef() );
		//ashes::d3d11::apply( context
		//	, m_pipeline.getMultisampleState() );
		//ashes::d3d11::apply( context
		//	, m_pipeline.getTessellationState() );

		if ( !m_dynamicViewport )
		{
			assert( m_pipeline.hasViewport() );
			ashes::d3d11::apply( context.context
				, m_pipeline.getViewports() );
		}

		if ( !m_dynamicScissor )
		{
			assert( m_pipeline.hasScissor() );
			ashes::d3d11::apply( context.context
				, m_pipeline.getScissors() );
		}

		// Bind program
		for ( auto & stage : m_pipeline.getShaderStages() )
		{
			switch ( stage.getStage() )
			{
			case ashes::ShaderStageFlag::eVertex:
				context.context->VSSetShader( stage.getVSShader()
					, nullptr
					, 0u );
				break;
			case ashes::ShaderStageFlag::eGeometry:
				context.context->GSSetShader( stage.getGSShader()
					, nullptr
					, 0u );
				break;
			case ashes::ShaderStageFlag::eTessellationControl:
				context.context->HSSetShader( stage.getHSShader()
					, nullptr
					, 0u );
				break;
			case ashes::ShaderStageFlag::eTessellationEvaluation:
				context.context->DSSetShader( stage.getDSShader()
					, nullptr
					, 0u );
				break;
			case ashes::ShaderStageFlag::eFragment:
				context.context->PSSetShader( stage.getPSShader()
					, nullptr
					, 0u );
				break;
			case ashes::ShaderStageFlag::eCompute:
				context.context->CSSetShader( stage.getCSShader()
					, nullptr
					, 0u );
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
			, m_pipeline.getBlendFactor()
			, m_pipeline.getSampleMask() );
		ashes::d3d11::apply( context.context
			, ( ID3D11RasterizerState * )nullptr );
		ashes::d3d11::apply( context.context
			, ( ID3D11DepthStencilState * )nullptr
			, m_pipeline.getStencilRef() );
		//ashes::d3d11::apply( context
		//	, m_pipeline.getMultisampleState() );
		//ashes::d3d11::apply( context
		//	, m_pipeline.getTessellationState() );

		// Bind program
		for ( auto & stage : m_pipeline.getShaderStages() )
		{
			switch ( stage.getStage() )
			{
			case ashes::ShaderStageFlag::eVertex:
				context.context->VSSetShader( nullptr, nullptr, 0u );
				break;
			case ashes::ShaderStageFlag::eGeometry:
				context.context->GSSetShader( nullptr, nullptr, 0u );
				break;
			case ashes::ShaderStageFlag::eTessellationControl:
				context.context->HSSetShader( nullptr, nullptr, 0u );
				break;
			case ashes::ShaderStageFlag::eTessellationEvaluation:
				context.context->DSSetShader( nullptr, nullptr, 0u );
				break;
			case ashes::ShaderStageFlag::eFragment:
				context.context->PSSetShader( nullptr, nullptr, 0u );
				break;
			case ashes::ShaderStageFlag::eCompute:
				context.context->CSSetShader( nullptr, nullptr, 0u );
				break;
			}
		}
	}

	CommandPtr BindPipelineCommand::clone()const
	{
		return std::make_unique< BindPipelineCommand >( *this );
	}
}
