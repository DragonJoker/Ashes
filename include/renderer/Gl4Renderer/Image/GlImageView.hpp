/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_TextureView_HPP___
#define ___GlRenderer_TextureView_HPP___
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"
#include "renderer/Gl4Renderer/Enum/GlTextureViewType.hpp"

namespace ashes::gl4
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
			, VkImage image );
		ImageView( VkDevice device
			, VkImageViewCreateInfo createInfo );
		/**
		*\~french
		*\brief
		*	Destructeur.
		*\~english
		*\brief
		*	Destructor.
		*/
		~ImageView();
		/**
		*\return
		*	L'image OpenGL.
		*/
		GLuint getInternal()const noexcept;

		inline VkImageViewType getType()const noexcept
		{
			return m_viewType;
		}

		inline VkFormat getFormat()const noexcept
		{
			return m_format;
		}

		inline VkComponentMapping const & getComponents()const noexcept
		{
			return m_components;
		}

		inline VkImageSubresourceRange const & getSubresourceRange()const noexcept
		{
			return m_subresourceRange;
		}

		inline VkImage getImage()const noexcept
		{
			return m_image;
		}

	private:
		VkDevice m_device;
		VkImageViewCreateFlags m_flags;
		VkImage m_image;
		VkImageViewType m_viewType;
		VkFormat m_format;
		VkComponentMapping m_components;
		VkImageSubresourceRange m_subresourceRange;
		GlTextureViewType m_target;
		GLuint m_texture{ GL_INVALID_INDEX };
	};
}

#endif
