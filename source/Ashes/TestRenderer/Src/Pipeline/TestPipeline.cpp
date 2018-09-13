#include "Pipeline/TestPipeline.hpp"

#include "Core/TestDevice.hpp"

namespace test_renderer
{
	Pipeline::Pipeline( Device const & device
		, ashes::PipelineLayout const & layout
		, ashes::GraphicsPipelineCreateInfo && createInfo )
		: ashes::Pipeline{ device
			, layout
			, std::move( createInfo ) }
	{
	}

	Pipeline::~Pipeline()
	{
	}
}
