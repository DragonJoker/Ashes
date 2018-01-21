/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "GlBindPipelineCommand.hpp"

#include "Pipeline/GlPipeline.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "Shader/GlShaderProgram.hpp"

namespace gl_renderer
{
	namespace
	{
		void doApply( renderer::ColourBlendState const & state )
		{
			if ( state.isLogicOpEnabled() )
			{
				glLogCall( glLogicOp, convert( state.getLogicOp() ) );
			}

			auto & blendConstants = state.getBlendConstants();
			glLogCall( glBlendColor, blendConstants.r, blendConstants.g, blendConstants.b, blendConstants.a );
			bool blend = false;
			GLuint buf = 0u;

			for ( auto & blendState : state )
			{
				if ( blendState.isBlendEnabled() )
				{
					blend = true;
					glLogCall( glBlendEquationSeparatei
						, buf
						, convert( blendState.getColourBlendOp() )
						, convert( blendState.getAlphaBlendOp() ) );
					glLogCall( glBlendFuncSeparatei
						, buf
						, convert( blendState.getSrcColourBlendFactor() )
						, convert( blendState.getDstColourBlendFactor() )
						, convert( blendState.getSrcAlphaBlendFactor() )
						, convert( blendState.getDstAlphaBlendFactor() ) );
				}

				++buf;
			}

			if ( blend )
			{
				glLogCall( glEnable, GL_BLEND );
			}
			else
			{
				glLogCall( glDisable, GL_BLEND );
			}
		}

		void doApply( renderer::RasterisationState const & state )
		{
			if ( state.getCullMode() != renderer::CullModeFlag::eNone )
			{
				glLogCall( glEnable, GL_CULL_FACE );
				glLogCall( glCullFace, convert( state.getCullMode() ) );
				glLogCall( glFrontFace, convert( state.getFrontFace() ) );
			}
			else
			{
				glLogCall( glDisable, GL_CULL_FACE );
			}

			glLogCall( glPolygonMode, GL_FRONT_AND_BACK, convert( state.getPolygonMode() ) );

			if ( state.isDepthBiasEnabled() )
			{
				switch ( state.getPolygonMode() )
				{
				case renderer::PolygonMode::eFill:
					glLogCall( glEnable, GL_POLYGON_OFFSET_FILL );
					break;

				case renderer::PolygonMode::eLine:
					glLogCall( glEnable, GL_POLYGON_OFFSET_LINE );
					break;

				case renderer::PolygonMode::ePoint:
					glLogCall( glEnable, GL_POLYGON_OFFSET_POINT );
					break;
				}

				glLogCall( glPolygonOffsetClampEXT, state.getDepthBiasConstantFactor()
					, state.getDepthBiasSlopeFactor()
					, state.getDepthBiasClamp() );
			}
			else
			{
				switch ( state.getPolygonMode() )
				{
				case renderer::PolygonMode::eFill:
					glLogCall( glDisable, GL_POLYGON_OFFSET_FILL );
					break;

				case renderer::PolygonMode::eLine:
					glLogCall( glDisable, GL_POLYGON_OFFSET_LINE );
					break;

				case renderer::PolygonMode::ePoint:
					glLogCall( glDisable, GL_POLYGON_OFFSET_POINT );
					break;
				}
			}

			if ( state.isDepthClampEnabled() )
			{
				glLogCall( glEnable, GL_DEPTH_CLAMP );
			}
			else
			{
				glLogCall( glDisable, GL_DEPTH_CLAMP );
			}

			if ( state.isRasteriserDiscardEnabled() )
			{
				glLogCall( glEnable, GL_RASTERIZER_DISCARD );
			}
			else
			{
				glLogCall( glDisable, GL_RASTERIZER_DISCARD );
			}

			if ( state.getPolygonMode() == renderer::PolygonMode::eLine )
			{
				glLogCall( glLineWidth, state.getLineWidth() );
			}
		}

		void doApply( renderer::MultisampleState const & state )
		{
			if ( state.getRasterisationSamples() != renderer::SampleCountFlag::e1 )
			{
				glLogCall( glEnable, GL_MULTISAMPLE );

				if ( state.isAlphaToCoverageEnabled() )
				{
					glLogCall( glEnable, GL_SAMPLE_ALPHA_TO_COVERAGE );
				}
				else
				{
					glLogCall( glDisable, GL_SAMPLE_ALPHA_TO_COVERAGE );
				}

				if ( state.isAlphaToOneEnabled() )
				{
					glLogCall( glEnable, GL_SAMPLE_ALPHA_TO_ONE );
				}
				else
				{
					glLogCall( glDisable, GL_SAMPLE_ALPHA_TO_ONE );
				}
			}
			else
			{
				glLogCall( glDisable, GL_MULTISAMPLE );
			}
		}

