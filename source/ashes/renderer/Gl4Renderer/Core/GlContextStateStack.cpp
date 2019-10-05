#include "Core/GlContextStateStack.hpp"

#include "Command/GlQueue.hpp"
#include "Command/Commands/GlCommandBase.hpp"
#include "Core/GlContextLock.hpp"
#include "Core/GlDevice.hpp"

#include "ashesgl4_api.hpp"

#include <iterator>

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl4
{
	namespace
	{
		thread_local ContextState initialState;

		bool doApplyEnable( CmdList & list
			, uint32_t value
			, bool enable )
		{
			if ( enable )
			{
				list.emplace_back( makeCmd< OpType::eEnable >( GlTweak( value ) ) );
			}
			else
			{
				list.emplace_back( makeCmd< OpType::eDisable >( GlTweak( value ) ) );
			}

			return enable;
		}

		void doApplyTopology( CmdList & list
			, VkPipelineInputAssemblyStateCreateInfo const & state )
		{
			doApplyEnable( list
				, GL_PROGRAM_POINT_SIZE
				, state.topology == VK_PRIMITIVE_TOPOLOGY_POINT_LIST );
		}

		void doApplyPrimitiveRestart( CmdList & list
			, VkPipelineInputAssemblyStateCreateInfo const & state )
		{
			if ( doApplyEnable( list
				, GL_PRIMITIVE_RESTART
				, state.primitiveRestartEnable ) )
			{
			}
		}

		void doApplyLogicOp( CmdList & list
			, VkPipelineColorBlendStateCreateInfo const & state )
		{
			if ( state.logicOpEnable )
			{
				list.emplace_back( makeCmd< OpType::eLogicOp >( convert( state.logicOp ) ) );
			}
			else
			{
				list.emplace_back( makeCmd< OpType::eLogicOp >( GL_LOGIC_OP_COPY ) );
			}
		}

		void doApplyBlendConstants( CmdList & list
			, VkPipelineColorBlendStateCreateInfo const & state )
		{
			auto & blendConstants = state.blendConstants;
			list.emplace_back( makeCmd< OpType::eBlendConstants >( blendConstants[0]
				, blendConstants[1]
				, blendConstants[2]
				, blendConstants[3] ) );
		}

		bool doApplyBlendAttach( CmdList & list
			, VkPipelineColorBlendAttachmentState const & state
			, uint32_t index )
		{
			if ( state.blendEnable )
			{
				list.emplace_back( makeCmd< OpType::eBlendEquation >( index
					, convert( state.colorBlendOp )
					, convert( state.alphaBlendOp ) ) );
				list.emplace_back( makeCmd< OpType::eBlendFunc >( index
					, convert( state.srcColorBlendFactor )
					, convert( state.dstColorBlendFactor )
					, convert( state.srcAlphaBlendFactor )
					, convert( state.dstAlphaBlendFactor ) ) );
			}

			return state.blendEnable;
		}

		bool doApplyBlendAttaches( CmdList & list
			, VkPipelineColorBlendStateCreateInfo const & state )
		{
			bool result = false;
			GLuint index = 0u;

			for ( auto & attach : makeArrayView( state.pAttachments, state.attachmentCount ) )
			{
				result = doApplyBlendAttach( list, attach, index )
					|| result;
				++index;
			}

			return result;
		}

		void doApplyBlending( CmdList & list
			, bool enable )
		{
			doApplyEnable( list
				, GL_BLEND
				, enable );
		}

		void doApplyPolygonMode( CmdList & list
			, VkPipelineRasterizationStateCreateInfo const & state )
		{
			list.emplace_back( makeCmd< OpType::ePolygonMode >( convert( state.polygonMode ) ) );
		}

		void doApplyLineWidth( CmdList & list
			, VkPipelineRasterizationStateCreateInfo const & rasterState
			, VkPipelineDynamicStateCreateInfo dynamicState )
		{
			if ( rasterState.polygonMode == VK_POLYGON_MODE_LINE
				&& !hasDynamicState( dynamicState.pDynamicStates
					, dynamicState.pDynamicStates + dynamicState.dynamicStateCount
					, VK_DYNAMIC_STATE_LINE_WIDTH ) )
			{
				list.emplace_back( makeCmd< OpType::eLineWidth >( rasterState.lineWidth ) );
			}
		}

		void doApplyDepthBias( CmdList & list
			, VkPipelineRasterizationStateCreateInfo const & rasterState
			, VkPipelineDynamicStateCreateInfo dynamicState )
		{
			switch ( rasterState.polygonMode )
			{
			case VK_POLYGON_MODE_FILL:
				doApplyEnable( list
					, GL_POLYGON_OFFSET_FILL
					, rasterState.depthBiasEnable );
				break;

			case VK_POLYGON_MODE_LINE:
				doApplyEnable( list
					, GL_POLYGON_OFFSET_LINE
					, rasterState.depthBiasEnable );
				break;

			case VK_POLYGON_MODE_POINT:
				doApplyEnable( list
					, GL_POLYGON_OFFSET_POINT
					, rasterState.depthBiasEnable );
				break;
			}

			if ( rasterState.depthBiasEnable )
			{
				if ( !hasDynamicState( dynamicState.pDynamicStates
					, dynamicState.pDynamicStates + dynamicState.dynamicStateCount
					, VK_DYNAMIC_STATE_DEPTH_BIAS ) )
				{
					list.emplace_back( makeCmd< OpType::ePolygonOffset >( rasterState.depthBiasConstantFactor
						, rasterState.depthBiasSlopeFactor
						, rasterState.depthBiasClamp ) );
				}
			}
		}

		void doApplyDepthClamp( CmdList & list
			, VkPipelineRasterizationStateCreateInfo const & state )
		{
			doApplyEnable( list
				, GL_DEPTH_CLAMP
				, state.depthClampEnable );
		}

		void doApplyRasterizerDiscard( CmdList & list
			, VkPipelineRasterizationStateCreateInfo const & state )
		{
			doApplyEnable( list
				, GL_RASTERIZER_DISCARD
				, state.rasterizerDiscardEnable );
		}

		void doApplyAlphaToCoverage( CmdList & list
			, VkPipelineMultisampleStateCreateInfo const & state )
		{
			doApplyEnable( list
				, GL_SAMPLE_ALPHA_TO_COVERAGE
				, state.alphaToCoverageEnable );
		}

		void doApplyAlphaToOne( CmdList & list
			, VkPipelineMultisampleStateCreateInfo const & state )
		{
			doApplyEnable( list
				, GL_SAMPLE_ALPHA_TO_ONE
				, state.alphaToOneEnable );
		}

		void doApplySampleShading( CmdList & list
			, VkPipelineMultisampleStateCreateInfo const & state )
		{
			if ( doApplyEnable( list
				, GL_SAMPLE_SHADING
				, state.sampleShadingEnable ) )
			{
				list.emplace_back( makeCmd< OpType::eMinSampleShading >( state.minSampleShading ) );
			}
		}

		void doApplyMultisampling( CmdList & list
			, VkPipelineMultisampleStateCreateInfo const & state )
		{
			doApplyEnable( list
				, GL_MULTISAMPLE
				, state.rasterizationSamples != VK_SAMPLE_COUNT_1_BIT );
		}

		void doApplyDepthWrite( CmdList & list
			, VkPipelineDepthStencilStateCreateInfo const & state )
		{
			list.emplace_back( makeCmd< OpType::eDepthMask >( state.depthWriteEnable
				? GL_TRUE
				: GL_FALSE ) );
		}

		void doApplyDepthTest( CmdList & list
			, VkPipelineDepthStencilStateCreateInfo const & state )
		{
			if ( doApplyEnable( list
				, GL_DEPTH_TEST
				, state.depthTestEnable ) )
			{
				list.emplace_back( makeCmd< OpType::eDepthFunc >( convert( state.depthCompareOp ) ) );
			}
		}

		void doApplyStencilOpState( CmdList & list
			, VkStencilOpState const & state
			, GlCullMode face )
		{
			list.emplace_back( makeCmd< OpType::eStencilMask >( face
				, state.writeMask ) );
			list.emplace_back( makeCmd< OpType::eStencilFunc >( face
				, convert( state.compareOp )
				, state.reference
				, state.compareMask ) );
			list.emplace_back( makeCmd< OpType::eStencilOp >( face
				, convert( state.failOp )
				, convert( state.depthFailOp )
				, convert( state.passOp ) ) );
		}

		void doApplyStencilTest( CmdList & list
			, VkPipelineDepthStencilStateCreateInfo const & state )
		{
			if ( doApplyEnable( list
				, GL_STENCIL_TEST
				, state.stencilTestEnable ) )
			{
				doApplyStencilOpState( list, state.back, GL_CULL_MODE_BACK );
				doApplyStencilOpState( list, state.front, GL_CULL_MODE_FRONT );
			}
		}

		void doApplyDepthBoundsTest( CmdList & list
			, VkPipelineDepthStencilStateCreateInfo const & state )
		{
			if ( doApplyEnable( list
				, GL_DEPTH_CLAMP
				, state.depthBoundsTestEnable
					&& ( state.minDepthBounds != 0.0f || state.maxDepthBounds != 0.0f ) ) )
			{
				list.emplace_back( makeCmd< OpType::eDepthRange >( state.minDepthBounds
					, state.maxDepthBounds ) );
			}
		}

		void doApplyPathControlPoints( CmdList & list
			, VkPipelineTessellationStateCreateInfo const & state )
		{
			if ( state.patchControlPoints )
			{
				list.emplace_back( makeCmd< OpType::ePatchParameter >( GL_PATCH_VERTICES
					, int( state.patchControlPoints ) ) );
			}
		}
	}

	ContextStateStack::ContextStateStack( bool tessellation )
		: m_tessellation{ tessellation }
	{
	}

	ContextStateStack::ContextStateStack( VkDevice device )
		: ContextStateStack{ get( device )->getEnabledFeatures().tessellationShader != VK_FALSE }
	{
	}

	void ContextStateStack::apply( ContextLock const & context
		, ContextState & state )
	{
		if ( !m_save )
		{
			m_save = &context->getState();
		}

		CmdList list;
		apply( list, state, true );
		applyList( context, list );
	}

	void ContextStateStack::apply( CmdList & list
		, ContextState & newState
		, bool force )
	{
		if ( !m_save )
		{
			m_save = &initialState;
			force = true;
		}

		apply( list, newState.cbState, force );
		apply( list, newState.dsState, force );
		apply( list, newState.msState, force );
		apply( list, newState.rsState, newState.dyState, force );
		apply( list, newState.iaState, force );

		if ( m_tessellation )
		{
			apply( list, newState.tsState, force );
		}

		*m_save = newState;
	}

	void ContextStateStack::apply( CmdList & list
		, VkPipelineInputAssemblyStateCreateInfo const & newState
		, bool force )
	{
		auto & save = m_save->iaState;

		if ( force || newState.topology != save.topology )
		{
			doApplyTopology( list, newState );
		}

		if ( force || newState.primitiveRestartEnable != save.primitiveRestartEnable )
		{
			doApplyPrimitiveRestart( list, newState );
		}
	}

	void ContextStateStack::apply( CmdList & list
		, VkPipelineColorBlendStateCreateInfo const & newState
		, bool force )
	{
		thread_local bool hadBlend = false;

		auto & save = m_save->cbState;

		if ( force || newState.logicOp != save.logicOp )
		{
			doApplyLogicOp( list, newState );
		}

		if ( force || newState.blendConstants != save.blendConstants )
		{
			doApplyBlendConstants( list, newState );
		}

		bool blend = doApplyBlendAttaches( list, newState );

		if ( force || hadBlend != blend )
		{
			doApplyBlending( list, blend );
			hadBlend = blend;
		}
	}

	void ContextStateStack::apply( CmdList & list
		, VkPipelineRasterizationStateCreateInfo const & newState
		, VkPipelineDynamicStateCreateInfo newDyState
		, bool force )
	{
		auto & save = m_save->rsState;
		auto mode = convertCullModeFlags( newState.cullMode );
		auto enableCulling = newState.cullMode != VK_CULL_MODE_NONE && mode;

		if ( force || newState.cullMode != save.cullMode )
		{
			doApplyEnable( list
				, GL_CULL_FACE
				, enableCulling );
		}

		if ( enableCulling )
		{
			if ( force || newState.cullMode != save.cullMode )
			{
				list.emplace_back( makeCmd< OpType::eCullFace >( mode ) );
			}

			if ( force || newState.frontFace != save.frontFace )
			{
				list.emplace_back( makeCmd< OpType::eFrontFace >( convert( newState.frontFace ) ) );
			}
		}

		if ( force || newState.polygonMode != save.polygonMode )
		{
			doApplyPolygonMode( list, newState );
		}

		if ( force
			|| newState.depthBiasEnable != save.depthBiasEnable
			|| newState.polygonMode != save.polygonMode )
		{
			doApplyDepthBias( list, newState, newDyState );
		}

		if ( force || newState.depthClampEnable != save.depthClampEnable )
		{
			doApplyDepthClamp( list, newState );
		}

		if ( force || newState.rasterizerDiscardEnable != save.rasterizerDiscardEnable )
		{
			doApplyRasterizerDiscard( list, newState );
		}

		doApplyLineWidth( list, newState, newDyState );
	}

	void ContextStateStack::apply( CmdList & list
		, VkPipelineMultisampleStateCreateInfo const & newState
		, bool force )
	{
		auto & save = m_save->msState;

		if ( force || newState.rasterizationSamples != save.rasterizationSamples )
		{
			doApplyMultisampling( list, newState );
		}

		if ( force || newState.rasterizationSamples != VK_SAMPLE_COUNT_1_BIT )
		{
			if ( force || newState.alphaToCoverageEnable != save.alphaToCoverageEnable )
			{
				doApplyAlphaToCoverage( list, newState );
			}

			if ( force || newState.alphaToOneEnable != save.alphaToOneEnable )
			{
				doApplyAlphaToOne( list, newState );
			}

			doApplySampleShading( list, newState );
		}
	}

	void ContextStateStack::apply( CmdList & list
		, VkPipelineDepthStencilStateCreateInfo const & newState
		, bool force )
	{
		auto & save = m_save->dsState;

		if ( force || newState.depthWriteEnable != save.depthWriteEnable )
		{
			doApplyDepthWrite( list, newState );
		}

		if ( force || newState.depthTestEnable != save.depthTestEnable )
		{
			doApplyDepthTest( list, newState );
		}

		if ( force
			|| newState.stencilTestEnable != save.stencilTestEnable
			|| ( newState.stencilTestEnable
				&& ( newState.back != save.back
					|| newState.front != save.front ) ) )
		{
			doApplyStencilTest( list, newState );
		}

		if ( force
			|| newState.depthBoundsTestEnable != save.depthBoundsTestEnable
			|| ( newState.depthBoundsTestEnable
				&& ( newState.minDepthBounds != save.minDepthBounds
					|| newState.maxDepthBounds != save.maxDepthBounds ) ) )
		{
			doApplyDepthBoundsTest( list, newState );
		}
	}

	void ContextStateStack::apply( CmdList & list
		, VkPipelineTessellationStateCreateInfo const & newState
		, bool force )
	{
		auto & save = m_save->tsState;

		if ( force || newState.patchControlPoints != save.patchControlPoints )
		{
			doApplyPathControlPoints( list, newState );
		}
	}

	void ContextStateStack::apply( CmdList & list
		, VkScissorArray const & scissors
		, bool force )
	{
		if ( force || getCurrentScissors() != scissors )
		{
			list.push_back( makeCmd< OpType::eApplyScissors >( scissors ) );
			setCurrentScissors( scissors );
		}
	}

	void ContextStateStack::apply( CmdList & list
		, VkViewportArray const & viewports
		, bool force )
	{
		if ( force || getCurrentViewports() != viewports )
		{
			list.push_back( makeCmd< OpType::eApplyViewports >( viewports ) );
			list.push_back( makeCmd< OpType::eApplyDepthRanges >( viewports ) );
			setCurrentViewports( viewports );
		}
	}

	void ContextStateStack::apply( CmdList & list
		, VkPipelineViewportStateCreateInfo const & newState
		, bool force )
	{
		apply( list
			, VkViewportArray{ newState.pViewports, newState.pViewports + newState.scissorCount }
			, force );
		apply( list
			, VkScissorArray{ newState.pScissors, newState.pScissors + newState.scissorCount }
			, force );
	}
}
