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
		explicit Semaphore( Device const & device );
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

	private:
		Device const & m_device;
		VkSemaphore m_internal{ VK_NULL_HANDLE };
	};
}

#endif
