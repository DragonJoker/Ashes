/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Pipeline/Pipeline.hpp"

namespace renderer
{
	Pipeline::Pipeline( Device const & device
		, PipelineLayout const & layout
		, GraphicsPipelineCreateInfo && createInfo )
		: m_createInfo{ std::move( createInfo ) }
		, m_layout{ layout }
	{
	}
}
