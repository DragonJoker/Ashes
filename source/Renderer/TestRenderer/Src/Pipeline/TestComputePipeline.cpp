#include "Pipeline/TestComputePipeline.hpp"

#include "Core/TestDevice.hpp"

namespace test_renderer
{
	ComputePipeline::ComputePipeline( Device const & device
		, renderer::PipelineLayout const & layout
		, renderer::ComputePipelineCreateInfo && createInfo )
		: renderer::ComputePipeline{ device
			, layout
			, std::move( createInfo ) }
	{
	}

	ComputePipeline::~ComputePipeline()
	{
	}
}
