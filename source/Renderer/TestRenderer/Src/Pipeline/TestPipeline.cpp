#include "Pipeline/TestPipeline.hpp"

#include "Core/TestDevice.hpp"

namespace test_renderer
{
	Pipeline::Pipeline( Device const & device
		, renderer::PipelineLayout const & layout
		, renderer::GraphicsPipelineCreateInfo && createInfo )
		: renderer::Pipeline{ device
			, layout
			, std::move( createInfo ) }
	{
	}

	Pipeline::~Pipeline()
	{
	}
}
