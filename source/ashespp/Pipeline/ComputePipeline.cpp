/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Pipeline/ComputePipeline.hpp"

namespace ashes
{
	ComputePipeline::ComputePipeline( Device const & device
		, ComputePipelineCreateInfo createInfo )
		: ComputePipeline{ device, "ComputePipeline", std::move( createInfo ) }
	{
	}

	ComputePipeline::ComputePipeline( Device const & device
		, std::string const & debugName
		, ComputePipelineCreateInfo createInfo )
		: Pipeline{ device, debugName, createInfo }
		, m_createInfo{ std::move( createInfo ) }
	{
	}
}
