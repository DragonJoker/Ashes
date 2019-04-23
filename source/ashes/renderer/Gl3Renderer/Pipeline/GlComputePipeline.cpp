#include "Pipeline/GlComputePipeline.hpp"

#include "Core/GlDevice.hpp"
#include "Pipeline/GlPipelineLayout.hpp"
#include "Shader/GlShaderModule.hpp"

namespace gl_renderer
{
	ComputePipeline::ComputePipeline( Device const & device
		, ashes::PipelineLayout const & layout
		, ashes::ComputePipelineCreateInfo createInfo )
		: ashes::ComputePipeline{ device
			, layout
			, std::move( createInfo ) }
		, m_device{ device }
		, m_layout{ layout }
		, m_program{ m_device, m_createInfo.stage }
	{
		ShaderDesc shaderDesc = m_program.link();
		m_constantsPcb.stageFlags = shaderDesc.stageFlags;
		uint32_t offset = 0u;

		for ( auto & constant : shaderDesc.constantsLayout )
		{
			m_constantsPcb.constants.push_back( { constant.format
				, constant.location
				, offset
				, constant.size
				, constant.arraySize } );
			offset += constant.size;
		}

		m_constantsPcb.size = offset;

		//if ( m_createInfo.stage.specialisationInfo )
		//{
		//	m_constantsPcbs.push_back( convert( device
		//		, ~( 0u )
		//		, m_createInfo.stage.module->getStage()
		//		, *m_createInfo.stage.specialisationInfo ) );
		//}
	}

	PushConstantsDesc ComputePipeline::findPushConstantBuffer( PushConstantsDesc const & pushConstants )const
	{
		PushConstantsDesc result{ m_constantsPcb };
		result.offset = pushConstants.offset;
		result.size = pushConstants.size;
		result.data = pushConstants.data;
		return result;
	}
}
