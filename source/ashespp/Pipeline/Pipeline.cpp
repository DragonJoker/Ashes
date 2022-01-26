/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Pipeline/Pipeline.hpp"

#include "ashespp/Core/Device.hpp"
#include "ashespp/Miscellaneous/DeferredOperation.hpp"

namespace ashes
{
	Pipeline::Pipeline( Device const & device
		, std::string const & debugName
		, ComputePipelineCreateInfo const & createInfo )
		: VkObject{ debugName }
		, m_device{ device }
	{
		DEBUG_DUMP( m_createInfo );
		DEBUG_WRITE( "pipeline.log" );
		auto res = m_device.vkCreateComputePipelines( m_device
			, nullptr
			, 1
			, &static_cast< VkComputePipelineCreateInfo const & >( createInfo )
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "ComputePipeline creation" );
		registerObject( m_device, debugName, *this );
	}

	Pipeline::Pipeline( Device const & device
		, std::string const & debugName
		, GraphicsPipelineCreateInfo const & createInfo )
		: VkObject{ debugName }
		, m_device{ device }
	{
		DEBUG_DUMP( m_createInfo );
		DEBUG_WRITE( "pipeline.log" );
		auto res = m_device.vkCreateGraphicsPipelines( m_device
			, nullptr
			, 1
			, &static_cast< VkGraphicsPipelineCreateInfo const & >( createInfo )
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "GraphicsPipeline creation" );
		registerObject( m_device, debugName, *this );
	}

	Pipeline::Pipeline( Device const & device
		, std::string const & debugName
		, DeferredOperation const & deferredOperation
		, RayTracingPipelineCreateInfoArray const & createInfos )
		: VkObject{ debugName }
		, m_device{ device }
	{
		DEBUG_WRITE( "pipeline.log" );
		auto vkCreateInfos = makeVkArray< VkRayTracingPipelineCreateInfoKHR >( createInfos );
		auto res = m_device.vkCreateRayTracingPipelinesKHR( m_device
			, deferredOperation
			, nullptr
			, uint32_t( vkCreateInfos.size() )
			, vkCreateInfos.data()
			, m_device.getAllocationCallbacks()
			, &m_internal );
		checkError( res, "RayTracingPipeline creation" );
		registerObject( m_device, debugName, *this );
	}

	Pipeline::~Pipeline()
	{
		unregisterObject( m_device, *this );
		m_device.vkDestroyPipeline( m_device
			, m_internal
			, m_device.getAllocationCallbacks() );
	}
}
