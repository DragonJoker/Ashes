/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_CommandPool_HPP___
#define ___AshesPP_CommandPool_HPP___
#pragma once

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	Command buffers pool wrapper.
	*/
	class CommandPool
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] queueFamilyIndex
		*	The index of que queue family to which the pool belongs.
		*\param[in] flags
		*	VkCommandPoolCreateFlagBits combination.
		*/
		CommandPool( Device const & device
			, uint32_t queueFamilyIndex
			, VkCommandPoolCreateFlags flags = 0 );
		/**
		*\brief
		*	Destructor..
		*/
		~CommandPool();
		/**
		*\brief
		*	Creates a command buffer.
		*\param[in] primary
		*	Tells if the command buffer is primary (\p true) or secondary (\p false).
		*\return
		*	The created command buffer.
		*/
		CommandBufferPtr createCommandBuffer( bool primary = true )const;
		/**
		*\brief
		*	VkCommandPool implicit cast operator.
		*/
		inline operator VkCommandPool const & ()const
		{
			return m_internal;
		}

	protected:
		Device const & m_device;
		VkCommandPool m_internal{ VK_NULL_HANDLE };
	};
}

#endif
