/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11BindPipelineCommand.hpp"

#include "Core/D3D11Device.hpp"
#include "Pipeline/D3D11Pipeline.hpp"
#include "Pipeline/D3D11PipelineLayout.hpp"
#include "Shader/D3D11ShaderModule.hpp"

#if defined( ASHES_D3D11_USE_AMD_AGS )
#	pragma warning( push )
#	pragma warning( disable: 4828 )
#	include <amd_ags.h>
#	pragma warning( pop )
#endif

#if defined( ASHES_D3D11_USE_NVAPI )
#	pragma warning( push )
#	pragma warning( disable: 4828 )
#	include <nvapi.h>
#	pragma warning( pop )
#endif

#include <array>

#include "ashesd3d11_api.hpp"

namespace ashes::D3D11_NAMESPACE
{
	//*************************************************************************

	namespace
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
			, ID3D11DepthStencilState * state
			, UINT stencilRef )
		{
			context->OMSetDepthStencilState( state, stencilRef );
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
	}

	//*************************************************************************

	BindPipelineCommand::BindPipelineCommand( VkDevice device
		, VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint )
		: CommandBase{ device }
		, m_pipeline{ pipeline }
		, m_layout{ get( m_pipeline )->getLayout() }
		, m_bindingPoint{ bindingPoint }
		, m_dynamicBlendFactor{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_BLEND_CONSTANTS ) }
		, m_dynamicLineWidth{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_LINE_WIDTH ) }
		, m_dynamicDepthBias{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_DEPTH_BIAS ) }
		, m_dynamicStencil{ ( get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK )
			|| get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_WRITE_MASK )
			|| get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_STENCIL_REFERENCE ) ) }
		, m_dynamicScissor{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_SCISSOR ) }
		, m_dynamicViewport{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_VIEWPORT ) }
	{
	}

	void BindPipelineCommand::apply( Context const & context )const
	{
		ashes::D3D11_NAMESPACE::apply( *context.context
			, get( m_pipeline )->getIAState() );

		if ( !m_dynamicStencil )
		{
			ashes::D3D11_NAMESPACE::apply( *context.context
				, get( m_pipeline )->getDSState()
				, get( m_pipeline )->getStencilRef() );
		}

		if ( !m_dynamicDepthBias )
		{
			ashes::D3D11_NAMESPACE::apply( *context.context
				, get( m_pipeline )->getRSState() );
		}

		if ( !m_dynamicBlendFactor )
		{
			ashes::D3D11_NAMESPACE::apply( *context.context
				, get( m_pipeline )->getBDState()
				, get( m_pipeline )->getBlendFactor()
				, get( m_pipeline )->getSampleMask() );
		}

		if ( !m_dynamicViewport )
		{
			assert( get( m_pipeline )->hasViewport() );
			ashes::D3D11_NAMESPACE::apply( *context.context
				, get( m_pipeline )->getViewports() );
		}

		if ( !m_dynamicScissor )
		{
			assert( get( m_pipeline )->hasScissor() );
			ashes::D3D11_NAMESPACE::apply( *context.context
				, get( m_pipeline )->getScissors() );
		}

		// Bind program
		for ( auto const & stage : get( m_pipeline )->getShaderStages() )
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
		ashes::D3D11_NAMESPACE::apply( *context.context
			, ( ID3D11InputLayout * )nullptr );

		if ( !m_dynamicStencil )
		{
			ashes::D3D11_NAMESPACE::apply( *context.context
				, ( ID3D11DepthStencilState * )nullptr
				, get( m_pipeline )->getStencilRef() );
		}

		if ( !m_dynamicDepthBias )
		{
			ashes::D3D11_NAMESPACE::apply( *context.context
				, ( ID3D11RasterizerState * )nullptr );
		}

		if ( !m_dynamicBlendFactor )
		{
			ashes::D3D11_NAMESPACE::apply( *context.context
				, nullptr
				, get( m_pipeline )->getBlendFactor()
				, get( m_pipeline )->getSampleMask() );
		}

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

	//*************************************************************************

	BindDepthStencilStateCommand::BindDepthStencilStateCommand( VkDevice device
		, VkPipeline pipeline )
		: CommandBase{ device }
		, m_pipeline{ pipeline }
	{
	}

	void BindDepthStencilStateCommand::apply( Context const & context )const
	{
		ashes::D3D11_NAMESPACE::apply( *context.context
			, get( m_pipeline )->getDSState()
			, get( m_pipeline )->getDynamicStates().getBackStencilReference() );
	}

	void BindDepthStencilStateCommand::remove( Context const & context )const
	{
		ashes::D3D11_NAMESPACE::apply( *context.context
			, ( ID3D11DepthStencilState * )nullptr
			, get( m_pipeline )->getDynamicStates().getBackStencilReference() );
	}

	CommandPtr BindDepthStencilStateCommand::clone()const
	{
		return std::make_unique< BindDepthStencilStateCommand >( *this );
	}

	//*************************************************************************

	BindRasterizerStateCommand::BindRasterizerStateCommand( VkDevice device
		, VkPipeline pipeline )
		: CommandBase{ device }
		, m_pipeline{ pipeline }
	{
	}

	void BindRasterizerStateCommand::apply( Context const & context )const
	{
		ashes::D3D11_NAMESPACE::apply( *context.context
			, get( m_pipeline )->getRSState() );
	}

	void BindRasterizerStateCommand::remove( Context const & context )const
	{
		ashes::D3D11_NAMESPACE::apply( *context.context
			, ( ID3D11RasterizerState * )nullptr );
	}

	CommandPtr BindRasterizerStateCommand::clone()const
	{
		return std::make_unique< BindRasterizerStateCommand >( *this );
	}

	//*************************************************************************

	BindBlendStateCommand::BindBlendStateCommand( VkDevice device
		, VkPipeline pipeline
		, std::array< float, 4u > const & blendConstants )
		: CommandBase{ device }
		, m_pipeline{ pipeline }
		, m_blendConstants{ blendConstants }
	{
	}

	void BindBlendStateCommand::apply( Context const & context )const
	{
		ashes::D3D11_NAMESPACE::apply( *context.context
			, get( m_pipeline )->getBDState()
			, m_blendConstants.data()
			, get( m_pipeline )->getSampleMask() );
	}

	void BindBlendStateCommand::remove( Context const & context )const
	{
		ashes::D3D11_NAMESPACE::apply( *context.context
			, nullptr
			, m_blendConstants.data()
			, get( m_pipeline )->getSampleMask() );
	}

	CommandPtr BindBlendStateCommand::clone()const
	{
		return std::make_unique< BindBlendStateCommand >( *this );
	}

	//*************************************************************************

	SetDepthBoundsCommand::SetDepthBoundsCommand( VkDevice device
		, DepthBounds const & depthBounds )
		: CommandBase{ device }
		, m_physicalDevice{ get( getDevice() )->getPhysicalDevice() }
		, m_depthBounds{ depthBounds }
	{
	}

	void SetDepthBoundsCommand::apply( Context const & context )const
	{
#if ASHES_D3D11_USE_NVAPI
		if ( get( m_physicalDevice )->isNVIDIA() )
		{
			NvAPI_D3D11_SetDepthBoundsTest( *context.context
				, TRUE
				, m_depthBounds.minBound
				, m_depthBounds.maxBound );
		}
		else
#endif
#if ASHES_D3D11_USE_AMD_AGS
		if ( get( m_physicalDevice )->isAMD() )
		{
			agsDriverExtensionsDX11_SetDepthBounds( nullptr
				, *context.context
				, true
				, m_depthBounds.minBound
				, m_depthBounds.maxBound );
		}
#endif
	}

	void SetDepthBoundsCommand::remove( Context const & context )const
	{
#if ASHES_D3D11_USE_NVAPI
		if ( get( m_physicalDevice )->isNVIDIA() )
		{
			NvAPI_D3D11_SetDepthBoundsTest( *context.context
				, FALSE
				, m_depthBounds.minBound
				, m_depthBounds.maxBound );
		}
		else
#endif
#if ASHES_D3D11_USE_AMD_AGS
		if ( get( m_physicalDevice )->isAMD() )
		{
			agsDriverExtensionsDX11_SetDepthBounds( get( m_physicalDevice )->getAGSContext()
				, *context.context
				, false
				, m_depthBounds.minBound
				, m_depthBounds.maxBound );
		}
#endif
	}

	CommandPtr SetDepthBoundsCommand::clone()const
	{
		return std::make_unique< SetDepthBoundsCommand >( *this );
	}

	//*************************************************************************
}
