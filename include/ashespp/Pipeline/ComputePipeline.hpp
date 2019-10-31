/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ComputePipeline_HPP___
#define ___AshesPP_ComputePipeline_HPP___
#pragma once

#include "ComputePipelineCreateInfo.hpp"

namespace ashes
{
	/**
	*\brief
	*	A compute pipeline.
	*/
	class ComputePipeline
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent logical device.
		*\param[in] createInfo
		*	The pipeline creation informations.
		*/
		ComputePipeline( Device const & device
			, ComputePipelineCreateInfo createInfo );
		/**
		*\brief
		*	Destructor.
		*/
		~ComputePipeline();
		/**
		*\brief
		*	VkPipeline implicit cast operator.
		*/
		inline operator VkPipeline const & ()const
		{
			return m_internal;
		}

	protected:
		Device const & m_device;
		ComputePipelineCreateInfo m_createInfo;
		VkPipeline m_internal{ VK_NULL_HANDLE };
	};
}

#endif
