/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/Fence.hpp>

namespace renderer
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
			, FenceCreateFlags flags = 0 );
		/**
		*\brief
		*	Attend que la barrière soit signalée.
		*\param[in] timeout
		*	Le temps à attendre pour le signalement.
		*\return
		*	\p WaitResult::eSuccess ou \p WaitResult::eTimeOut en cas de succès.
		*/ 
		WaitResult wait( uint32_t timeout );
		/**
		*\brief
		*	Remet la barrière en non signalée.
		*/ 
		void reset();
		/**
		*\return
		*	La vk::Fence.
		*/
		inline vk::Fence const & getFence()const
		{
			return m_fence;
		}

	private:
		vk::Fence m_fence;
	};
}
