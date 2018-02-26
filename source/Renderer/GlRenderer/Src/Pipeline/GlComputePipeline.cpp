#include "Pipeline/GlComputePipeline.hpp"

#include "Core/GlDevice.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "Shader/GlShaderProgram.hpp"

namespace gl_renderer
{
	namespace
	{
		renderer::PushConstantArray convert( renderer::ShaderStageFlag stage
			, std::vector< renderer::SpecialisationMapEntry > const & entries )
		{
			renderer::PushConstantArray result;

			for ( auto & entry : entries )
			{
				result.push_back( {
					entry.constantID,
					entry.offset,
					entry.format,
					entry.arraySize
				} );
			}

			return result;
		}

		renderer::PushConstantsBufferBase convert( renderer::ShaderStageFlag stage
			, renderer::SpecialisationInfo const & specialisationInfo )
		{
			renderer::PushConstantsBufferBase result{ stage, convert( stage, specialisationInfo.mapEntries ) };
			assert( result.getSize() == specialisationInfo.data.size() );
			std::memcpy( result.getData(), specialisationInfo.data.data(), specialisationInfo.data.size() );
			return result;
		}
	}

	ComputePipeline::ComputePipeline( Device const & device
		, renderer::PipelineLayout const & layout
		, renderer::ComputePipelineCreateInfo const & createInfo )
		: renderer::ComputePipeline{ device
			, layout
			, createInfo }
		, m_device{ device }
		, m_layout{ layout }
		, m_program{ static_cast< ShaderProgram const & >( createInfo.program.get() ) }
	{
		m_program.link();

		if ( !m_createInfo.specialisationInfo.empty() )
		{
			for ( auto & info : m_createInfo.specialisationInfo )
			{
				m_constantsPcbs.push_back( convert( info.first, info.second ) );
			}
		}
	}
}
