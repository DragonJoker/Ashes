#include "Pipeline/D3D11Pipeline.hpp"

#include "Core/D3D11Device.hpp"
#include "Shader/D3D11ShaderModule.hpp"

#include <Buffer/PushConstantsBuffer.hpp>
#include <Pipeline/SpecialisationInfo.hpp>

namespace d3d11_renderer
{
	Pipeline::Pipeline( Device const & device
		, ashes::PipelineLayout const & layout
		, ashes::GraphicsPipelineCreateInfo && createInfo )
		: ashes::Pipeline{ device
			, layout
			, std::move( createInfo ) }
		, m_scissor{ ( bool )m_createInfo.scissor ? std::optional< RECT >( makeScissor( *m_createInfo.scissor ) ) : std::nullopt }
		, m_viewport{ ( bool )m_createInfo.viewport ? std::optional< D3D11_VIEWPORT >( makeViewport( *m_createInfo.viewport ) ) : std::nullopt }
	{
		doCreateBlendState( device );
		doCreateRasterizerState( device );
		doCreateDepthStencilState( device );
		auto vtxShader = doCompileProgram( device );
		doCreateInputLayout( device, vtxShader );
	}

	void Pipeline::doCreateBlendState( Device const & device )
	{
		auto d3ddevice = device.getDevice();
		auto blendDesc = convert( m_createInfo.colourBlendState );
		HRESULT hr = d3ddevice->CreateBlendState( &blendDesc, &m_bdState );

		if ( !dxCheckError( hr, "CreateBlendState" ) )
		{
			throw std::runtime_error( "CreateBlendState() failed" );
		}

		dxDebugName( m_bdState, PipelineBlendState );
	}

	void Pipeline::doCreateRasterizerState( Device const & device )
	{
		auto d3ddevice = device.getDevice();
		auto rasterizerDesc = convert( m_createInfo.rasterisationState
			, m_createInfo.multisampleState );
		auto hr = d3ddevice->CreateRasterizerState( &rasterizerDesc, &m_rsState );

		if ( !dxCheckError( hr, "CreateRasterizerState" ) )
		{
			throw std::runtime_error( "CreateRasterizerState() failed" );
		}

		dxDebugName( m_rsState, PipelineRasterizerState );
	}

	void Pipeline::doCreateDepthStencilState( Device const & device )
	{
		if ( m_createInfo.depthStencilState )
		{
			auto d3ddevice = device.getDevice();
			auto depthStencilDesc = convert( m_createInfo.depthStencilState.value() );
			auto hr = d3ddevice->CreateDepthStencilState( &depthStencilDesc, &m_dsState );

			if ( !dxCheckError( hr, "CreateDepthStencilState" ) )
			{
				throw std::runtime_error( "CreateDepthStencilState() failed" );
			}

			dxDebugName( m_dsState, PipelineDepthStencilState );
		}
	}

	ShaderModule * Pipeline::doCompileProgram( Device const & device )
	{
		for ( auto & stage : m_createInfo.stages )
		{
			if ( stage.specialisationInfo )
			{
				m_constantsPcbs.push_back( convert( device
					, ~( 0u )
					, stage.module->getStage()
					, *stage.specialisationInfo ) );
			}
		}

		ShaderModule * result{ nullptr };

		for ( auto & state : m_createInfo.stages )
		{
			auto module = std::static_pointer_cast< ShaderModule >( state.module );
			module->compile( state );

			if ( module->getStage() == ashes::ShaderStageFlag::eVertex )
			{
				result = module.get();
			}
		}

		return result;
	}

	void Pipeline::doCreateInputLayout( Device const & device
		, ShaderModule * vtxShader )
	{
		if ( vtxShader )
		{
			auto d3ddevice = device.getDevice();
			std::list< std::string > strings;
			auto inputDesc = convert( m_createInfo.vertexInputState
				, strings );
			auto hr = d3ddevice->CreateInputLayout( inputDesc.data()
				, UINT( inputDesc.size() )
				, vtxShader->getCompiled()->GetBufferPointer()
				, vtxShader->getCompiled()->GetBufferSize()
				, &m_iaState );

			if ( !dxCheckError( hr, "CreateInputLayout" ) )
			{
				throw std::runtime_error( "CreateInputLayout() failed" );
			}

			dxDebugName( m_iaState, PipelineInputLayout );
		}
	}

	Pipeline::~Pipeline()
	{
		safeRelease( m_bdState );
		safeRelease( m_rsState );
		safeRelease( m_iaState );
		safeRelease( m_dsState );
	}
}
