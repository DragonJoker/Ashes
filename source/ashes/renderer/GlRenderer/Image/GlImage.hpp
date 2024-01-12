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

#pragma warning( push )
#pragma warning( disable: 5262 )
#include <mutex>
#pragma warning( pop )

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
		~Image()noexcept;

		VkImageView createView( VkImageViewCreateInfo info );
		VkResult createView( VkImageView & imageView
			, VkImageViewCreateInfo info );
		void destroyView( VkImageView view )noexcept;
		VkMemoryRequirements getMemoryRequirements()const;
		std::vector< VkSparseImageMemoryRequirements > getSparseImageMemoryRequirements()const;

		VkDevice getDevice()const noexcept
		{
			return m_device;
		}
		
		GlTextureType getTarget()const noexcept
		{
			return m_target;
		}

		VkImageCreateFlags getCreateFlags()const noexcept
		{
			return m_createInfo.flags;
		}

		VkImageType getType()const noexcept
		{
			return m_createInfo.imageType;
		}

		VkFormat getFormatVk()const noexcept
		{
			return m_createInfo.format;
		}

		uint32_t getArrayLayers()const noexcept
		{
			return m_createInfo.arrayLayers;
		}

		VkImageUsageFlags getUsage()const noexcept
		{
			return m_createInfo.usage;
		}

		VkSampleCountFlagBits getSamples()const noexcept
		{
			return m_createInfo.samples;
		}

		VkExtent3D const & getDimensions()const noexcept
		{
			return m_createInfo.extent;
		}

		uint32_t getMipLevels()const noexcept
		{
			return m_createInfo.mipLevels;
		}

		GlInternal getInternalFormat()const noexcept
		{
			return m_pixelFormat.internal;
		}

		GlFormat getUnpackFormat()const noexcept
		{
			return m_pixelFormat.unpackFormat;
		}

		GlType getUnpackType()const noexcept
		{
			return m_pixelFormat.unpackType;
		}

		GlFormat getPackFormat()const noexcept
		{
			return m_pixelFormat.packFormat;
		}

		GlType getPackType()const noexcept
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

		bool isSwapchainImage()const noexcept
		{
			return m_swapchainImage;
		}

		void setMemoryBinding( DeviceMemoryBinding const * binding )noexcept
		{
			m_binding = binding;
		}

		DeviceMemoryBinding const & getMemoryBinding()const noexcept
		{
			return *m_binding;
		}

	private:
		void doInitialiseMemoryRequirements();

	private:
		VkAllocationCallbacks const * m_allocInfo{};
		VkDevice m_device{};
		VkImageCreateInfo m_createInfo{};
		GlTextureType m_target{};
		PixelFormat m_pixelFormat{};
		bool m_swapchainImage{ false };
		DeviceMemoryBinding const * m_binding{ nullptr };
		VkMemoryRequirements m_memoryRequirements{};
		std::mutex m_mtx;
		ImageViewCache m_views;
	};
}

#endif
