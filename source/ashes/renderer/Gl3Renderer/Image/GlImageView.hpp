/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_TextureView_HPP___
#define ___GlRenderer_TextureView_HPP___
#pragma once

#include "renderer/Gl3Renderer/GlRendererPrerequisites.hpp"

namespace ashes::gl3
{
	class ImageView
	{
	public:
		ImageView( VkDevice device
			, VkImage image );
		ImageView( VkDevice device
			, VkImageViewCreateInfo createInfo );
		~ImageView();
		
		GLuint getInternal()const noexcept;

		inline GlTextureType getTarget()const noexcept
		{
			return m_target;
		}
		
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
		GlTextureType m_target;
		GLuint m_texture{ GL_INVALID_INDEX };
	};
}

#endif
