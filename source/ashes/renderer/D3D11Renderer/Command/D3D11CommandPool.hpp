/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/D3D11RendererPrerequisites.hpp"

namespace ashes::d3d11
{
	class CommandPool
	{
	public:
		CommandPool( VkDevice device
			, VkCommandPoolCreateInfo createInfo );
		~CommandPool();

		void registerCommands( VkCommandBuffer commands );
		VkResult reset( VkCommandPoolResetFlags flags );
		VkResult free( ArrayView< VkCommandBuffer const > sets );

	private:
		VkDevice m_device;
		VkCommandPoolCreateInfo m_createInfo;
		VkCommandBufferArray m_commands;
	};
}
