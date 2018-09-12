#include "Pipeline/D3D11ComputePipeline.hpp"

#include "Core/D3D11Device.hpp"
#include "Shader/D3D11ShaderModule.hpp"

#include <Buffer/PushConstantsBuffer.hpp>
#include <Pipeline/SpecialisationInfo.hpp>

namespace d3d11_renderer
{
	ComputePipeline::ComputePipeline( Device const & device
		, renderer::PipelineLayout const & layout
		, renderer::ComputePipelineCreateInfo && createInfo )
		: renderer::ComputePipeline{ device
			, layout
			, std::move( createInfo ) }
	{
		doCompileProgram( device );
	}

	ComputePipeline::~ComputePipeline()
	{
	}

	void ComputePipeline::doCompileProgram( Device const & device )
	{
		if ( m_createInfo.stage.specialisationInfo )
		{
			m_constantsPcbs.push_back( convert( device
				, ~( 0u )
				, m_createInfo.stage.module->getStage()
				, *m_createInfo.stage.specialisationInfo ) );
		}

		auto module = std::static_pointer_cast< ShaderModule >( m_createInfo.stage.module );
		module->compile( m_createInfo.stage );
	}
}
