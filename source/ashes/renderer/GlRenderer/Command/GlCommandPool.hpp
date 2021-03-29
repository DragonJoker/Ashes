/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

#include <map>

namespace ashes::gl
{
	/**
	*\brief
	*	Emulation d'un pool de commandes, à la manière de Vulkan.
	*/
	class CommandPool
		: public AutoIdIcdObject< CommandPool >
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le device parent.
		*\param[in] queueFamilyIndex
		*	L'index de la famille à laquelle appartient le get( pool )->
		*\param[in] flags
		*	Combinaison binaire de VkCommandPoolCreateFlagBits.
		*/
		CommandPool( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkCommandPoolCreateInfo createInfo );
		~CommandPool();

		VkResult createCommandBuffer( VkCommandBuffer & commandBuffer
			, VkCommandBufferAllocateInfo const & info );
		void destroyCommandBuffer( VkCommandBuffer commandBuffer );
		VkResult reset( VkCommandPoolResetFlags flags );
		VkResult free( VkCommandBufferArray sets );

		VkDevice getDevice()const
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkAllocationCallbacks const * m_allocInfo;
		VkCommandBufferArray m_commandBuffers;
	};
}
