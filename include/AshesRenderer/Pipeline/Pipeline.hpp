/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_Pipeline_HPP___
#define ___Ashes_Pipeline_HPP___
#pragma once

#include "AshesRenderer/Miscellaneous/GraphicsPipelineCreateInfo.hpp"

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
	class Pipeline
	{
	protected:
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
		Pipeline( Device const & device
			, PipelineLayout const & layout
			, GraphicsPipelineCreateInfo createInfo );

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~Pipeline();
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
		GraphicsPipelineCreateInfo m_createInfo;
		PipelineLayout const & m_layout;
	};
}

#endif
