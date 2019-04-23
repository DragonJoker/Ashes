/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Fence_HPP___
#define ___AshesPP_Fence_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	Enumération des retours possibles pour une attente.
	*/
	enum class WaitResult
	{
		eSuccess,
		eTimeOut,
		eError
	};
	/**
	*\brief
	*	Classe permettant la synchronisation des opérations sur une file.
	*/
	class Fence
	{
	public:
		/**
		*\brief
		*	Constructeur
		*\param[in] device
		*	Le device parent.
		*\param[in] flags
		*	Les indicateurs de création de la barrière.
		*/ 
		Fence( Device const & device
			, VkFenceCreateFlags flags );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~Fence();
		/**
		*\brief
		*	Attend que la barrière soit signalée.
		*\param[in] timeout
		*	Le temps à attendre pour le signalement.
		*\return
		*	\p WaitResult::eSuccess ou \p WaitResult::eTimeOut en cas de succès.
		*/ 
		WaitResult wait( uint64_t timeout )const;
		/**
		*\brief
		*	Remet la barrière en non signalée.
		*/ 
		void reset()const;
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkFence.
		*\~english
		*\brief
		*	VkFence implicit cast operator.
		*/
		inline operator VkFence const &()const
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		VkFence m_internal{ VK_NULL_HANDLE };
	};
}

#endif
