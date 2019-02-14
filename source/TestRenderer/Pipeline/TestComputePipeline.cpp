#include "Pipeline/TestComputePipeline.hpp"

#include "Core/TestDevice.hpp"

namespace test_renderer
{
	ComputePipeline::ComputePipeline( Device const & device
		, ashes::PipelineLayout const & layout
		, ashes::ComputePipelineCreateInfo && createInfo )
		: ashes::ComputePipeline{ device
			, layout
			, std::move( createInfo ) }
	{
	}

	ComputePipeline::~ComputePipeline()
	{
	}
}
