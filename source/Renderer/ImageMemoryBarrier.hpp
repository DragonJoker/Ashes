/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/VkLibPrerequisites.hpp>

namespace renderer
{
	/**
	*\brief
	*	Encapsulation d'un vkImageMemoryBarrier.
	*/
	class ImageMemoryBarrier
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		ImageMemoryBarrier( AccessFlags srcAccessMask
			, AccessFlags dstAccessMask
			, ImageLayout oldLayout
			, ImageLayout newLayout
			, uint32_t srcQueueFamilyIndex
			, uint32_t dstQueueFamilyIndex
			, Texture const & image
			, ImageSubresourceRange const & subresourceRange );
		/**
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*/
		ImageMemoryBarrier( VkImageMemoryBarrier const & barrier );
		/**
		*\return
		*	Le barrière mémoire vulkan.
		*/
		VkImageMemoryBarrier const & getBarrier()const
		{
			return m_barrier;
		}

	private:
		VkImageMemoryBarrier m_barrier;
	};
}
