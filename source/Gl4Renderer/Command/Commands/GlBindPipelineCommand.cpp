/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlBindPipelineCommand.hpp"

#include "Core/GlDevice.hpp"
#include "Pipeline/GlPipeline.hpp"
#include "Pipeline/GlPipelineLayout.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	bool operator==( VkStencilOpState const & lhs
		, VkStencilOpState const & rhs )
	{
		return lhs.failOp == rhs.failOp
			&& lhs.passOp == rhs.passOp
			&& lhs.depthFailOp == rhs.depthFailOp
			&& lhs.compareOp == rhs.compareOp
			&& lhs.compareMask == rhs.compareMask
			&& lhs.writeMask == rhs.writeMask
			&& lhs.reference == rhs.reference;
	}

	bool operator!=( VkStencilOpState const & lhs
		, VkStencilOpState const & rhs )
	{
		return !( lhs == rhs );
	}

	void apply( VkDevice device
		, ContextLock const & context
		, VkPipelineInputAssemblyStateCreateInfo const & state )
	{
		auto & save = get( device )->getCurrentInputAssemblyState();

		if ( state.topology != save.topology )
		{
			if ( state.topology == VK_PRIMITIVE_TOPOLOGY_POINT_LIST )
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

	void apply( VkDevice device
		, ContextLock const & context
		, VkPipelineColorBlendStateCreateInfo const & state )
	{
		auto & save = get( device )->getCurrentBlendState();
		static bool hadBlend = false;

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

		for ( auto it = state.pAttachments;
			it != state.pAttachments + state.attachmentCount;
			++it )
		{
			auto & blendState = *it;

			if ( blendState.blendEnable )
			{
				blend = true;
				glLogCall( context
					, glBlendEquationSeparatei
					, buf
					, convert( blendState.colorBlendOp )
					, convert( blendState.alphaBlendOp ) );
				glLogCall( context
					, glBlendFuncSeparatei
					, buf
					, convert( blendState.srcColorBlendFactor )
					, convert( blendState.dstColorBlendFactor )
					, convert( blendState.srcAlphaBlendFactor )
					, convert( blendState.dstAlphaBlendFactor ) );
			}

			++buf;
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

	void apply( VkDevice device
		, ContextLock const & context
		, VkPipelineRasterizationStateCreateInfo const & state
		, bool dynamicLineWidth
		, bool dynamicDepthBias )
	{
		auto & save = get( device )->getCurrentRasterisationState();

		if ( state.cullMode != save.cullMode
			|| state.frontFace != save.frontFace )
		{
			if ( state.cullMode != VK_CULL_MODE_NONE )
			{
				if ( save.cullMode == VK_CULL_MODE_NONE )
				{
					glLogCall( context
						, glEnable
						, GL_CULL_FACE );
				}

				if ( state.cullMode != save.cullMode )
				{
					glLogCall( context
						, glCullFace
						, convertCullModeFlags( state.cullMode ) );
				}

				if ( state.frontFace != save.frontFace )
				{
					glLogCall( context
						, glFrontFace
						, convert( state.frontFace ) );
				}
			}
			else if ( save.cullMode != VK_CULL_MODE_NONE )
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
				case VK_POLYGON_MODE_FILL:
					glLogCall( context
						, glEnable
						, GL_POLYGON_OFFSET_FILL );
					break;

				case VK_POLYGON_MODE_LINE:
					glLogCall( context
						, glEnable
						, GL_POLYGON_OFFSET_LINE );
					break;

				case VK_POLYGON_MODE_POINT:
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
				case VK_POLYGON_MODE_FILL:
					glLogCall( context
						, glDisable
						, GL_POLYGON_OFFSET_FILL );
					break;

				case VK_POLYGON_MODE_LINE:
					glLogCall( context
						, glDisable
						, GL_POLYGON_OFFSET_LINE );
					break;

				case VK_POLYGON_MODE_POINT:
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

		if ( state.rasterizerDiscardEnable != save.rasterizerDiscardEnable )
		{
			if ( state.rasterizerDiscardEnable )
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

		if ( state.polygonMode == VK_POLYGON_MODE_LINE
			&& !dynamicLineWidth )
		{
			glLogCall( context
				, glLineWidth
				, state.lineWidth );
		}

		save = state;
	}

	void apply( VkDevice device
		, ContextLock const & context
		, VkPipelineMultisampleStateCreateInfo const & state )
	{
		auto & save = get( device )->getCurrentMultisampleState();

		if ( state.rasterizationSamples != VK_SAMPLE_COUNT_1_BIT )
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

			if ( state.sampleShadingEnable )
			{
				glLogCall( context
					, glEnable
					, GL_SAMPLE_SHADING );
				glLogCall( context
					, glMinSampleShading
					, state.minSampleShading );
			}
			else
			{
				glLogCall( context
					, glDisable
					, GL_SAMPLE_SHADING );
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

	void apply( VkDevice device
		, ContextLock const & context
		, VkPipelineDepthStencilStateCreateInfo const & state )
	{
		auto & save = get( device )->getCurrentDepthStencilState();

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

	void apply( VkDevice device
		, ContextLock const & context
		, VkPipelineTessellationStateCreateInfo const & state )
	{
		auto & save = get( device )->getCurrentTessellationState();

		if ( state.patchControlPoints )
		{
			glLogCall( context
				, glPatchParameteri
				, GL_PATCH_VERTICES
				, int( state.patchControlPoints ) );
		}

		save = state;
	}

	void apply( VkDevice device
		, ContextLock const & context
		, VkViewportArray const & states )
	{
		auto & save = get( device )->getCurrentViewport();
		auto & state = *states.begin();

		glLogCall( context
			, glViewport
			, GLsizei( state.x )
			, GLsizei( state.y )
			, GLint( state.width )
			, GLint( state.height ) );
		glLogCall( context
			, glDepthRange
			, state.minDepth
			, state.maxDepth );
		save = state;
	}

	void apply( VkDevice device
		, ContextLock const & context
		, VkScissorArray const & states )
	{
		auto & save = get( device )->getCurrentScissor();
		auto & state = *states.begin();

		glLogCall( context
			, glScissor
			, state.offset.x
			, state.offset.y
			, state.extent.width
			, state.extent.height );
		save = state;
	}

	BindPipelineCommand::BindPipelineCommand( VkDevice device
		, VkPipeline pipeline
		, VkPipelineBindPoint bindingPoint )
		: CommandBase{ device }
		, m_pipeline{ pipeline }
		, m_layout{ get( m_pipeline )->getLayout() }
		, m_program{ get( m_pipeline )->getProgram() }
		, m_bindingPoint{ bindingPoint }
		, m_dynamicLineWidth{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_LINE_WIDTH ) }
		, m_dynamicDepthBias{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_DEPTH_BIAS ) }
		, m_dynamicScissor{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_SCISSOR ) }
		, m_dynamicViewport{ get( m_pipeline )->hasDynamicStateEnable( VK_DYNAMIC_STATE_VIEWPORT ) }
	{
	}

	void BindPipelineCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "BindPipelineCommand" );
		auto pipeline = get( m_pipeline );

		if ( pipeline->hasInputAssemblyState() )
		{
			gl4::apply( m_device
				, context
				, pipeline->getInputAssemblyState() );
		}

		if ( pipeline->hasColourBlendState() )
		{
			gl4::apply( m_device
				, context
				, pipeline->getColourBlendState() );
		}

		if ( pipeline->hasRasterisationState() )
		{
			gl4::apply( m_device
				, context
				, pipeline->getRasterisationState()
				, m_dynamicLineWidth
				, m_dynamicDepthBias );
		}

		if ( pipeline->hasDepthStencilState() )
		{
			gl4::apply( m_device
				, context
				, pipeline->getDepthStencilState() );
		}

		if ( pipeline->hasMultisampleState() )
		{
			gl4::apply( m_device
				, context
				, pipeline->getMultisampleState() );
		}

		if ( pipeline->hasTessellationState() )
		{
			gl4::apply( m_device
				, context
				, pipeline->getTessellationState() );
		}

		if ( !m_dynamicViewport )
		{
			assert( get( m_pipeline )->hasViewport() );
			gl4::apply( m_device
				, context
				, get( m_pipeline )->getViewports() );
		}

		if ( !m_dynamicScissor )
		{
			assert( get( m_pipeline )->hasScissor() );
			gl4::apply( m_device
				, context
				, get( m_pipeline )->getScissors() );
		}

		auto & save = get( m_device )->getCurrentProgram();

		//if ( m_program != save )
		{
			glLogCall( context
				, glUseProgram
				, m_program );
			save = m_program;
		}
	}

	CommandPtr BindPipelineCommand::clone()const
	{
		return std::make_unique< BindPipelineCommand >( *this );
	}
}
