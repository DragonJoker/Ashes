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
#include "renderer/GlRenderer/Miscellaneous/GlPixelFormat.hpp"

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
		ImageView( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkImageViewCreateInfo createInfo
			, bool createView );

	public:
		ImageView( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkImage image );
		ImageView( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkImageViewCreateInfo createInfo );
		~ImageView();

		static VkImageViewCreateInfo adjustCreateInfo( VkDevice device
			, VkImageViewCreateInfo createInfo );

		GLuint getInternal()const noexcept;

		GlTextureType getTextureType()const noexcept
		{
			return m_gltextureType;
		}

		gl4::GlTextureViewType getViewType()const noexcept
		{
			return m_glviewType;
		}

		VkImageViewCreateInfo const & getCreateInfo()const noexcept
		{
			return m_createInfo;
		}

		VkImageViewType getType()const noexcept
		{
			return m_createInfo.viewType;
		}

		VkFormat getFormatVk()const noexcept
		{
			return m_createInfo.format;
		}

		VkComponentMapping const & getComponents()const noexcept
		{
			return m_createInfo.components;
		}

		VkImageSubresourceRange const & getSubresourceRange()const noexcept
		{
			return m_createInfo.subresourceRange;
		}

		VkImage getImage()const noexcept
		{
			return m_createInfo.image;
		}

		GlInternal getInternalFormat()const noexcept
		{
			return m_pixelFormat.internal;
		}

		inline GlFormat getUnpackFormat()const noexcept
		{
			return m_pixelFormat.unpackFormat;
		}

		inline GlType getUnpackType()const noexcept
		{
			return m_pixelFormat.unpackType;
		}

		inline GlFormat getPackFormat()const noexcept
		{
			return m_pixelFormat.packFormat;
		}

		inline GlType getPackType()const noexcept
		{
			return m_pixelFormat.packType;
		}

		bool isReadSupported()const noexcept
		{
			return m_pixelFormat.readSupport;
		}

		GlFormat getReadFormat()const noexcept
		{
			assert( isReadSupported() );
			return m_pixelFormat.readFormat;
		}

		GlType getReadType()const noexcept
		{
			assert( isReadSupported() );
			return m_pixelFormat.readType;
		}

		GlComponentMapping getFormatSwizzle()const noexcept
		{
			return m_pixelFormat.swizzle;
		}

	private:
		VkDevice m_device;
		VkImageViewCreateInfo const m_createInfo;
		GlTextureType m_gltextureType;
		gl4::GlTextureViewType m_glviewType;
		PixelFormat m_pixelFormat;
	};
}

#endif
