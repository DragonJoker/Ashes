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
		Semaphore( Device const & device
			, VkSemaphore internal );
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
		*	Constructor.
		*\param[in] device
		*	The parent logical device.
		*\param[in] createInfo
		*	The creation informations.
		*/
		Semaphore( Device const & device
			, VkSemaphoreCreateInfo createInfo );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent logical device.
		*\param[in] createInfo
		*	The creation informations.
		*/
		Semaphore( Device const & device
			, std::string const & debugName
			, VkSemaphoreCreateInfo createInfo );
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

		Device const & getDevice()const
		{
			return m_device;
		}

		void wait( std::set< Semaphore const * > & list )const;
		void signal( Fence const * fence )const;

	private:
		void signal()const;

	private:
		Device const & m_device;
		VkSemaphore m_internal{ VK_NULL_HANDLE };
		bool m_ownInternal{ true };
#pragma warning( push )
#pragma warning( disable: 4068 )
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
		mutable enum class State
		{
			eSignalable,
			eWaitable,
		} m_state{ State::eSignalable };
		mutable std::set< Semaphore const * > * m_list{ nullptr };
#pragma clang diagnostic pop
#pragma warning( pop )
	};
}

#endif
