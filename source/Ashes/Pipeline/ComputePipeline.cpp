/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Pipeline/ComputePipeline.hpp"

#include "Ashes/Core/Device.hpp"

namespace ashes
{
	ComputePipeline::ComputePipeline( Device const & device
		, PipelineLayout const & layout
		, ComputePipelineCreateInfo && createInfo )
		: m_device{ device }
		, m_createInfo
		{
			std::move( createInfo.stage )
		}
		, m_layout{ layout }
	{
		registerObject( m_device, "ComputePipeline", this );
	}

	ComputePipeline::~ComputePipeline()
	{
		unregisterObject( m_device, this );
	}
}
