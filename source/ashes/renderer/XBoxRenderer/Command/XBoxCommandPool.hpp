/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/XBoxRendererPrerequisites.hpp"

namespace ashes::xbox
{
	class CommandPool
		: public NonCopyable
	{
	public:
		CommandPool( VkDevice device
			, VkCommandPoolCreateInfo createInfo );
		~CommandPool()noexcept;

		void registerCommands( VkCommandBuffer commands );
		VkResult reset();
		VkResult freeCommands( ArrayView< VkCommandBuffer const > sets )noexcept;

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkCommandPoolCreateInfo m_createInfo;
		VkCommandBufferArray m_commands;
	};
}
