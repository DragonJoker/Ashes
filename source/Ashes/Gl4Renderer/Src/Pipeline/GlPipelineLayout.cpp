#include "Pipeline/GlPipelineLayout.hpp"

#include "Core/GlDevice.hpp"
#include "Descriptor/GlDescriptorSetLayout.hpp"
#include "Pipeline/GlComputePipeline.hpp"
#include "Pipeline/GlPipeline.hpp"

namespace gl_renderer
{
	PipelineLayout::PipelineLayout( Device const & device
		, ashes::DescriptorSetLayoutCRefArray const & setLayouts
		, ashes::PushConstantRangeArray const & pushConstantRanges )
		: ashes::PipelineLayout{ device, setLayouts, pushConstantRanges }
		, m_device{ device }
	{
	}

	ashes::PipelinePtr PipelineLayout::createPipeline( ashes::GraphicsPipelineCreateInfo createInfo )const
	{
		return std::make_unique< Pipeline >( m_device
			, *this
			, std::move( createInfo ) );
	}

	ashes::ComputePipelinePtr PipelineLayout::createPipeline( ashes::ComputePipelineCreateInfo createInfo )const
	{
		return std::make_unique< ComputePipeline >( m_device
			, *this
			, std::move( createInfo ) );
	}
}
