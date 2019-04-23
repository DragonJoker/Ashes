/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Event_HPP___
#define ___AshesPP_Event_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

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
	public:
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
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~Event();
		/**
		*\~french
		*\brief
		*	S'enquiert de l'état de l'évènement
		*\return
		*	\p EventStatus::eSet ou \p EventStatus::eReset en cas de succès.
		*/ 
		EventStatus getStatus()const;
		/**
		*\~french
		*\brief
		*	Définit l'état de l'évènement comme signalé.
		*/ 
		void set()const;
		/**
		*\~french
		*\brief
		*	Définit l'état de l'évènement comme non signalé.
		*/ 
		void reset()const;
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkEvent.
		*\~english
		*\brief
		*	VkEvent implicit cast operator.
		*/
		inline operator VkEvent const & ( )const
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		VkEvent m_internal{ VK_NULL_HANDLE };
	};
}

#endif
