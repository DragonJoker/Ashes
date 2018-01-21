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
				glLogicOp( convert( state.getLogicOp() ) );
			}

			auto & blendConstants = state.getBlendConstants();
			glBlendColor( blendConstants.r, blendConstants.g, blendConstants.b, blendConstants.a );
			bool blend = false;
			GLuint buf = 0u;

			for ( auto & blendState : state )
			{
				if ( blendState.isBlendEnabled() )
				{
					blend = true;
					glBlendEquationSeparatei( buf
						, convert( blendState.getColourBlendOp() )
						, convert( blendState.getAlphaBlendOp() ) );
					glBlendFuncSeparatei( buf
						, convert( blendState.getSrcColourBlendFactor() )
						, convert( blendState.getDstColourBlendFactor() )
						, convert( blendState.getSrcAlphaBlendFactor() )
						, convert( blendState.getDstAlphaBlendFactor() ) );
				}

				++buf;
			}

			if ( blend )
			{
				glEnable( GL_BLEND );
			}
			else
			{
				glDisable( GL_BLEND );
			}
		}

		void doApply( renderer::RasterisationState const & state )
		{
			if ( state.getCullMode() != renderer::CullModeFlag::eNone )
			{
				glEnable( GL_CULL_FACE );
				glCullFace( convert( state.getCullMode() ) );
				glFrontFace( convert( state.getFrontFace() ) );
			}
			else
			{
				glDisable( GL_CULL_FACE );
			}

			glPolygonMode( GL_FRONT_AND_BACK, convert( state.getPolygonMode() ) );

			if ( state.isDepthBiasEnabled() )
			{
				switch ( state.getPolygonMode() )
				{
				case renderer::PolygonMode::eFill:
					glEnable( GL_POLYGON_OFFSET_FILL );
					break;

				case renderer::PolygonMode::eLine:
					glEnable( GL_POLYGON_OFFSET_LINE );
					break;

				case renderer::PolygonMode::ePoint:
					glEnable( GL_POLYGON_OFFSET_POINT );
					break;
				}

				glPolygonOffsetClampEXT( state.getDepthBiasConstantFactor()
					, state.getDepthBiasSlopeFactor()
					, state.getDepthBiasClamp() );
			}
			else
			{
				switch ( state.getPolygonMode() )
				{
				case renderer::PolygonMode::eFill:
					glDisable( GL_POLYGON_OFFSET_FILL );
					break;

				case renderer::PolygonMode::eLine:
					glDisable( GL_POLYGON_OFFSET_LINE );
					break;

				case renderer::PolygonMode::ePoint:
					glDisable( GL_POLYGON_OFFSET_POINT );
					break;
				}
			}

			if ( state.isDepthClampEnabled() )
			{
				glEnable( GL_DEPTH_CLAMP );
			}
			else
			{
				glDisable( GL_DEPTH_CLAMP );
			}

			if ( state.isRasteriserDiscardEnabled() )
			{
				glEnable( GL_RASTERIZER_DISCARD );
			}
			else
			{
				glDisable( GL_RASTERIZER_DISCARD );
			}

			if ( state.getPolygonMode() == renderer::PolygonMode::eLine )
			{
				glLineWidth( state.getLineWidth() );
			}
		}

		void doApply( renderer::MultisampleState const & state )
		{
			if ( state.getRasterisationSamples() != renderer::SampleCountFlag::e1 )
			{
				glEnable( GL_MULTISAMPLE );

				if ( state.isAlphaToCoverageEnabled() )
				{
					glEnable( GL_SAMPLE_ALPHA_TO_COVERAGE );
				}
				else
				{
					glDisable( GL_SAMPLE_ALPHA_TO_COVERAGE );
				}

				if ( state.isAlphaToOneEnabled() )
				{
					glEnable( GL_SAMPLE_ALPHA_TO_ONE );
				}
				else
				{
					glDisable( GL_SAMPLE_ALPHA_TO_ONE );
				}
			}
			else
			{
				glDisable( GL_MULTISAMPLE );
			}
		}

		void doApply( renderer::DepthStencilState const & state )
		{
			if ( state.isDepthWriteEnabled() )
			{
				glDepthMask( GL_TRUE );
			}
			else
			{
				glDepthMask( GL_FALSE );
			}

			if ( state.isDepthTestEnabled() )
			{
				glEnable( GL_DEPTH_TEST );
				glDepthFunc( convert( state.getDepthCompareOp() ) );
			}
			else
			{
				glDisable( GL_DEPTH_TEST );
			}

			if ( state.isStencilTestEnabled() )
			{
				glEnable( GL_STENCIL_TEST );

				glStencilMaskSeparate( GL_BACK
					, state.getBackStencilOp().getWriteMask() );
				glStencilFuncSeparate( GL_BACK
					, convert( state.getBackStencilOp().getCompareOp() )
					, state.getBackStencilOp().getReference()
					, state.getBackStencilOp().getCompareMask() );
				glStencilOpSeparate( GL_BACK
					, convert( state.getBackStencilOp().getFailOp() )
					, convert( state.getBackStencilOp().getDepthFailOp() )
					, convert( state.getBackStencilOp().getPassOp() ) );
				glStencilMaskSeparate( GL_FRONT
					, state.getFrontStencilOp().getWriteMask() );
				glStencilFuncSeparate( GL_FRONT
					, convert( state.getFrontStencilOp().getCompareOp() )
					, state.getFrontStencilOp().getReference()
					, state.getFrontStencilOp().getCompareMask() );
				glStencilOpSeparate( GL_FRONT
					, convert( state.getFrontStencilOp().getFailOp() )
					, convert( state.getFrontStencilOp().getDepthFailOp() )
					, convert( state.getFrontStencilOp().getPassOp() ) );
			}
			else
			{
				glDisable( GL_STENCIL_TEST );
			}

			if ( state.isDepthBoundsTestEnabled() )
			{
				glEnable( GL_DEPTH_CLAMP );
				glDepthRange( state.getMinDepthBounds(), state.getMaxDepthBounds() );
			}
			else
			{
				glDisable( GL_DEPTH_CLAMP );
			}
		}

		void doApply( renderer::TessellationState const & state )
		{
			if ( state.getControlPoints() )
			{
				glPatchParameteri( GL_PATCH_VERTICES, int( state.getControlPoints() ) );
			}
		}

		void doApply( renderer::Viewport const & state )
		{
			glViewport( state.getOffset().x
				, state.getOffset().y
				, state.getSize().x
				, state.getSize().y );
		}

		void doApply( renderer::Scissor const & state )
		{
			glScissor( state.getOffset().x
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

		glUseProgram( m_program.getProgram() );
	}

	CommandPtr BindPipelineCommand::clone()const
	{
		return std::make_unique< BindPipelineCommand >( *this );
	}
}
