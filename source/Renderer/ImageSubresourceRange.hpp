/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

namespace renderer
{
	/**
	*\brief
	*	Encapsulation d'un VkImageSubresourceRange.
	*/
	class ImageSubresourceRange
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		ImageSubresourceRange( ImageAspectFlags aspectMask
			, uint32_t baseMipLevel
			, uint32_t levelCount
			, uint32_t baseArrayLayer
			, uint32_t layerCount );
		/**
		*\return
		*	Le VkImageSubresourceRange.
		*/
		VkImageSubresourceRange const & getRange()const
		{
			return m_range;
		}

	private:
		VkImageSubresourceRange m_range;
	};
}
