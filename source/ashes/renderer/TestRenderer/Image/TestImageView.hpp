/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___TestRenderer_TextureView_HPP___
#define ___TestRenderer_TextureView_HPP___
#pragma once

#include "renderer/TestRenderer/TestRendererPrerequisites.hpp"

namespace ashes::test
{
	/**
	*\~french
	*\brief
	*	Description d'une vue sur une image Vulkan.
	*\~english
	*\brief
	*	Vulkan image view wrapper.
	*/
	class ImageView
	{
	public:
		ImageView( VkDevice device
			, VkImageViewCreateInfo createInfo );

		inline VkFormat getFormat()const
		{
			return m_createInfo.format;
		}

		inline VkImage getImage()const
		{
			return m_createInfo.image;
		}

		inline VkImageSubresourceRange const & getSubResourceRange()const
		{
			return m_createInfo.subresourceRange;
		}

	private:
		VkDevice m_device;
		VkImageViewCreateInfo m_createInfo;
	};
}

#endif
