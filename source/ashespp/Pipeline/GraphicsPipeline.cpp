/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Pipeline/GraphicsPipeline.hpp"

#include "ashespp/Core/Device.hpp"

#include <ashes/common/Exception.hpp>

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
		: m_device{ device }
		, m_createInfo{ std::move( createInfo ) }
	{
		DEBUG_DUMP( m_createInfo );
		DEBUG_WRITE( "pipeline.log" );
		auto res = m_device.vkCreateGraphicsPipelines( m_device
			, VK_NULL_HANDLE
			, 1
			, &static_cast< VkGraphicsPipelineCreateInfo const & >( m_createInfo )
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "GraphicsPipeline creation" );
		registerObject( m_device, debugName, *this );
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
		unregisterObject( m_device, *this );
		m_device.vkDestroyPipeline( m_device
			, m_internal
			, m_device.getAllocationCallbacks() );
	}
}
