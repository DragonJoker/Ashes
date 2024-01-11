/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Pipeline_HPP___
#define ___AshesPP_Pipeline_HPP___
#pragma once

#include "ComputePipelineCreateInfo.hpp"
#include "GraphicsPipelineCreateInfo.hpp"
#include "RayTracingPipelineCreateInfo.hpp"

namespace ashes
{
	/**
	*\brief
	*	A Vulkan pipeline.
	*/
	class Pipeline
		: public VkObject
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent logical device.
		*\param[in] debugName
		*	The pipeline debug name.
		*\param[in] createInfo
		*	The creation informations.
		*/
		Pipeline( Device const & device
			, std::string const & debugName
			, ComputePipelineCreateInfo const & createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent logical device.
		*\param[in] debugName
		*	The pipeline debug name.
		*\param[in] createInfo
		*	The creation informations.
		*/
		Pipeline( Device const & device
			, std::string const & debugName
			, GraphicsPipelineCreateInfo const & createInfo );
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
		Pipeline( Device const & device
			, std::string const & debugName
			, DeferredOperation const & deferredOperation
			, RayTracingPipelineCreateInfoArray const & createInfos );
		/**
		*\brief
		*	Destructor.
		*/
		virtual ~Pipeline()noexcept;
		/**
		*\brief
		*	VkPipeline implicit cast operator.
		*/
		operator VkPipeline const & ()const noexcept
		{
			return m_internal;
		}

		Device const & getDevice()const noexcept
		{
			return m_device;
		}

	private:
		Device const & m_device;
		VkPipeline m_internal{};
	};
}

#endif
