/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_TextureView_HPP___
#define ___GlRenderer_TextureView_HPP___
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"
#include "renderer/GlRenderer/Enum/GlTextureViewType.hpp"

namespace ashes::gl
{
	namespace gl3
	{
		GlTextureType convert( VkImageViewType viewType
			, VkImageType imageType
			, VkImageCreateFlags flags
			, VkSampleCountFlagBits samples
			, uint32_t baseArrayLayer );
	}

	class ImageView
		: public IcdObject
	{
	public:
		ImageView( VkDevice device
			, VkImage image );
		ImageView( VkDevice device
			, VkImageViewCreateInfo createInfo );
		~ImageView();

		GLuint getInternal()const noexcept;

		inline GlTextureType getTextureType()const noexcept
		{
			return m_gltextureType;
		}

		inline gl4::GlTextureViewType getViewType()const noexcept
		{
			return m_glviewType;
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
		GlTextureType m_gltextureType;
		gl4::GlTextureViewType m_glviewType;
	};
}

#endif
