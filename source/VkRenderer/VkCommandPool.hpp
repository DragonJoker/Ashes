/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "VkRendererPrerequisites.hpp"

#include <VkLib/CommandPool.hpp>
#include <Renderer/CommandPool.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Encapsulation d'un vk::CommandPool.
	*/
	class CommandPool
		: public renderer::CommandPool
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
		CommandPool( renderer::Device const & device
			, uint32_t queueFamilyIndex
			, renderer::CommandPoolCreateFlags flags = 0 );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] pool
		*	Le vk::CommandPool.
		*/
		CommandPool( renderer::Device const & device
			, vk::CommandPool const & pool );
		/**
		*\brief
		*	Crée un tampon de commandes.
		*\param[in] primary
		*	Dit si le tampon est un tampon de commandes primaire (\p true) ou secondaire (\p false).
		*\return
		*	Le tampon de commandes créé.
		*/
		renderer::CommandBufferPtr createCommandBuffer( bool primary )const override;
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
		vk::CommandPoolPtr m_ownedCommandPool;
		vk::CommandPool const * m_nonOwnedCommandPool;
	};
}
