/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "Pipeline/ComputePipeline.hpp"

#include "Core/Device.hpp"

namespace renderer
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
