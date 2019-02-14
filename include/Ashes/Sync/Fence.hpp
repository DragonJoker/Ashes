/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_Fence_HPP___
#define ___Ashes_Fence_HPP___
#pragma once

#include "Ashes/AshesPrerequisites.hpp"

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
	protected:
		/**
		*\brief
		*	Constructeur
		*\param[in] device
		*	Le device parent.
		*\param[in] flags
		*	Les indicateurs de création de la barrière.
		*/ 
		Fence( Device const & device
			, FenceCreateFlags flags );

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~Fence();
		/**
		*\brief
		*	Attend que la barrière soit signalée.
		*\param[in] timeout
		*	Le temps à attendre pour le signalement.
		*\return
		*	\p WaitResult::eSuccess ou \p WaitResult::eTimeOut en cas de succès.
		*/ 
		virtual WaitResult wait( uint64_t timeout )const = 0;
		/**
		*\brief
		*	Remet la barrière en non signalée.
		*/ 
		virtual void reset()const = 0;

	private:
		Device const & m_device;
	};
}

#endif
