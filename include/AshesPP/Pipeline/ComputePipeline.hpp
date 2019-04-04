/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_ComputePipeline_HPP___
#define ___AshesPP_ComputePipeline_HPP___
#pragma once

#include "AshesPP/AshesPPPrerequisites.hpp"

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
		*\param[in] layout
		*	The pipeline layout.
		*\param[in] createInfo
		*	The pipeline creation informations.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le device parent.
		*\param[in] layout
		*	Le layout du pipeline.
		*\param[in] createInfo
		*	Les informations de creation du pipeline.
		*/
		ComputePipeline( Device const & device
			, PipelineLayout const & layout
			, VkComputePipelineCreateInfo createInfo );
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
		*\~english
		*\return
		*	The parent layout.
		*\~french
		*\return
		*	Le layout parent.
		*/
		inline PipelineLayout const & getLayout()const
		{
			return m_layout;
		}
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
		PipelineLayout const & m_layout;
		VkComputePipelineCreateInfo m_createInfo;
		VkPipeline m_internal{ VK_NULL_HANDLE };
	};
}

#endif
