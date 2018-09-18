#include "Pipeline/D3D11Pipeline.hpp"

#include "Buffer/D3D11UniformBuffer.hpp"
#include "Core/D3D11Device.hpp"
#include "Pipeline/D3D11VertexInputState.hpp"
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
		doCompileProgram( device );
		doCreateInputLayout( device );
	}

	PushConstantsBuffer Pipeline::findPushConstantBuffer( PushConstantsDesc const & pushConstants )const
	{
		// Try to find a PCB that has the same flags, and the same size as the push constants.
		auto it = std::find_if( m_constantsPcbs.begin()
			, m_constantsPcbs.end()
			, [&pushConstants]( PushConstantsBuffer const & lookup )
			{
				return lookup.data.stageFlags == pushConstants.stageFlags
					&& lookup.data.size == pushConstants.offset + pushConstants.size;
			} );

		if ( it == m_constantsPcbs.end() )
		{
			// Try a PCB that has the same flags, but is larger than the push constants.
			it = std::find_if( m_constantsPcbs.begin()
				, m_constantsPcbs.end()
				, [&pushConstants]( PushConstantsBuffer const & lookup )
				{
					return lookup.data.stageFlags == pushConstants.stageFlags
						&& lookup.data.size > pushConstants.offset + pushConstants.size;
				} );
		}

		if ( it == m_constantsPcbs.end() )
		{
			// Try a PCB that contains the flags of the push constants.
			it = std::find_if( m_constantsPcbs.begin()
				, m_constantsPcbs.end()
				, [&pushConstants]( PushConstantsBuffer const & lookup )
				{
					return checkFlag( lookup.data.stageFlags, pushConstants.stageFlags )
						&& lookup.data.size == pushConstants.offset + pushConstants.size;
				} );
		}

		if ( it == m_constantsPcbs.end() )
		{
			// Try a PCB that contains the flags of the push constants, and is larger than them.
			it = std::find_if( m_constantsPcbs.begin()
				, m_constantsPcbs.end()
				, [&pushConstants]( PushConstantsBuffer const & lookup )
				{
					return checkFlag( lookup.data.stageFlags, pushConstants.stageFlags )
						&& lookup.data.size > pushConstants.offset + pushConstants.size;
				} );
		}

		if ( it != m_constantsPcbs.end() )
		{
			return PushConstantsBuffer
			{
				it->ubo,
				it->location,
				pushConstants
			};
		}

		static PushConstantsBuffer const dummy{};
		return dummy;
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

	void Pipeline::doCompileProgram( Device const & device )
	{
		for ( auto & state : m_createInfo.stages )
		{
			auto module = std::static_pointer_cast< ShaderModule >( state.module );
			m_programLayout.emplace( state.module->getStage(), module->compile( state ) );
		}

		for ( auto & shaderLayoutIt : m_programLayout )
		{
			for ( auto & blockLayout : shaderLayoutIt.second.interfaceBlockLayout )
			{
				PushConstantsBuffer pcb
				{
					std::make_shared< UniformBuffer >( device
						, 1u
						, blockLayout.size
						, 0u
						, ashes::MemoryPropertyFlag::eHostVisible ),
					blockLayout.binding,
					{
						shaderLayoutIt.first,
						0u,
						blockLayout.size
					}
				};
				m_constantsPcbs.push_back( std::move( pcb ) );
			}
		}
	}

	void Pipeline::doCreateInputLayout( Device const & device )
	{
		auto it = m_programLayout.find( ashes::ShaderStageFlag::eVertex );

		if ( it != m_programLayout.end() )
		{
			auto compiled = it->second.module->getCompiled();
			auto & inputLayout = it->second.inputLayout;
			auto d3ddevice = device.getDevice();
			auto inputDesc = convert( m_createInfo.vertexInputState, inputLayout );

			if ( !inputDesc.empty() )
			{
				auto hr = d3ddevice->CreateInputLayout( inputDesc.data()
					, UINT( inputDesc.size() )
					, compiled->GetBufferPointer()
					, compiled->GetBufferSize()
					, &m_iaState );

				if ( !dxCheckError( hr, "CreateInputLayout" ) )
				{
					throw std::runtime_error( "CreateInputLayout() failed" );
				}

				dxDebugName( m_iaState, PipelineInputLayout );
			}
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
