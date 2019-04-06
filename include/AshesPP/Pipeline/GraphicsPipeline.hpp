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
	*\~english
	*\brief
	*	A rendering pipeline.
	*\~french
	*\brief
	*	Un pipeline de rendu.
	*/
	class GraphicsPipeline
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent device.
		*\param[in] layout
		*	The pipeline layout.
		*\param[in] createInfo
		*	The creation informations.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*\param[in] layout
		*	Le layout du pipeline.
		*\param[in] createInfo
		*	Les informations de création.
		*/
		GraphicsPipeline( Device const & device
			, GraphicsPipelineCreateInfo createInfo );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~GraphicsPipeline();
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkPipeline.
		*\~english
		*\brief
		*	VkPipeline implicit cast operator.
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
