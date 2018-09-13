/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBindPipelineCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Pipeline/GlPipeline.hpp"
#include "Pipeline/GlPipelineLayout.hpp"

#include <algorithm>

namespace gl_renderer
{
	void apply( Device const & device
		, ContextLock const & context
		, ashes::InputAssemblyState const & state )
	{
		auto & save = device.getCurrentInputAssemblyState();

		if ( state != save )
		{
			if ( state.topology != save.topology )
			{
				if ( state.topology == ashes::PrimitiveTopology::ePointList )
				{
					glLogCall( context
						, glEnable
						, GL_PROGRAM_POINT_SIZE );
				}
				else
				{
					glLogCall( context
						, glDisable
						, GL_PROGRAM_POINT_SIZE );
				}
			}

			if ( state.primitiveRestartEnable != save.primitiveRestartEnable )
			{
				if ( state.primitiveRestartEnable )
				{
					glLogCall( context
						, glEnable
						, GL_PRIMITIVE_RESTART );
				}
				else
				{
					glLogCall( context
						, glDisable
						, GL_PRIMITIVE_RESTART );
				}
			}
		}
	}

	void apply( Device const & device
		, ContextLock const & context
		, ashes::ColourBlendState const & state )
	{
		auto & save = device.getCurrentBlendState();
		static bool hadBlend = false;

		if ( state != save )
		{
			if ( state.logicOpEnable
				&& !save.logicOpEnable
				&& state.logicOp != save.logicOp )
			{
				glLogCall( context
					, glLogicOp
					, convert( state.logicOp ) );
			}

			if ( state.blendConstants != save.blendConstants )
			{
				auto & blendConstants = state.blendConstants;
				glLogCall( context
					, glBlendColor
					, blendConstants[0]
					, blendConstants[1]
					, blendConstants[2]
					, blendConstants[3] );
			}

			bool blend = false;
			GLuint buf = 0u;

			if ( context->hasBlendEquationSeparatei_40() )
			{
				for ( auto & blendState : state.attachs )
				{
					if ( blendState.blendEnable )
					{
						blend = true;
						glLogCall( context
							, glBlendEquationSeparatei_40
							, buf
							, convert( blendState.colorBlendOp )
							, convert( blendState.alphaBlendOp ) );
						glLogCall( context
							, glBlendFuncSeparatei_40
							, buf
							, convert( blendState.srcColorBlendFactor )
							, convert( blendState.dstColorBlendFactor )
							, convert( blendState.srcAlphaBlendFactor )
							, convert( blendState.dstAlphaBlendFactor ) );
					}

					++buf;
				}
			}
			else
			{
				auto count = std::count_if( state.attachs.begin()
					, state.attachs.end()
					, []( ashes::ColourBlendStateAttachment const & attach )
					{
						return attach.blendEnable;
					} );
				blend = count > 0;

				if ( count > 1 )
				{
					ashes::Logger::logWarning( "Separate blend equations are not available." );
				}

				if ( blend )
				{
					auto it = std::find_if( state.attachs.begin()
						, state.attachs.end()
						, []( ashes::ColourBlendStateAttachment const & attach )
						{
							return attach.blendEnable;
						} );

					glLogCall( context
						, glBlendEquationSeparate
						, convert( it->colorBlendOp )
						, convert( it->alphaBlendOp ) );
					glLogCall( context
						, glBlendFuncSeparate
						, convert( it->srcColorBlendFactor )
						, convert( it->dstColorBlendFactor )
						, convert( it->srcAlphaBlendFactor )
						, convert( it->dstAlphaBlendFactor ) );
				}
			}

			if ( hadBlend != blend )
			{
				if ( blend )
				{
					glLogCall( context
						, glEnable
						, GL_BLEND );
				}
				else
				{
					glLogCall( context
						, glDisable
						, GL_BLEND );
				}
			}

			save = state;
			hadBlend = blend;
		}
	}

