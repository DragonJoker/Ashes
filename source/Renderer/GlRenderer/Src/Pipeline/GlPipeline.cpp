#include "Pipeline/GlPipeline.hpp"

#include "Command/Commands/GlBindPipelineCommand.hpp"
#include "Core/GlDevice.hpp"
#include "Core/GlRenderer.hpp"
#include "Core/GlRenderingResources.hpp"
#include "Miscellaneous/GlValidator.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "RenderPass/GlRenderPass.hpp"
#include "Shader/GlShaderProgram.hpp"

#if defined( interface )
#	undef interface
#endif

namespace gl_renderer
{
	Pipeline::Pipeline( Device const & device
		, PipelineLayout const & layout
		, renderer::GraphicsPipelineCreateInfo && createInfo )
		: renderer::Pipeline{ device
			, layout
			, std::move( createInfo ) }
		, m_device{ device }
		, m_layout{ layout }
		, m_program{ static_cast< ShaderProgram const & >( m_createInfo.program.get() ) }
		, m_vertexInputState{ m_createInfo.vertexInputState }
		, m_renderPass{ m_createInfo.renderPass.get() }
		, m_iaState{ m_createInfo.inputAssemblyState }
		, m_cbState{ m_createInfo.colourBlendState }
		, m_rsState{ m_createInfo.rasterisationState }
		, m_dsState{ 0u, false, true, renderer::CompareOp::eLess,  }
		, m_viewport{ m_createInfo.viewport }
		, m_scissor{ m_createInfo.scissor }
	{
		if ( m_createInfo.depthStencilState )
		{
			m_dsState = m_createInfo.depthStencilState.value();
		}

		if ( m_createInfo.tessellationState )
		{
			m_tsState = m_createInfo.tessellationState.value();
		}

		if ( !m_createInfo.specialisationInfo.empty() )
		{
			for ( auto & info : m_createInfo.specialisationInfo )
			{
				m_constantsPcbs.push_back( convert( info.first, *info.second ) );
			}
		}

		apply( m_device, m_cbState );
		apply( m_device, m_rsState );
		apply( m_device, m_dsState );
		apply( m_device, m_msState );
		apply( m_device, m_tsState );
		m_program.link();

		if ( m_device.getRenderer().isValidationEnabled() )
		{
			validatePipeline( m_layout
				, m_program
				, m_vertexInputState
				, m_renderPass );
		}
	}
}
