/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Pipeline/GraphicsPipeline.hpp"

#include "AshesPP/Core/Device.hpp"
#include "AshesPP/Core/Exception.hpp"

namespace ashespp
{
	GraphicsPipeline::GraphicsPipeline( Device const & device
		, PipelineLayout const & layout
		, VkGraphicsPipelineCreateInfo createInfo )
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
		, m_layout{ layout }
	{
		DEBUG_DUMP( m_createInfo );
		DEBUG_WRITE( "pipeline.log" );
		auto res = m_device.vkCreateGraphicsPipelines( m_device
			, VK_NULL_HANDLE
			, 1
			, &m_createInfo
			, nullptr
			, &m_internal );
		checkError( res, "GraphicsPipeline creation" );
		registerObject( m_device, "GraphicsPipeline", this );
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
		unregisterObject( m_device, this );
		m_device.vkDestroyPipeline( m_device
			, m_internal
			, nullptr );
	}
}