	void apply( Device const & device
		, ContextLock const & context
		, ashes::RasterisationState const & state
		, bool dynamicLineWidth
		, bool dynamicDepthBias )
	{
		auto & save = device.getCurrentRasterisationState();

		if ( state != save )
		{
			if ( state.cullMode != save.cullMode
				|| state.frontFace != save.frontFace )
			{
				if ( state.cullMode != ashes::CullModeFlag::eNone )
				{
					if ( save.cullMode == ashes::CullModeFlag::eNone )
					{
						glLogCall( context
							, glEnable
							, GL_CULL_FACE );
					}

					if ( state.cullMode != save.cullMode )
					{
						glLogCall( context
							, glCullFace
							, convert( state.cullMode ) );
					}

					if ( state.frontFace != save.frontFace )
					{
						glLogCall( context
							, glFrontFace
							, convert( state.frontFace ) );
					}
				}
				else if ( save.cullMode != ashes::CullModeFlag::eNone )
				{
					glLogCall( context
						, glDisable
						, GL_CULL_FACE );
				}
			}

			if ( state.polygonMode != save.polygonMode )
			{
				glLogCall( context
					, glPolygonMode
					, GL_CULL_MODE_FRONT_AND_BACK
					, convert( state.polygonMode ) );
			}

			if ( state.depthBiasEnable != save.depthBiasEnable
				|| state.polygonMode != save.polygonMode )
			{
				if ( state.depthBiasEnable )
				{
					switch ( state.polygonMode )
					{
					case ashes::PolygonMode::eFill:
						glLogCall( context
							, glEnable
							, GL_POLYGON_OFFSET_FILL );
						break;

					case ashes::PolygonMode::eLine:
						glLogCall( context
							, glEnable
							, GL_POLYGON_OFFSET_LINE );
						break;

					case ashes::PolygonMode::ePoint:
						glLogCall( context
							, glEnable
							, GL_POLYGON_OFFSET_POINT );
						break;
					}

					if ( !dynamicDepthBias )
					{
						glLogCall( context
							, glPolygonOffsetClampEXT
							, state.depthBiasConstantFactor
							, state.depthBiasSlopeFactor
							, state.depthBiasClamp );
					}
				}
				else
				{
					switch ( state.polygonMode )
					{
					case ashes::PolygonMode::eFill:
						glLogCall( context
							, glDisable
							, GL_POLYGON_OFFSET_FILL );
						break;

					case ashes::PolygonMode::eLine:
						glLogCall( context
							, glDisable
							, GL_POLYGON_OFFSET_LINE );
						break;

					case ashes::PolygonMode::ePoint:
						glLogCall( context
							, glDisable
							, GL_POLYGON_OFFSET_POINT );
						break;
					}
				}
			}

			if ( state.depthClampEnable != save.depthClampEnable )
			{
				if ( state.depthClampEnable )
				{
					glLogCall( context
						, glEnable
						, GL_DEPTH_CLAMP );
				}
				else
				{
					glLogCall( context
						, glDisable
						, GL_DEPTH_CLAMP );
				}
			}

			if ( state.rasteriserDiscardEnable != save.rasteriserDiscardEnable )
			{
				if ( state.rasteriserDiscardEnable )
				{
					glLogCall( context
						, glEnable
						, GL_RASTERIZER_DISCARD );
				}
				else
				{
					glLogCall( context
						, glDisable
						, GL_RASTERIZER_DISCARD );
				}
			}

			if ( state.polygonMode == ashes::PolygonMode::eLine
				&& !dynamicLineWidth )
			{
				glLogCall( context
					, glLineWidth
					, state.lineWidth );
			}

			save = state;
		}
	}

