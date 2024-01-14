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

		VkFormat getFormat()const noexcept
		{
			return m_createInfo.format;
		}

		VkImage getImage()const noexcept
		{
			return m_createInfo.image;
		}

		VkImageSubresourceRange const & getSubResourceRange()const noexcept
		{
			return m_createInfo.subresourceRange;
		}

		VkDevice getDevice()const noexcept
		{
			return m_device;
		}

	private:
		VkDevice m_device;
		VkImageViewCreateInfo m_createInfo;
	};
}

#endif
