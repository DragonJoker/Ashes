/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Pipeline/ComputePipeline.hpp"

#include "AshesPP/Core/Device.hpp"

namespace ashes
{
	ComputePipeline::ComputePipeline( Device const & device
		, PipelineLayout const & layout
		, VkComputePipelineCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
		, m_layout{ layout }
	{
		DEBUG_DUMP( m_createInfo );
		DEBUG_WRITE( "pipeline.log" );
		auto res = m_device.vkCreateComputePipelines( m_device
			, VK_NULL_HANDLE
			, 1
			, &m_createInfo
			, nullptr
			, &m_internal );
		checkError( res, "ComputePipeline creation" );
		registerObject( m_device, "ComputePipeline", this );
	}

	ComputePipeline::~ComputePipeline()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroyPipeline( m_device
			, m_internal
			, nullptr );
	}
}
