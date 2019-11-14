/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	class CommandPool
	{
	public:
		CommandPool( VkDevice device
			, VkCommandPoolCreateInfo createInfo );
		~CommandPool();

		void registerCommands( VkCommandBuffer commands );
		VkResult reset( VkCommandPoolResetFlags flags );
		VkResult free( VkCommandBufferArray sets );

	private:
		VkDevice m_device;
		VkCommandPoolCreateInfo m_createInfo;
		VkCommandBufferArray m_commands;
	};
}
