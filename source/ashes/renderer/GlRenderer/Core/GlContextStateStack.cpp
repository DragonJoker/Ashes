#include "Core/GlContextStateStack.hpp"

#include "Command/GlQueue.hpp"
#include "Command/Commands/GlCommandBase.hpp"
#include "Core/GlContextLock.hpp"
#include "Core/GlDevice.hpp"

#include "ashesgl_api.hpp"

#include <iterator>

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl
{
	namespace
	{
		thread_local ContextState initialState = []()
		{
			VkPipelineColorBlendAttachmentStateArray cbStateAttachments{ 1u, getColourBlendStateAttachment() };
			VkPipelineColorBlendStateCreateInfo cbState{ getDefaultColorBlendState( cbStateAttachments ) };
			return ContextState{ std::move( cbState ) };
		}();

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
				list.emplace_back( makeCmd< OpType::eColorMask >( index
					, checkFlag( state.colorWriteMask, VK_COLOR_COMPONENT_R_BIT )
					, checkFlag( state.colorWriteMask, VK_COLOR_COMPONENT_G_BIT )
					, checkFlag( state.colorWriteMask, VK_COLOR_COMPONENT_B_BIT )
					, checkFlag( state.colorWriteMask, VK_COLOR_COMPONENT_A_BIT ) ) );
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
			case VK_POLYGON_MODE_FILL_RECTANGLE_NV:
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

			default:
				assert( false && "Unsupported polygon mode." );
				break;
			}

			if ( rasterState.depthBiasEnable )
			{
				if ( !hasDynamicState( dynamicState.pDynamicStates
					, dynamicState.pDynamicStates + dynamicState.dynamicStateCount
					, VK_DYNAMIC_STATE_DEPTH_BIAS ) )
				{
					list.emplace_back( makeCmd< OpType::ePolygonOffset >( rasterState.depthBiasConstantFactor
						, rasterState.depthBiasClamp
						, rasterState.depthBiasSlopeFactor ) );
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

		void doApplyStencilWrite( CmdList & list
			, uint32_t const & state
			, uint32_t & save
			, GlCullMode face )
		{
			list.emplace_back( makeCmd< OpType::eStencilMask >( face
				, state ) );
			save = state;
		}

		void doApplyDepthBias( CmdList & list
			, VkPolygonMode polygonMode
			, VkBool32 const & newEnable
			, VkBool32 & saveEnable
			, float const & newConstantFactor
			, float & saveConstantFactor
			, float const & newClamp
			, float & saveClamp
			, float const & newSlopeFactor
			, float & saveSlopeFactor )
		{
			switch ( polygonMode )
			{
			case VK_POLYGON_MODE_FILL:
			case VK_POLYGON_MODE_FILL_RECTANGLE_NV:
				doApplyEnable( list
					, GL_POLYGON_OFFSET_FILL
					, newEnable );
				break;

			case VK_POLYGON_MODE_LINE:
				doApplyEnable( list
					, GL_POLYGON_OFFSET_LINE
					, newEnable );
				break;

			case VK_POLYGON_MODE_POINT:
				doApplyEnable( list
					, GL_POLYGON_OFFSET_POINT
					, newEnable );
				break;

			default:
				assert( false && "Unsupported polygon mode." );
				break;
			}
			list.emplace_back( makeCmd< OpType::ePolygonOffset >( newConstantFactor
				, newClamp
				, newSlopeFactor ) );
			saveEnable = newEnable;
			saveConstantFactor = newConstantFactor;
			saveClamp = newClamp;
			saveSlopeFactor = newSlopeFactor;
		}

		void doApplyStencilFunc( CmdList & list
			, VkCompareOp const & newCompareOp
			, VkCompareOp & saveCompareOp
			, uint32_t const & newReference
			, uint32_t & saveReference
			, uint32_t const & newCompareMask
			, uint32_t & saveCompareMask
			, GlCullMode face )
		{
			list.emplace_back( makeCmd< OpType::eStencilFunc >( face
				, convert( newCompareOp )
				, newReference
				, newCompareMask ) );
			saveCompareOp = newCompareOp;
			saveReference = newReference;
			saveCompareMask = newCompareMask;
		}

		void doApplyStencilOp( CmdList & list
			, VkStencilOp const & newFailOp
			, VkStencilOp & saveFailOp
			, VkStencilOp const & newDepthFailOp
			, VkStencilOp & saveDepthFailOp
			, VkStencilOp const & newPassOp
			, VkStencilOp & savePassOp
			, GlCullMode face )
		{
			list.emplace_back( makeCmd< OpType::eStencilOp >( face
				, convert( newFailOp )
				, convert( newDepthFailOp )
				, convert( newPassOp ) ) );
			saveFailOp = newFailOp;
			saveDepthFailOp = newDepthFailOp;
			savePassOp = newPassOp;
		}

		void doApplyStencilOpState( CmdList & list
			, VkStencilOpState const & state
			, VkStencilOpState & save
			, GlCullMode face )
		{
			doApplyStencilWrite( list
				, state.writeMask
				, save.writeMask
				, face );
			doApplyStencilFunc( list
				, state.compareOp
				, save.compareOp
				, state.reference
				, save.reference
				, state.compareMask
				, save.compareMask
				, face );
			doApplyStencilOp( list
				, state.failOp
				, save.failOp
				, state.depthFailOp
				, save.depthFailOp
				, state.passOp
				, save.passOp
				, face );
		}

		void doApplyStencilTest( CmdList & list
			, VkPipelineDepthStencilStateCreateInfo const & state
			, VkPipelineDepthStencilStateCreateInfo & save )
		{
			if ( doApplyEnable( list
				, GL_STENCIL_TEST
				, state.stencilTestEnable ) )
			{
				doApplyStencilOpState( list, state.back, save.back, GL_CULL_MODE_BACK );
				doApplyStencilOpState( list, state.front, save.front, GL_CULL_MODE_FRONT );
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

		struct MocVkScissor
		{
			int x;
			int y;
			int w;
			int h;
		};

		void adjust( MocVkScissor & value
			, VkExtent2D const & renderArea )
		{
			auto vkbottom = value.y + value.h;
			value.y = renderArea.height - vkbottom;
		}

		void adjust( ashes::ArrayView< MocVkScissor > const & values
			, VkExtent2D const & renderArea )
		{
			for ( auto & value : values )
			{
				adjust( value, renderArea );
			}
		}

		VkRect2D adjust( VkRect2D const & value
			, VkExtent2D const & renderArea )
		{
			auto vkbottom = value.offset.y + value.extent.height;
			auto vktop = value.offset.y;
			return
			{
				{ value.offset.x, int32_t( renderArea.height - vkbottom ) },
				{ value.extent.width, value.extent.height },
			};
		}

		VkScissorArray adjust( ArrayView< VkRect2D const > const & values
			, VkExtent2D const & renderArea )
		{
			VkScissorArray result;
			result.reserve( values.size() );

			for ( auto & value : values )
			{
				result.push_back( adjust( value, renderArea ) );
			}

			return result;
		}

		struct MocVkViewport
		{
			float x;
			float y;
			float w;
			float h;
		};

		void adjust( MocVkViewport & value
			, VkExtent2D const & renderArea )
		{
			auto vkbottom = value.y + value.h;
			value.y = renderArea.height - vkbottom;
		}

		void adjust( ashes::ArrayView< MocVkViewport > const & values
			, VkExtent2D const & renderArea )
		{
			for ( auto & value : values )
			{
				adjust( value, renderArea );
			}
		}

		VkViewport adjust( VkViewport const & value
			, VkExtent2D const & renderArea )
		{
			auto vkbottom = value.y + value.height;
			auto vktop = value.y;
			return
			{
				value.x, renderArea.height - vkbottom,
				value.width, value.height,
				value.minDepth, value.maxDepth,
			};
		}

		VkViewportArray adjust( ArrayView< VkViewport const > const & values
			, VkExtent2D const & renderArea )
		{
			VkViewportArray result;
			result.reserve( values.size() );

			for ( auto & value : values )
			{
				result.push_back( adjust( value, renderArea ) );
			}

			return result;
		}
	}

	ContextStateStack::ContextStateStack( bool tessellation
		, bool viewportArrays )
		: m_tessellation{ tessellation }
		, m_viewportArrays{ viewportArrays }
	{
	}

	ContextStateStack::ContextStateStack( VkDevice device )
		: ContextStateStack{ get( device )->getEnabledFeatures().tessellationShader != VK_FALSE
			, get( getInstance( device ) )->hasViewportArray() }
	{
	}

	void ContextStateStack::apply( ContextLock const & context
		, ContextState & state )
	{
		doCheckSave( &context->getState() );
		CmdList list;
		apply( list, state, true );
		applyList( context, list );
	}

	void ContextStateStack::apply( CmdList & list
		, ContextState & newState
		, bool force )
	{
		force = doCheckSave() || force;
		doApply( list, newState.cbState, force );
		doApply( list, newState.dsState, force );
		doApply( list, newState.msState, force );
		doApply( list, newState.rsState, newState.dyState, force );
		doApply( list, newState.iaState, force );

		if ( m_tessellation )
		{
			doApply( list, newState.tsState, force );
		}

		*m_save = newState;
	}

	void ContextStateStack::apply( CmdList & list
		, PreExecuteActions & preExecuteActions
		, uint32_t firstViewport
		, ArrayView< VkViewport const > const & viewports
		, bool force )
	{
		force = doCheckSave() || force;

		if ( force || getCurrentViewports() != viewports )
		{
			if ( !viewports.empty() )
			{
				if ( m_viewportArrays )
				{
					if ( m_renderArea == VkExtent2D{ ~( 0u ), ~( 0u ) } )
					{
						auto index = list.size();
						list.push_back( makeCmd< OpType::eApplyViewports >( firstViewport
							, uint32_t( viewports.size() )
							, viewports ) );
						preExecuteActions.push_back( [index]( CmdList & list
							, ContextStateStack const & stack )
							{
								Command * pCmd = nullptr;
								auto it = list[index].begin();

								if ( map( it, list[index].end(), pCmd ) )
								{
									assert( pCmd->op.type == OpType::eApplyViewports );
									CmdApplyViewports & oldCmd = map< OpType::eApplyViewports >( *pCmd );
									adjust( ashes::makeArrayView( reinterpret_cast< MocVkViewport * >( oldCmd.viewports.data() )
										, reinterpret_cast< MocVkViewport * >( oldCmd.viewports.data() ) + oldCmd.count )
										, stack.m_renderArea );
								}
							} );
					}
					else
					{
						list.push_back( makeCmd< OpType::eApplyViewports >( firstViewport
							, uint32_t( viewports.size() )
							, adjust( viewports, m_renderArea ) ) );
					}

					list.push_back( makeCmd< OpType::eApplyDepthRanges >( firstViewport
						, uint32_t( viewports.size() )
						, viewports ) );
				}
				else if ( m_renderArea == VkExtent2D{ ~( 0u ), ~( 0u ) } )
				{
					auto index = list.size();
					list.push_back( makeCmd< OpType::eApplyViewport >( viewports.front() ) );
					preExecuteActions.push_back( [index]( CmdList & list
						, ContextStateStack const & stack )
						{
							Command * pCmd = nullptr;
							auto it = list[index].begin();

							if ( map( it, list[index].end(), pCmd ) )
							{
								assert( pCmd->op.type == OpType::eApplyViewport );
								CmdApplyViewport & oldCmd = map< OpType::eApplyViewport >( *pCmd );
								adjust( oldCmd.viewport, stack.m_renderArea );
							}
						} );
				}
				else
				{
					list.push_back( makeCmd< OpType::eApplyViewport >( adjust( viewports.front(), m_renderArea ) ) );
				}
			}
			else if ( m_renderArea == VkExtent2D{ ~( 0u ), ~( 0u ) } )
			{
				auto index = list.size();
				list.push_back( makeCmd< OpType::eApplyViewport >( VkViewport{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f } ) );
				preExecuteActions.push_back( [index]( CmdList & list
					, ContextStateStack const & stack )
					{
						Command * pCmd = nullptr;
						auto it = list[index].begin();

						if ( map( it, list[index].end(), pCmd ) )
						{
							assert( pCmd->op.type == OpType::eApplyViewport );
							CmdApplyViewport & oldCmd = map< OpType::eApplyViewport >( *pCmd );
							oldCmd.viewport = VkViewport
							{
								0.0f, 0.0f,
								float( stack.m_renderArea.width ), float( stack.m_renderArea.height ),
								0.0f, 1.0f
							};
						}
					} );
			}
			else
			{
				list.push_back( makeCmd< OpType::eApplyViewport >( VkViewport{ 0.0f, 0.0f, float( m_renderArea.width ), float( m_renderArea.height ), 0.0f, 1.0f } ) );
			}

			setCurrentViewports( viewports );
		}
	}

	void ContextStateStack::apply( CmdList & list
		, PreExecuteActions & preExecuteActions
		, uint32_t firstScissor
		, ArrayView< VkRect2D const > const & scissors
		, bool force )
	{
		force = doCheckSave() || force;

		if ( force || getCurrentScissors() != scissors )
		{
			doApplyEnable( list
				, GL_SCISSOR_TEST
				, !scissors.empty() );

			if ( !scissors.empty() )
			{
				if ( m_viewportArrays )
				{
					if ( m_renderArea == VkExtent2D{ ~( 0u ), ~( 0u ) } )
					{
						auto index = list.size();
						list.push_back( makeCmd< OpType::eApplyScissors >( firstScissor
							, uint32_t( scissors.size() )
							, scissors ) );
						preExecuteActions.push_back( [index]( CmdList & list
							, ContextStateStack const & stack )
							{
								if ( stack.isRtot() )
								{
									Command * pCmd = nullptr;
									auto it = list[index].begin();

									if ( map( it, list[index].end(), pCmd ) )
									{
										assert( pCmd->op.type == OpType::eApplyScissors );
										CmdApplyScissors & oldCmd = map< OpType::eApplyScissors >( *pCmd );
										adjust( ashes::makeArrayView( reinterpret_cast< MocVkScissor * >( oldCmd.scissors.data() )
											, reinterpret_cast< MocVkScissor * >( oldCmd.scissors.data() ) + oldCmd.count )
											, stack.m_renderArea );
									}
								}
							} );
					}
					else if ( isRtot() )
					{
						list.push_back( makeCmd< OpType::eApplyScissors >( firstScissor
							, uint32_t( scissors.size() )
							, adjust( scissors, m_renderArea ) ) );
					}
					else
					{
						list.push_back( makeCmd< OpType::eApplyScissors >( firstScissor
							, uint32_t( scissors.size() )
							, scissors ) );
					}
				}
				else if ( m_renderArea == VkExtent2D{ ~( 0u ), ~( 0u ) } )
				{
					auto index = list.size();
					list.push_back( makeCmd< OpType::eApplyScissor >( VkRect2D{} ) );
					preExecuteActions.push_back( [index]( CmdList & list
						, ContextStateStack const & stack )
						{
							if ( stack.isRtot() )
							{
								Command * pCmd = nullptr;
								auto it = list[index].begin();

								if ( map( it, list[index].end(), pCmd ) )
								{
									assert( pCmd->op.type == OpType::eApplyScissor );
									CmdApplyScissor & oldCmd = map< OpType::eApplyScissor >( *pCmd );
									oldCmd.scissor = VkRect2D
									{
										{ 0, 0 },
									{ stack.m_renderArea.width, stack.m_renderArea.height },
									};
								}
							}
						} );
				}
				else if ( isRtot() )
				{
					list.push_back( makeCmd< OpType::eApplyScissor >( adjust( scissors.front(), m_renderArea ) ) );
				}
				else
				{
					list.push_back( makeCmd< OpType::eApplyScissor >( scissors.front() ) );
				}
			}
			else if ( m_renderArea == VkExtent2D{ ~( 0u ), ~( 0u ) } )
			{
				auto index = list.size();
				list.push_back( makeCmd< OpType::eApplyScissor >( VkRect2D{ {}, {} } ) );
				preExecuteActions.push_back( [index]( CmdList & list
					, ContextStateStack const & stack )
					{
						if ( stack.isRtot() )
						{
							Command * pCmd = nullptr;
							auto it = list[index].begin();

							if ( map( it, list[index].end(), pCmd ) )
							{
								assert( pCmd->op.type == OpType::eApplyScissor );
								CmdApplyScissor oldCmd = map< OpType::eApplyScissor >( *pCmd );
								oldCmd.scissor = VkRect2D{ { 0, 0 }, stack.m_renderArea };
							}
						}
					} );
			}
			else
			{
				list.push_back( makeCmd< OpType::eApplyScissor >( VkRect2D{ {}, m_renderArea } ) );
			}

			setCurrentScissors( scissors );
		}
	}

	void ContextStateStack::applySRGBStatus( CmdList & list
		, bool enable
		, bool force )
	{
		force = doCheckSave() || force;

		if ( enable != m_save->sRGB )
		{
			if ( enable )
			{
				list.push_back( makeCmd< OpType::eEnable >( GL_FRAMEBUFFER_SRGB ) );
			}
			else
			{
				list.push_back( makeCmd< OpType::eDisable >( GL_FRAMEBUFFER_SRGB ) );
			}

			m_save->sRGB = enable;
		}
	}

	void ContextStateStack::applyDepthBias( CmdList & list
		, float constantFactor
		, float clamp
		, float slopeFactor )
	{
		doCheckSave();
		auto & save = m_save->rsState;
		doApplyDepthBias( list
			, save.polygonMode
			, VK_TRUE
			, save.depthBiasEnable
			, constantFactor
			, save.depthBiasConstantFactor
			, clamp
			, save.depthBiasClamp
			, slopeFactor
			, save.depthBiasSlopeFactor );
	}

	void ContextStateStack::applyStencilCompareMask( CmdList & list
		, uint32_t compareMask
		, VkStencilFaceFlags faceFlags )
	{
		doCheckSave();
		auto & save = m_save->dsState;

		if ( faceFlags & VK_STENCIL_FACE_FRONT_BIT )
		{
			doApplyStencilFunc( list
				, save.front.compareOp
				, save.front.compareOp
				, save.front.reference
				, save.front.reference
				, compareMask
				, save.front.compareMask
				, GL_CULL_MODE_FRONT );
		}

		if ( faceFlags & VK_STENCIL_FACE_BACK_BIT )
		{
			doApplyStencilFunc( list
				, save.back.compareOp
				, save.back.compareOp
				, save.back.reference
				, save.back.reference
				, compareMask
				, save.back.compareMask
				, GL_CULL_MODE_BACK );
		}
	}

	void ContextStateStack::applyStencilWriteMask( CmdList & list
		, uint32_t writeMask
		, VkStencilFaceFlags faceFlags )
	{
		doCheckSave();
		auto & save = m_save->dsState;

		if ( faceFlags & VK_STENCIL_FACE_FRONT_BIT )
		{
			doApplyStencilWrite( list
				, writeMask
				, save.front.writeMask
				, GL_CULL_MODE_FRONT );
		}

		if ( faceFlags & VK_STENCIL_FACE_BACK_BIT )
		{
			doApplyStencilWrite( list
				, writeMask
				, save.back.writeMask
				, GL_CULL_MODE_BACK );
		}
	}

	void ContextStateStack::applyStencilReference( CmdList & list
		, uint32_t reference
		, VkStencilFaceFlags faceFlags )
	{
		doCheckSave();
		auto & save = m_save->dsState;

		if ( faceFlags & VK_STENCIL_FACE_FRONT_BIT )
		{
			doApplyStencilFunc( list
				, save.front.compareOp
				, save.front.compareOp
				, reference
				, save.front.reference
				, save.front.compareMask
				, save.front.compareMask
				, GL_CULL_MODE_FRONT );
		}

		if ( faceFlags & VK_STENCIL_FACE_BACK_BIT )
		{
			doApplyStencilFunc( list
				, save.back.compareOp
				, save.back.compareOp
				, reference
				, save.back.reference
				, save.back.compareMask
				, save.back.compareMask
				, GL_CULL_MODE_BACK );
		}
	}

	void ContextStateStack::setCurrentFramebuffer( VkFramebuffer value )
	{
		m_currentFbo = value;

		if ( m_currentFbo )
		{
			m_isRtot = get( m_currentFbo )->hasSwapchainImage();
		}
	}

	void ContextStateStack::doApply( CmdList & list
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

	void ContextStateStack::doApply( CmdList & list
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

	void ContextStateStack::doApply( CmdList & list
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

		if ( force || newState.cullMode != save.cullMode )
		{
			if ( enableCulling )
			{
				list.emplace_back( makeCmd< OpType::eCullFace >( mode ) );
			}
		}

		if ( force || newState.frontFace != save.frontFace )
		{
			list.emplace_back( makeCmd< OpType::eFrontFace >( convert( newState.frontFace ) ) );
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

	void ContextStateStack::doApply( CmdList & list
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

	void ContextStateStack::doApply( CmdList & list
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
			doApplyStencilTest( list, newState, save );
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

	void ContextStateStack::doApply( CmdList & list
		, VkPipelineTessellationStateCreateInfo const & newState
		, bool force )
	{
		auto & save = m_save->tsState;

		if ( force || newState.patchControlPoints != save.patchControlPoints )
		{
			doApplyPathControlPoints( list, newState );
		}
	}

	void ContextStateStack::doApply( CmdList & list
		, PreExecuteActions & preExecuteActions
		, VkPipelineViewportStateCreateInfo const & newState
		, bool force )
	{
		apply( list
			, preExecuteActions
			, 0u
			, makeArrayView( newState.pViewports, newState.pViewports + newState.scissorCount )
			, force );
		apply( list
			, preExecuteActions
			, 0u
			, makeArrayView( newState.pScissors, newState.pScissors + newState.scissorCount )
			, force );
	}

	bool ContextStateStack::doCheckSave( ContextState * state )
	{
		bool result = m_save;

		if ( !result )
		{
			if ( state )
			{
				m_save = state;
			}
			else
			{
				m_ownInitial = std::make_unique< ContextState >( initialState );
				m_save = m_ownInitial.get();
			}

			result = true;
		}

		return result;
	}
}
