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
	private:
		ImageView( VkDevice device
			, VkImageViewCreateInfo createInfo
			, bool createView );

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
			return m_createInfo.viewType;
		}

		inline VkFormat getFormat()const noexcept
		{
			return m_createInfo.format;
		}

		inline VkComponentMapping const & getComponents()const noexcept
		{
			return m_createInfo.components;
		}

		inline VkImageSubresourceRange const & getSubresourceRange()const noexcept
		{
			return m_createInfo.subresourceRange;
		}

		inline VkImage getImage()const noexcept
		{
			return m_createInfo.image;
		}

	private:
		VkDevice m_device;
		VkImageViewCreateInfo m_createInfo;
		GlTextureType m_gltextureType;
		gl4::GlTextureViewType m_glviewType;
	};
}

#endif
