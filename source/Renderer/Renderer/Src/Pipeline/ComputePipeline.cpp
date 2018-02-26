/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "Pipeline/ComputePipeline.hpp"

namespace renderer
{
	ComputePipeline::ComputePipeline( Device const & device
		, PipelineLayout const & layout
		, ComputePipelineCreateInfo const & createInfo )
		: m_createInfo{ createInfo }
		, m_layout{ layout }
	{
	}
}
