/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/Semaphore.hpp>

namespace renderer
{
	/**
	*\brief
	*	Classe de Semaphore.
	*\remarks
	*	Un sémaphore est un élément de synchronisation servant pour les files.
	*/
	class Semaphore
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le LogicalDevice parent.
		*/
		explicit Semaphore( Device const & device );
		/**
		*\brief
		*	Conversion implicite vers VkSemaphore.
		*/
		inline vk::Semaphore const & getSemaphore()const
		{
			return m_semaphore;
		}

	private:
		vk::Semaphore m_semaphore;
	};
}
