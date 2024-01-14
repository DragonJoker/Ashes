/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
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
		VkResult freeCommands( VkCommandBufferArray sets );

		VkDevice getDevice()const noexcept
		{
			return m_device;
		}

		VkCommandPoolCreateInfo const &  getCreateInfo()const noexcept
		{
			return m_createInfo;
		}

	private:
		VkDevice m_device;
		VkCommandPoolCreateInfo m_createInfo;
		VkCommandBufferArray m_commands;
	};
}
