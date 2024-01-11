/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_RayTracingPipeline_HPP___
#define ___AshesPP_RayTracingPipeline_HPP___
#pragma once

#include "Pipeline.hpp"
#include "RayTracingPipelineCreateInfo.hpp"

namespace ashes
{
	/**
	*\brief
	*	A rendering pipeline.
	*/
	class RayTracingPipeline
		: public Pipeline
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent device.
		*\param[in] deferredOperation
		*	The deferred operation.
		*\param[in] createInfos
		*	The creation informations.
		*/
		RayTracingPipeline( Device const & device
			, DeferredOperation const & deferredOperation
			, RayTracingPipelineCreateInfoArray createInfos );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent device.
		*\param[in] debugName
		*	The object debug name.
		*\param[in] deferredOperation
		*	The deferred operation.
		*\param[in] createInfos
		*	The creation informations.
		*/
		RayTracingPipeline( Device const & device
			, std::string const & debugName
			, DeferredOperation const & deferredOperation
			, RayTracingPipelineCreateInfoArray createInfos );

		ByteArray getCaptureReplayShaderGroupHandles( uint32_t firstGroup
			, uint32_t groupCount
			, uint32_t dataSize )const;
		VkDeviceSize getShaderGroupStackSize( uint32_t group
			, VkShaderGroupShaderKHR groupShader )const;

	private:
		RayTracingPipelineCreateInfoArray m_createInfos;
	};
}

#endif
