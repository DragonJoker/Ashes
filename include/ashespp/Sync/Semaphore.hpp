/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_Semaphore_HPP___
#define ___AshesPP_Semaphore_HPP___
#pragma once

#include "ashespp/Sync/SyncException.hpp"

namespace ashes
{
	/**
	*\brief
	*	Semaphore wrapper.
	*/
	class Semaphore
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*/
		explicit Semaphore( Device const & device );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*/
		Semaphore( Device const & device
			, std::string const & debugName );
		/**
		*\brief
		*	Destructor.
		*/
		~Semaphore();
		/**
		*\brief
		*	VkSemaphore implicit cast operator.
		*/
		inline operator VkSemaphore const & ()const
		{
			return m_internal;
		}

		void wait( std::set< Semaphore const * > & list )const;
		void signal( Fence const * fence )const;

	private:
		void signal()const;

	private:
		Device const & m_device;
		VkSemaphore m_internal{ VK_NULL_HANDLE };
		mutable enum class State
		{
			eSignalable,
			eWaitable,
		} m_state{ State::eSignalable };
		mutable std::set< Semaphore const * > * m_list{ nullptr };
		mutable OnWaitEndConnection m_connection;
	};
}

#endif
