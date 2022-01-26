/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Pipeline/RayTracingPipeline.hpp"

#include "ashespp/Core/Device.hpp"

#include <ashes/common/Exception.hpp>

namespace ashes
{
	RayTracingPipeline::RayTracingPipeline( Device const & device
		, DeferredOperation const & deferredOperation
		, RayTracingPipelineCreateInfoArray createInfos )
		: RayTracingPipeline{ device
			, "RayTracingPipeline"
			, deferredOperation
			, std::move( createInfos ) }
	{
	}

	RayTracingPipeline::RayTracingPipeline( Device const & device
		, std::string const & debugName
		, DeferredOperation const & deferredOperation
		, RayTracingPipelineCreateInfoArray createInfos )
		: Pipeline{ device, debugName, deferredOperation, createInfos }
		, m_createInfos{ std::move( createInfos ) }
	{
	}

	ByteArray RayTracingPipeline::getCaptureReplayShaderGroupHandles( uint32_t firstGroup
		, uint32_t groupCount
		, uint32_t dataSize )const
	{
		ByteArray result;
		result.resize( dataSize );
		auto res = m_device.vkGetRayTracingCaptureReplayShaderGroupHandlesKHR( m_device
			, m_internal
			, firstGroup
			, groupCount
			, dataSize
			, result.data() );
		checkError( res, "vkGetRayTracingCaptureReplayShaderGroupHandlesKHR" );
		return result;
	}

	VkDeviceSize RayTracingPipeline::getShaderGroupStackSize( uint32_t group
		, VkShaderGroupShaderKHR groupShader )const
	{
		return m_device.vkGetRayTracingShaderGroupStackSizeKHR( m_device
			, m_internal
			, group
			, groupShader );
	}
}
