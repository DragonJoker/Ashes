#include "Pipeline/GlComputePipeline.hpp"

#include "Core/GlDevice.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "Shader/GlShaderProgram.hpp"

namespace gl_renderer
{
	ComputePipeline::ComputePipeline( Device const & device
		, renderer::PipelineLayout const & layout
		, renderer::ComputePipelineCreateInfo && createInfo )
		: renderer::ComputePipeline{ device
			, layout
			, std::move( createInfo ) }
		, m_device{ device }
		, m_layout{ layout }
		, m_program{ static_cast< ShaderProgram const & >( m_createInfo.program.get() ) }
	{
		m_program.link();

		if ( !m_createInfo.specialisationInfo.empty() )
		{
			for ( auto & info : m_createInfo.specialisationInfo )
			{
				m_constantsPcbs.push_back( convert( info.first, *info.second ) );
			}
		}
	}
}
