/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Pipeline_HPP___
#define ___AshesPP_Pipeline_HPP___
#pragma once

#include "ComputePipelineCreateInfo.hpp"
#include "GraphicsPipelineCreateInfo.hpp"

namespace ashes
{
	/**
	*\brief
	*	A Vulkan pipeline.
	*/
	class Pipeline
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
		*	Destructor.
		*/
		virtual ~Pipeline();
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
		VkPipeline m_internal{ VK_NULL_HANDLE };
	};
}

#endif
