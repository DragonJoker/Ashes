/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11BindPipelineCommand.hpp"

#include "Core/D3D11Device.hpp"
#include "Pipeline/D3D11Pipeline.hpp"
#include "Pipeline/D3D11PipelineLayout.hpp"
#include "Shader/D3D11ShaderModule.hpp"

namespace d3d11_renderer
{
	void apply( ID3D11DeviceContext * context
		, ID3D11InputLayout * state )
	{
		context->IASetInputLayout( state );
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
		, D3D11_VIEWPORT const & state )
	{
		context->RSSetViewports( 1u, &state );
	}

	void apply( ID3D11DeviceContext * context
		, RECT const & state )
	{
		context->RSSetScissorRects( 1u, &state );
	}

	BindPipelineCommand::BindPipelineCommand( Device const & device
		, ashes::Pipeline const & pipeline
		, ashes::PipelineBindPoint bindingPoint )
		: CommandBase{ device }
		, m_pipeline{ static_cast< Pipeline const & > ( pipeline ) }
		, m_layout{ static_cast< PipelineLayout const & > ( m_pipeline.getLayout() ) }
		, m_bindingPoint{ bindingPoint }
		, m_dynamicLineWidth{ m_pipeline.hasDynamicState( ashes::DynamicState::eLineWidth ) }
		, m_dynamicDepthBias{ m_pipeline.hasDynamicState( ashes::DynamicState::eDepthBias ) }
		, m_dynamicScissor{ m_pipeline.hasDynamicState( ashes::DynamicState::eScissor ) }
		, m_dynamicViewport{ m_pipeline.hasDynamicState( ashes::DynamicState::eViewport ) }
	{
	}

	void BindPipelineCommand::apply( Context const & context )const
	{
		d3d11_renderer::apply( context.context
			, m_pipeline.getIAState() );
		d3d11_renderer::apply( context.context
			, m_pipeline.getBDState()
			, m_pipeline.getBlendFactor()
			, m_pipeline.getSampleMask() );
		d3d11_renderer::apply( context.context
			, m_pipeline.getRSState() );
		d3d11_renderer::apply( context.context
			, m_pipeline.getDSState()
			, m_pipeline.getStencilRef() );
		//d3d11_renderer::apply( context
		//	, m_pipeline.getMultisampleState() );
		//d3d11_renderer::apply( context
		//	, m_pipeline.getTessellationState() );

		if ( !m_dynamicViewport )
		{
			assert( m_pipeline.hasViewport() );
			d3d11_renderer::apply( context.context
				, m_pipeline.getViewport() );
		}

		if ( !m_dynamicScissor )
		{
			assert( m_pipeline.hasScissor() );
			d3d11_renderer::apply( context.context
				, m_pipeline.getScissor() );
		}

		// Bind program
		for ( auto & stage : m_pipeline.getShaderStages() )
		{
			switch ( stage.module->getStage() )
			{
			case ashes::ShaderStageFlag::eVertex:
				context.context->VSSetShader( std::static_pointer_cast< ShaderModule >( stage.module )->getVSShader()
					, nullptr
					, 0u );
				break;
			case ashes::ShaderStageFlag::eGeometry:
				context.context->GSSetShader( std::static_pointer_cast< ShaderModule >( stage.module )->getGSShader()
					, nullptr
					, 0u );
				break;
			case ashes::ShaderStageFlag::eTessellationControl:
				context.context->HSSetShader( std::static_pointer_cast< ShaderModule >( stage.module )->getHSShader()
					, nullptr
					, 0u );
				break;
			case ashes::ShaderStageFlag::eTessellationEvaluation:
				context.context->DSSetShader( std::static_pointer_cast< ShaderModule >( stage.module )->getDSShader()
					, nullptr
					, 0u );
				break;
			case ashes::ShaderStageFlag::eFragment:
				context.context->PSSetShader( std::static_pointer_cast< ShaderModule >( stage.module )->getPSShader()
					, nullptr
					, 0u );
				break;
			case ashes::ShaderStageFlag::eCompute:
				context.context->CSSetShader( std::static_pointer_cast< ShaderModule >( stage.module )->getCSShader()
					, nullptr
					, 0u );
				break;
			}
		}
	}

	void BindPipelineCommand::remove( Context const & context )const
	{
		d3d11_renderer::apply( context.context
			, ( ID3D11InputLayout * )nullptr );
		d3d11_renderer::apply( context.context
			, nullptr
			, m_pipeline.getBlendFactor()
			, m_pipeline.getSampleMask() );
		d3d11_renderer::apply( context.context
			, ( ID3D11RasterizerState * )nullptr );
		d3d11_renderer::apply( context.context
			, ( ID3D11DepthStencilState * )nullptr
			, m_pipeline.getStencilRef() );
		//d3d11_renderer::apply( context
		//	, m_pipeline.getMultisampleState() );
		//d3d11_renderer::apply( context
		//	, m_pipeline.getTessellationState() );

		// Bind program
		for ( auto & stage : m_pipeline.getShaderStages() )
		{
			switch ( stage.module->getStage() )
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
