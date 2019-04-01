/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Command/CommandPool.hpp>

namespace gl_renderer
{
	/**
	*\brief
	*	Emulation d'un pool de commandes, à la manière de Vulkan.
	*/
	class CommandPool
		: public ashes::CommandPool
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
		CommandPool( ashes::Device const & device
			, uint32_t queueFamilyIndex
			, VkCommandPoolCreateFlags flags = 0 );
		/**
		*\brief
		*	Crée un tampon de commandes.
		*\param[in] primary
		*	Dit si le tampon est un tampon de commandes primaire (\p true) ou secondaire (\p false).
		*\return
		*	Le tampon de commandes créé.
		*/
		ashes::CommandBufferPtr createCommandBuffer( bool primary )const override;
	};
}
