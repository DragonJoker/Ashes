#include "Pipeline/GlPipelineLayout.hpp"

#include "Core/GlDevice.hpp"
#include "Descriptor/GlDescriptorSetLayout.hpp"
#include "Pipeline/GlComputePipeline.hpp"
#include "Pipeline/GlPipeline.hpp"

namespace gl_renderer
{
	PipelineLayout::PipelineLayout( Device const & device
		, renderer::DescriptorSetLayoutCRefArray const & setLayouts
		, renderer::PushConstantRangeCRefArray const & pushConstantRanges )
		: renderer::PipelineLayout{ device, setLayouts, pushConstantRanges }
		, m_device{ device }
	{
	}

	renderer::PipelinePtr PipelineLayout::createPipeline( renderer::GraphicsPipelineCreateInfo const & createInfo )const
	{
		return std::make_unique< Pipeline >( m_device
			, *this
			, createInfo );
	}

	renderer::ComputePipelinePtr PipelineLayout::createPipeline( renderer::ComputePipelineCreateInfo const & createInfo )const
	{
		return std::make_unique< ComputePipeline >( m_device
			, *this
			, createInfo );
	}
}