	void apply( Device const & device
		, ContextLock const & context
		, ashes::MultisampleState const & state )
	{
		auto & save = device.getCurrentMultisampleState();

		if ( state != save )
		{
			if ( state.rasterisationSamples != ashes::SampleCountFlag::e1 )
			{
				glLogCall( context
					, glEnable
					, GL_MULTISAMPLE );

				if ( state.alphaToCoverageEnable != save.alphaToCoverageEnable )
				{
					if ( state.alphaToCoverageEnable )
					{
						glLogCall( context
							, glEnable
							, GL_SAMPLE_ALPHA_TO_COVERAGE );
					}
					else
					{
						glLogCall( context
							, glDisable
							, GL_SAMPLE_ALPHA_TO_COVERAGE );
					}
				}

				if ( state.alphaToOneEnable != save.alphaToOneEnable )
				{
					if ( state.alphaToOneEnable )
					{
						glLogCall( context
							, glEnable
							, GL_SAMPLE_ALPHA_TO_ONE );
					}
					else
					{
						glLogCall( context
							, glDisable
							, GL_SAMPLE_ALPHA_TO_ONE );
					}
				}

				if ( device.getPhysicalDevice().getFeatures().sampleRateShading )
				{
					if ( state.sampleShadingEnable )
					{
						glLogCall( context
							, glEnable
							, GL_SAMPLE_SHADING );
						glLogCall( context
							, glMinSampleShading_ARB
							, state.minSampleShading );
					}
					else
					{
						glLogCall( context
							, glDisable
							, GL_SAMPLE_SHADING );
					}
				}
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_MULTISAMPLE );
			}

			save = state;
		}
	}

	void apply( Device const & device
		, ContextLock const & context
		, ashes::DepthStencilState const & state )
	{
		auto & save = device.getCurrentDepthStencilState();

		if ( state != save )
		{
			if ( state.depthWriteEnable )
			{
				glLogCall( context
					, glDepthMask
					, GL_TRUE );
			}
			else
			{
				glLogCall( context
					, glDepthMask
					, GL_FALSE );
			}

			if ( state.depthTestEnable != save.depthTestEnable )
			{
				if ( state.depthTestEnable )
				{
					glLogCall( context
						, glEnable
						, GL_DEPTH_TEST );
				}
				else
				{
					glLogCall( context
						, glDisable
						, GL_DEPTH_TEST );
				}
			}

			if ( state.depthTestEnable
				&& state.depthCompareOp != save.depthCompareOp )
			{
				glLogCall( context
					, glDepthFunc
					, convert( state.depthCompareOp ) );
			}

			if ( state.stencilTestEnable != save.stencilTestEnable
				|| ( state.stencilTestEnable
					&& ( state.back != save.back
						|| state.front != save.front ) ) )
			{
				if ( state.stencilTestEnable )
				{
					glLogCall( context
						, glEnable
						, GL_STENCIL_TEST );
					glLogCall( context
						, glStencilMaskSeparate
						, GL_CULL_MODE_BACK
						, state.back.writeMask );
					glLogCall( context
						, glStencilFuncSeparate
						, GL_CULL_MODE_BACK
						, convert( state.back.compareOp )
						, state.back.reference
						, state.back.compareMask );
					glLogCall( context
						, glStencilOpSeparate
						, GL_CULL_MODE_BACK
						, convert( state.back.failOp )
						, convert( state.back.depthFailOp )
						, convert( state.back.passOp ) );
					glLogCall( context
						, glStencilMaskSeparate
						, GL_CULL_MODE_FRONT
						, state.front.writeMask );
					glLogCall( context
						, glStencilFuncSeparate
						, GL_CULL_MODE_FRONT
						, convert( state.front.compareOp )
						, state.front.reference
						, state.front.compareMask );
					glLogCall( context
						, glStencilOpSeparate
						, GL_CULL_MODE_FRONT
						, convert( state.front.failOp )
						, convert( state.front.depthFailOp )
						, convert( state.front.passOp ) );
				}
				else
				{
					glLogCall( context
						, glDisable
						, GL_STENCIL_TEST );
				}
			}

			if ( state.depthBoundsTestEnable != save.depthBoundsTestEnable
				|| ( state.depthBoundsTestEnable
					&& ( state.minDepthBounds != save.minDepthBounds
						|| state.maxDepthBounds != save.maxDepthBounds ) ) )
			{
				if ( state.depthBoundsTestEnable )
				{
					glLogCall( context
						, glEnable
						, GL_DEPTH_CLAMP );
					glLogCall( context
						, glDepthRange
						, state.minDepthBounds
						, state.maxDepthBounds );
				}
				else
				{
					glLogCall( context
						, glDisable
						, GL_DEPTH_CLAMP );
				}
			}

			save = state;
		}
	}

	void apply( Device const & device
		, ContextLock const & context
		, ashes::TessellationState const & state )
	{
		auto & save = device.getCurrentTessellationState();

		if ( state != save )
		{
			if ( state.patchControlPoints )
			{
				glLogCall( context
					, glPatchParameteri_ARB
					, GL_PATCH_VERTICES
					, int( state.patchControlPoints ) );
			}

			save = state;
		}
	}

	void apply( Device const & device
		, ContextLock const & context
		, ashes::Viewport const & state )
	{
		auto & save = device.getCurrentViewport();

		if ( state != save )
		{
			glLogCall( context
				, glViewport
				, state.offset.x
				, state.offset.y
				, state.size.width
				, state.size.height );
			glLogCall( context
				, glDepthRange
				, state.minDepth
				, state.maxDepth );
			save = state;
		}
	}

	void apply( Device const & device
		, ContextLock const & context
		, ashes::Scissor const & state )
	{
		auto & save = device.getCurrentScissor();

		if ( state != save )
		{
			glLogCall( context
				, glScissor
				, state.offset.x
				, state.offset.y
				, state.size.width
				, state.size.height );
			save = state;
		}
	}

	BindPipelineCommand::BindPipelineCommand( Device const & device
		, ashes::Pipeline const & pipeline
		, ashes::PipelineBindPoint bindingPoint )
		: CommandBase{ device }
		, m_pipeline{ static_cast< Pipeline const & > ( pipeline ) }
		, m_layout{ static_cast< PipelineLayout const & > ( m_pipeline.getLayout() ) }
		, m_program{ m_pipeline.getProgram() }
		, m_bindingPoint{ bindingPoint }
		, m_dynamicLineWidth{ m_pipeline.hasDynamicState( ashes::DynamicState::eLineWidth ) }
		, m_dynamicDepthBias{ m_pipeline.hasDynamicState( ashes::DynamicState::eDepthBias ) }
		, m_dynamicScissor{ m_pipeline.hasDynamicState( ashes::DynamicState::eScissor ) }
		, m_dynamicViewport{ m_pipeline.hasDynamicState( ashes::DynamicState::eViewport ) }
	{
	}

	void BindPipelineCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "BindPipelineCommand" );
		gl_renderer::apply( m_device, context, m_pipeline.getInputAssemblyState() );
		gl_renderer::apply( m_device, context, m_pipeline.getColourBlendState() );
		gl_renderer::apply( m_device, context, m_pipeline.getRasterisationState()
			, m_dynamicLineWidth
			, m_dynamicDepthBias );
		gl_renderer::apply( m_device, context, m_pipeline.getDepthStencilState() );
		gl_renderer::apply( m_device, context, m_pipeline.getMultisampleState() );
		gl_renderer::apply( m_device, context, m_pipeline.getTessellationState() );

		if ( !m_dynamicViewport )
		{
			assert( m_pipeline.hasViewport() );
			gl_renderer::apply( m_device, context, m_pipeline.getViewport() );
		}

		if ( !m_dynamicScissor )
		{
			assert( m_pipeline.hasScissor() );
			gl_renderer::apply( m_device, context, m_pipeline.getScissor() );
		}

		auto & save = m_device.getCurrentProgram();

		//if ( m_program != save )
		{
			glLogCall( context, glUseProgram, m_program );
			save = m_program;
		}
	}

	CommandPtr BindPipelineCommand::clone()const
	{
		return std::make_unique< BindPipelineCommand >( *this );
	}
}
