/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_CommandPool_HPP___
#define ___AshesPP_CommandPool_HPP___
#pragma once

#include "AshesPP/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	Pool de tampons de commandes.
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
			, VkCommandPoolCreateFlags flags = 0 );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~CommandPool();
		/**
		*\brief
		*	Crée un tampon de commandes.
		*\param[in] primary
		*	Dit si le tampon est un tampon de commandes primaire (\p true) ou secondaire (\p false).
		*\return
		*	Le tampon de commandes créé.
		*/
		CommandBufferPtr createCommandBuffer( bool primary = true )const;
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkCommandPool.
		*\~english
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
