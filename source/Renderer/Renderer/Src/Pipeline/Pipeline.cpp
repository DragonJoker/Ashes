/*
This file belongs to Renderer.
See LICENSE file in root folder.
*/
#include "Pipeline/Pipeline.hpp"

namespace renderer
{
	Pipeline::Pipeline( Device const & device
		, PipelineLayout const & layout
		, GraphicsPipelineCreateInfo const & createInfo )
		: m_createInfo{ createInfo }
	{
	}
}
