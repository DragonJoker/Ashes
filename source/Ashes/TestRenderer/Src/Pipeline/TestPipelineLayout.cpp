#include "Pipeline/TestPipelineLayout.hpp"

#include "Core/TestDevice.hpp"
#include "Pipeline/TestComputePipeline.hpp"
#include "Pipeline/TestPipeline.hpp"

namespace test_renderer
{
	PipelineLayout::PipelineLayout( Device const & device
		, ashes::DescriptorSetLayoutCRefArray const & setLayouts
		, ashes::PushConstantRangeCRefArray const & pushConstantRanges )
		: ashes::PipelineLayout{ device, setLayouts, pushConstantRanges }
		, m_device{ device }
	{
	}

	PipelineLayout::~PipelineLayout()
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
