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
	*	Un pipeline de rendu.
	*/
	class ComputePipeline
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent logical device.
		*\param[in] createInfo
		*	The pipeline creation informations.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le device parent.
		*\param[in] createInfo
		*	Les informations de creation du pipeline.
		*/
		ComputePipeline( Device const & device
			, ComputePipelineCreateInfo createInfo );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~ComputePipeline();
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
		ComputePipelineCreateInfo m_createInfo;
		VkPipeline m_internal{ VK_NULL_HANDLE };
	};
}

#endif
