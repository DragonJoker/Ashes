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
		*\param[in] createInfo
		*	The command pool creation info.
		*/
		CommandPool( Device const & device
			, VkCommandPoolCreateInfo createInfo );
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
		inline CommandPool( Device const & device
			, uint32_t queueFamilyIndex
			, VkCommandPoolCreateFlags flags = 0 )
			: CommandPool
			{
				device,
				{
					VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
					nullptr,
					flags,
					queueFamilyIndex,
				},
			}
		{
		}
		/**
		*\brief
		*	Destructor..
		*/
		~CommandPool();
		/**
		*\brief
		*	Creates a command buffer.
		*\param[in] level
		*	The Vulkan command buffer level.
		*\return
		*	The created command buffer.
		*/
		CommandBufferPtr createCommandBuffer( VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY )const;
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
