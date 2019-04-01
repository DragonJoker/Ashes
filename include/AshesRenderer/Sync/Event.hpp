/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___Ashes_Event_HPP___
#define ___Ashes_Event_HPP___
#pragma once

#include "AshesRenderer/AshesRendererPrerequisites.hpp"

namespace ashes
{
	enum class EventStatus
	{
		eSet,
		eReset,
		eError
	};
	/**
	*\~french
	*\brief
	*	Primitive de synchronisation pouvant être utilisée pour instaurer des dépendances entre des commandes d'une même file, ou entre l'hôte et la file.
	*\~ench
	*\brief
	*	Synchronization primitive that can be used to insert a fine-grained dependency between commands submitted to the same queue, or between the host and a queue.
	*/
	class Event
	{
	protected:
		/**
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le device parent.
		*\~french
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent device.
		*/ 
		Event( Device const & device );

	public:
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		virtual ~Event();
		/**
		*\~french
		*\brief
		*	S'enquiert de l'état de l'évènement
		*\return
		*	\p EventStatus::eSet ou \p EventStatus::eReset en cas de succès.
		*/ 
		virtual EventStatus getStatus()const = 0;
		/**
		*\~french
		*\brief
		*	Définit l'état de l'évènement comme signalé.
		*/ 
		virtual void set()const = 0;
		/**
		*\~french
		*\brief
		*	Définit l'état de l'évènement comme non signalé.
		*/ 
		virtual void reset()const = 0;

	private:
		Device const & m_device;
	};
}

#endif
