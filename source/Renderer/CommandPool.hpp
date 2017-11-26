/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/CommandPool.hpp>

namespace renderer
{
	/**
	*\brief
	*	Encapsulation d'un vk::CommandPool.
	*/
	class CommandPool
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le device parent.
		*\param[in] queueFamilyIndex
		*	L'index de la famille à laquelle appartient le pool.
		*\param[in] flags
		*	Combinaison binaire de VkCommandPoolCreateFlagBits.
		*/
		CommandPool( Device const & device
			, uint32_t queueFamilyIndex
			, CommandPoolCreateFlags flags = 0 );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] pool
		*	Le vk::CommandPool.
		*/
		CommandPool( Device const & device
			, vk::CommandPool const & pool );
		/**
		*\brief
		*	Crée un tampon de commandes.
		*\return
		*	Le tampon de commandes créé.
		*/
		CommandBufferPtr createCommandBuffer()const;
		/**
		*\return
		*	Le vk::CommandPool.
		*/
		vk::CommandPool const & getCommandPool()const
		{
			assert( m_nonOwnedCommandPool );
			return *m_nonOwnedCommandPool;
		}

	private:
		Device const & m_device;
		vk::CommandPoolPtr m_ownedCommandPool;
		vk::CommandPool const * m_nonOwnedCommandPool;
	};
}
