/**
*\file
*	Image.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlRenderer_Texture_HPP___
#define ___GlRenderer_Texture_HPP___
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"
#include "renderer/GlRenderer/Enum/GlTextureType.hpp"
#include "renderer/GlRenderer/Miscellaneous/GlPixelFormat.hpp"

#include <ashes/common/VkTypeTraits.hpp>

namespace ashes::gl
{
	class Image
		: public IcdObject
	{
	public:
		Image( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkFormat format
			, VkExtent2D const & dimensions
			, bool swapchainImage = false );
		Image( VkAllocationCallbacks const * allocInfo
			, VkDevice device
			, VkImageCreateInfo createInfo
			, bool swapchainImage = false );
		~Image();

		VkImageView createView( VkImageViewCreateInfo const & info );
		VkResult createView( VkImageView & imageView
			, VkImageViewCreateInfo const & info );
		void destroyView( VkImageView view );
		VkMemoryRequirements getMemoryRequirements()const;
		std::vector< VkSparseImageMemoryRequirements > getSparseImageMemoryRequirements()const;

		inline VkDevice getDevice()const noexcept
		{
			return m_device;
		}
		
		inline GlTextureType getTarget()const noexcept
		{
			return m_target;
		}

		inline VkImageCreateFlags getCreateFlags()const noexcept
		{
			return m_createInfo.flags;
		}

		inline VkImageType getType()const noexcept
		{
			return m_createInfo.imageType;
		}

		inline VkFormat getFormatVk()const noexcept
		{
			return m_createInfo.format;
		}

		inline uint32_t getArrayLayers()const noexcept
		{
			return m_createInfo.arrayLayers;
		}

		inline VkImageUsageFlags getUsage()const noexcept
		{
			return m_createInfo.usage;
		}

		inline VkSampleCountFlagBits getSamples()const noexcept
		{
			return m_createInfo.samples;
		}

		inline VkExtent3D const & getDimensions()const noexcept
		{
			return m_createInfo.extent;
		}

		inline uint32_t getMipLevels()const noexcept
		{
			return m_createInfo.mipLevels;
		}

		inline GlInternal getInternalFormat()const noexcept
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

		inline bool isReadSupported()const noexcept
		{
			return m_pixelFormat.readSupport;
		}

		inline GlFormat getReadFormat()const noexcept
		{
			assert( isReadSupported() );
			return m_pixelFormat.readFormat;
		}

		inline GlType getReadType()const noexcept
		{
			assert( isReadSupported() );
			return m_pixelFormat.readType;
		}

		inline GlComponentMapping getFormatSwizzle()const noexcept
		{
			return m_pixelFormat.swizzle;
		}

		inline bool isSwapchainImage()const noexcept
		{
			return m_swapchainImage;
		}

		void setMemoryBinding( DeviceMemoryBinding const * binding )
		{
			m_binding = binding;
		}

		DeviceMemoryBinding const & getMemoryBinding()const
		{
			return *m_binding;
		}

	private:
		void doInitialiseMemoryRequirements();

	private:
		VkAllocationCallbacks const * m_allocInfo;
		VkDevice m_device;
		VkImageCreateInfo m_createInfo;
		GlTextureType m_target;
		PixelFormat m_pixelFormat;
		bool m_swapchainImage{ false };
		DeviceMemoryBinding const * m_binding{ nullptr };
		VkMemoryRequirements m_memoryRequirements;
		ImageViewCache m_views;
	};
}

#endif
