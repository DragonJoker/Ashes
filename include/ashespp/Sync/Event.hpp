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
	*\brief
	*	Synchronization primitive that can be used to insert a fine-grained dependency between commands submitted to the same queue, or between the host and a queue.
	*/
	class Event
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent device.
		*/ 
		Event( Device const & device );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent device.
		*/ 
		Event( Device const & device 
			, std::string const & debugName );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent logical device.
		*\param[in] createInfo
		*	The creation informations.
		*/
		Event( Device const & device
			, VkEventCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent logical device.
		*\param[in] createInfo
		*	The creation informations.
		*/
		Event( Device const & device
			, std::string const & debugName
			, VkEventCreateInfo createInfo );
		/**
		*\brief
		*	Destructor.
		*/
		~Event();
		/**
		*\return
		*	The event status.
		*/
		EventStatus getStatus()const;
		/**
		*\brief
		*	Signals the event.
		*/ 
		void set()const;
		/**
		*\brief
		*	Unsignals the event.
		*/ 
		void reset()const;
		/**
		*\brief
		*	VkEvent implicit cast operator.
		*/
		inline operator VkEvent const & ()const
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		VkEvent m_internal{};
	};
}

#endif
