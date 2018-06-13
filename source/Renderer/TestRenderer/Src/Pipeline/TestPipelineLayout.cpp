#include "Pipeline/TestPipelineLayout.hpp"

#include "Core/TestDevice.hpp"
#include "Pipeline/TestComputePipeline.hpp"
#include "Pipeline/TestPipeline.hpp"

namespace test_renderer
{
	PipelineLayout::PipelineLayout( Device const & device
		, renderer::DescriptorSetLayoutCRefArray const & setLayouts
		, renderer::PushConstantRangeCRefArray const & pushConstantRanges )
		: renderer::PipelineLayout{ device, setLayouts, pushConstantRanges }
		, m_device{ device }
	{
	}

	PipelineLayout::~PipelineLayout()
	{
	}

	renderer::PipelinePtr PipelineLayout::createPipeline( renderer::GraphicsPipelineCreateInfo createInfo )const
	{
		return std::make_unique< Pipeline >( m_device
			, *this
			, std::move( createInfo ) );
	}

	renderer::ComputePipelinePtr PipelineLayout::createPipeline( renderer::ComputePipelineCreateInfo createInfo )const
	{
		return std::make_unique< ComputePipeline >( m_device
			, *this
			, std::move( createInfo ) );
	}
}
