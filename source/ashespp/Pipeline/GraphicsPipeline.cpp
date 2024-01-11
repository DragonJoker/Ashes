/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Pipeline/GraphicsPipeline.hpp"

namespace ashes
{
	GraphicsPipeline::GraphicsPipeline( Device const & device
		, GraphicsPipelineCreateInfo createInfo )
		: GraphicsPipeline{ device, "GraphicsPipeline", std::move( createInfo ) }
	{
	}

	GraphicsPipeline::GraphicsPipeline( Device const & device
		, std::string const & debugName
		, GraphicsPipelineCreateInfo createInfo )
		: Pipeline{ device, debugName, createInfo }
		, m_createInfo{ std::move( createInfo ) }
	{
	}
}
