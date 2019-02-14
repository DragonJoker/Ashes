/**
*\file
*	Texture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___VkRenderer_TextureView_HPP___
#define ___VkRenderer_TextureView_HPP___
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Image/TextureView.hpp>

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
	class TextureView
		: public ashes::TextureView
	{
	public:
		TextureView( Device const & device
			, Texture const & image
			, ashes::ImageViewCreateInfo const & createInfo );
		~TextureView();
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

#endif
