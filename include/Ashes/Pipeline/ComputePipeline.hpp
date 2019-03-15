/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_ComputePipeline_HPP___
#define ___Ashes_ComputePipeline_HPP___
#pragma once

#include "Ashes/Miscellaneous/ComputePipelineCreateInfo.hpp"

namespace ashes
{
	/**
	*\brief
	*	Un pipeline de rendu.
	*/
	class ComputePipeline
	{
	protected:
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
			, ComputePipelineCreateInfo createInfo );

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~ComputePipeline();
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

	protected:
		Device const & m_device;
		ComputePipelineCreateInfo m_createInfo;
		PipelineLayout const & m_layout;
	};
}

#endif
