/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Image/ImageView.hpp>

namespace vk_renderer
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
		: public ashes::ImageView
	{
	public:
		ImageView( Device const & device
			, Image const & image
			, ashes::ImageViewCreateInfo const & createInfo );
		~ImageView();
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkImageView.
		*\~english
		*\brief
		*	VkImageView implicit cast operator.
		*/
		inline operator VkImageView const &()const
		{
			return m_view;
		}

	private:
		Device const & m_device;
		VkImageView m_view{};
	};
}
