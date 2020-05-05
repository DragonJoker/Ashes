/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Pipeline_HPP___
#define ___AshesPP_Pipeline_HPP___
#pragma once

#include "GraphicsPipelineCreateInfo.hpp"

namespace ashes
{
	/**
	*\brief
	*	A rendering pipeline.
	*/
	class GraphicsPipeline
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent device.
		*\param[in] layout
		*	The pipeline layout.
		*\param[in] createInfo
		*	The creation informations.
		*/
		GraphicsPipeline( Device const & device
			, GraphicsPipelineCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent device.
		*\param[in] layout
		*	The pipeline layout.
		*\param[in] createInfo
		*	The creation informations.
		*/
		GraphicsPipeline( Device const & device
			, std::string const & debugName
			, GraphicsPipelineCreateInfo createInfo );
		/**
		*\brief
		*	Destructor.
		*/
		~GraphicsPipeline();
		/**
		*\brief
		*	Conversion implicite vers VkPipeline.
		*/
		inline operator VkPipeline const & ()const
		{
			return m_internal;
		}

	protected:
		Device const & m_device;
		GraphicsPipelineCreateInfo m_createInfo;
		VkPipeline m_internal{ VK_NULL_HANDLE };
	};
}

#endif