		void doApply( renderer::DepthStencilState const & state )
		{
			if ( state.isDepthWriteEnabled() )
			{
				glLogCall( glDepthMask, GL_TRUE );
			}
			else
			{
				glLogCall( glDepthMask, GL_FALSE );
			}

			if ( state.isDepthTestEnabled() )
			{
				glLogCall( glEnable, GL_DEPTH_TEST );
				glLogCall( glDepthFunc, convert( state.getDepthCompareOp() ) );
			}
			else
			{
				glLogCall( glDisable, GL_DEPTH_TEST );
			}

			if ( state.isStencilTestEnabled() )
			{
				glLogCall( glEnable, GL_STENCIL_TEST );

				glLogCall( glStencilMaskSeparate
					, GL_BACK
					, state.getBackStencilOp().getWriteMask() );
				glLogCall( glStencilFuncSeparate
					, GL_BACK
					, convert( state.getBackStencilOp().getCompareOp() )
					, state.getBackStencilOp().getReference()
					, state.getBackStencilOp().getCompareMask() );
				glLogCall( glStencilOpSeparate
					, GL_BACK
					, convert( state.getBackStencilOp().getFailOp() )
					, convert( state.getBackStencilOp().getDepthFailOp() )
					, convert( state.getBackStencilOp().getPassOp() ) );
				glLogCall( glStencilMaskSeparate
					, GL_FRONT
					, state.getFrontStencilOp().getWriteMask() );
				glLogCall( glStencilFuncSeparate
					, GL_FRONT
					, convert( state.getFrontStencilOp().getCompareOp() )
					, state.getFrontStencilOp().getReference()
					, state.getFrontStencilOp().getCompareMask() );
				glLogCall( glStencilOpSeparate
					, GL_FRONT
					, convert( state.getFrontStencilOp().getFailOp() )
					, convert( state.getFrontStencilOp().getDepthFailOp() )
					, convert( state.getFrontStencilOp().getPassOp() ) );
			}
			else
			{
				glLogCall( glDisable, GL_STENCIL_TEST );
			}

			if ( state.isDepthBoundsTestEnabled() )
			{
				glLogCall( glEnable, GL_DEPTH_CLAMP );
				glLogCall( glDepthRange, state.getMinDepthBounds(), state.getMaxDepthBounds() );
			}
			else
			{
				glLogCall( glDisable, GL_DEPTH_CLAMP );
			}
		}

		void doApply( renderer::TessellationState const & state )
		{
			if ( state.getControlPoints() )
			{
				glLogCall( glPatchParameteri, GL_PATCH_VERTICES, int( state.getControlPoints() ) );
			}
		}

		void doApply( renderer::Viewport const & state )
		{
			glLogCall( glViewport
				, state.getOffset().x
				, state.getOffset().y
				, state.getSize().x
				, state.getSize().y );
		}

		void doApply( renderer::Scissor const & state )
		{
			glLogCall( glScissor
				, state.getOffset().x
				, state.getOffset().y
				, state.getSize().x
				, state.getSize().y );
		}
	}

	BindPipelineCommand::BindPipelineCommand( renderer::Pipeline const & pipeline
		, renderer::PipelineBindPoint bindingPoint )
		: m_pipeline{ static_cast< Pipeline const & > ( pipeline ) }
		, m_layout{ static_cast< PipelineLayout const & > ( m_pipeline.getLayout() ) }
		, m_program{ static_cast< ShaderProgram const & > ( m_pipeline.getProgram() ) }
		, m_bindingPoint{ bindingPoint }
	{
	}

	void BindPipelineCommand::apply()const
	{
		doApply( m_pipeline.getColourBlendState() );
		doApply( m_pipeline.getRasterisationState() );
		doApply( m_pipeline.getDepthStencilState() );
		doApply( m_pipeline.getMultisampleState() );
		doApply( m_pipeline.getTessellationState() );

		if ( m_pipeline.hasViewport() )
		{
			doApply( m_pipeline.getViewport() );
		}

		if ( m_pipeline.hasScissor() )
		{
			doApply( m_pipeline.getScissor() );
		}

		glLogCall( glUseProgram, m_program.getProgram() );
	}

	CommandPtr BindPipelineCommand::clone()const
	{
		return std::make_unique< BindPipelineCommand >( *this );
	}
}
