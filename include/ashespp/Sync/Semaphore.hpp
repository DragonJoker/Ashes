/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Semaphore_HPP___
#define ___AshesPP_Semaphore_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
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
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~Semaphore();
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkSemaphore.
		*\~english
		*\brief
		*	VkSemaphore implicit cast operator.
		*/
		inline operator VkSemaphore const & ()const
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		VkSemaphore m_internal{ VK_NULL_HANDLE };
	};
}

#endif
